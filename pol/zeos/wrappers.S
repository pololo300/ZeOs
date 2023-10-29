/*
 * wrappers.S
 */

#include <asm.h>
#include <segment.h>
#include <errno.h>

ENTRY(write)
	pushl %ebp;
    movl %esp, %ebp;
    pushl %ebx;				//registers that need to be saved
    						//if used: %ebx %esi %edi
    						//GUARDA EDX Y ECX -----------------------------
	
	movl 8(%ebp), %edx;	//parameter passing
    	movl 12(%ebp), %ecx;
    	movl 16(%ebp), %ebx;

	movl $4, %eax;			//system call identifier -> %eax

	pushl %edx;				//@user code to returm back
	pushl %ecx;				//@top user stack
	push $ret;				//@return after sysenter
	pushl %ebp;				//fake dynamic link: sysexit use %ebp to find
							//EIP and ESP from system mode ??????
	movl %esp, %ebp;
	sysenter;
ret:
	popl %ebp;
	addl $4, %esp;			//pop @ret user
	popl %edx;
	popl %ecx;

	cmpl $0, %eax;			//error?
	jge no_error;

	negl %eax;
	movl %eax, errno;		//sysenter returns $-ENOSYS in 
	movl $-1, %eax;			//case of error 

no_error:
	popl %ebx;
	movl %ebp, %esp;
	popl %ebp;
	ret;


# write handler with

#ENTRY(write)
#  push   %ebp
#  movl   %esp, %ebp
#  push   %ebx
#  movl   8(%ebp) , %edx  # fd
#  movl   12(%ebp), %ecx  # buffer
#  movl   16(%ebp), %ebx  # size
#  movl   $4, %eax        # identifier
#  int    $0x80           # goes to sys_call_handler
#  pop    %ebx
#  cmp   $0, %eax        
#  jns   end_write       # ends if eax > 0
#  negl %eax;
#  movl %eax, errno;
#  mov   $-1, %eax
#end_write:
#  mov   %ebp, %esp
#  pop   %ebp
#  ret


ENTRY(gettime)
	pushl %ebp;
    movl %esp, %ebp;
    pushl %ebx;

    movl $10, %eax;

    pushl %edx;				//@user code to returm back
	pushl %ecx;				//@top user stack
	push $ret_gett;			//@return after sysenter
	pushl %ebp;				//fake dynamic link: sysexit use %ebp to find
							//EIP and ESP from system mode ??????
	movl %esp, %ebp;
	sysenter;

ret_gett:
	movl %ebp, %esp;
	popl %ebp;
	ret;

# gettime with int

#ENTRY(gettime)
#  push %ebp
#  movl   %esp, %ebp
#  movl   $10, %eax        # identifier
#  int    $0x80           # goes to sys_call_handler
#  cmp   $0, %eax        
#  mov   %ebp, %esp
#  pop   %ebp
#  ret
