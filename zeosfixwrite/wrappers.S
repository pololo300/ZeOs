#include <asm.h>
#include <segment.h>

ENTRY(write)
  push   %ebp
  movl   %esp, %ebp
  push   %ebx
  movl   8(%ebp) , %edx  # fd
  movl   12(%ebp), %ecx  # buffer
  movl   16(%ebp), %ebx  # size
  movl   $4, %eax        # identifier
  int    $0x80           # goes to sys_call_handler
  pop    %ebx
  cmp   $0, %eax        
  jns   end_write       # ends if eax > 0
  push  %eax
  call  set_errno
  pop   %eax
  mov   $-1, %eax
end_write:
  mov   %ebp, %esp
  pop   %ebp
  ret


ENTRY(gettime)
  push %ebp
  movl   %esp, %ebp
  movl   $10, %eax        # identifier
  int    $0x80           # goes to sys_call_handler
  cmp   $0, %eax        
  mov   %ebp, %esp
  pop   %ebp
  ret
