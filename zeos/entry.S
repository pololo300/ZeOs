/*
 * entry.S - Entry point to system mode from user mode
 */

#include <asm.h>
#include <segment.h>
#include <errno.h>


/**************************************************/
/**** Save & Restore ******************************/
/**                                              **/
/** When we change to privilege level 0 (kernel) **/
/** (through an interrupt, a system call, an     **/
/** exception ...) we must save the state of the **/
/** currently running task (save).               **/
/**                                              **/
/** Stack layout in 'systemCall':                **/
/**                                              **/
/**   0(%esp) - %edx    \                        **/
/**   4(%esp) - %ecx     |                       **/
/**   8(%esp) - %ebx     |                       **/
/**   C(%esp) - %esi     | Register saved        **/
/**  10(%esp) - %edi     |  by 'save'            **/
/**  14(%esp) - %ebp     |                       **/
/**  18(%esp) - %eax     |                       **/
/**  1C(%esp) - %ds      |                       **/
/**  20(%esp) - %es      |                       **/
/**  24(%esp) - %fs      |                       **/
/**  28(%esp) - %gs     /                        **/
/**  2C(%esp) - %eip    \                        **/
/**  30(%esp) - %cs      |                       **/
/**  34(%esp) - %eflags  |  Return context saved **/
/**  38(%esp) - %oldesp  |   by the processor.   **/
/**  3C(%esp) - %oldss  /                        **/
/**                                              **/
/**************************************************/

#define SAVE_ALL \
      pushl %gs; \
      pushl %fs; \
      pushl %es; \
      pushl %ds; \
      pushl %eax; \
      pushl %ebp; \
      pushl %edi; \
      pushl %esi; \
      pushl %ebx; \
      pushl %ecx; \
      pushl %edx; \
      movl $__KERNEL_DS, %edx;    \
      movl %edx, %ds;           \
      movl %edx, %es

#define RESTORE_ALL \
      popl %edx; \
      popl %ecx; \
      popl %ebx; \
      popl %esi; \
      popl %edi; \
      popl %ebp; \
      popl %eax; \
      popl %ds; \
      popl %es; \
      popl %fs; \
      popl %gs;
      
#define EOI \
      movb $0x20, %al ; \
      outb %al, $0x20 ;

ENTRY(clock_handler)
      SAVE_ALL;
      EOI;
      call clock_routine;
      RESTORE_ALL;
      iret;
      
ENTRY(keyboard_handler)
      SAVE_ALL;
      EOI;
      call keyboard_routine;
      RESTORE_ALL;
      iret;

ENTRY(pagefault_handler)
      call pagefault_routine;

ENTRY(syscall_handler_sysenter)
      pushl $__USER_DS;
      push %ebp;
      pushfl;
      push $__USER_CS;
      push 4(%ebp);
      SAVE_ALL;
      cmpl $0, %eax;
      jl sysenter_err;
      cmpl $MAX_SYSCALL, %eax;
      jg sysenter_err;
      call *sys_call_table(, %eax, 0x04)
      jmp sysenter_fin;
sysenter_err:
      movl $-ENOSYS, %eax;
sysenter_fin:
      movl %eax, 0x18(%esp);
      RESTORE_ALL;
      movl (%esp), %edx;
      movl 12(%esp), %ecx;
      sti; 
      sysexit;

# system_call_handler with int

#ENTRY(system_call_handler)
#      SAVE_ALL
#      cmpl $0, %EAX
#      jl err
#      cmpl $MAX_SYSCALL, %EAX
#      jg err
#      call *sys_call_table(, %EAX, 0x04)
#      jmp fin
#err:
#      movl $-ENOSYS, %EAX
#fin:
#      movl %EAX, 0x18(%esp)
#      RESTORE_ALL
#      iret


ENTRY(writeMSR)
      push %ebp;
      mov %esp, %ebp;
      movl 8(%ebp), %ecx;
      movl 12(%ebp), %edx;
      movl 16(%ebp), %eax;
      wrmsr;
      movl %ebp,%esp;
      pop %ebp;
      ret;


