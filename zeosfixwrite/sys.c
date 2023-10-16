/*
 * sys.c - Syscalls implementation
 */
#include <interrupt.h>
#include <devices.h>

#include <utils.h>

#include <io.h>

#include <mm.h>

#include <mm_address.h>

#include <sched.h>

#include <errno.h>

#define LECTURA 0
#define ESCRIPTURA 1

int check_fd(int fd, int permissions)
{
  if (fd!=1) return -9; /*EBADF*/
  if (permissions!=ESCRIPTURA) return -13; /*EACCES*/
  return 0;
}

int sys_ni_syscall()
{
	return -38; /*ENOSYS*/
}

int sys_getpid()
{
	return current()->PID;
}

int sys_fork()
{
  int PID=-1;

  // creates the child process
  
  return PID;
}

void sys_exit()
{  
}

int sys_write(int fd, char * buffer, int size)
{
  int error;

  if (error = check_fd(fd, ESCRIPTURA))
    return error;

  if ( buffer == NULL)
    return -EINVAL;

  if (size < 1)
    return -EINVAL; 

  char sys_buff[size];
  if (copy_from_user(buffer, sys_buff, size))
    return -EFAULT; 

  return sys_write_console(sys_buff, size); 
}

int sys_gettime()
{
  return zeos_ticks;
}

