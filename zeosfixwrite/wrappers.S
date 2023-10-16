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