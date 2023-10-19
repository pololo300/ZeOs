/*
 * sys.c - Syscalls implementation
 */
#include <devices.h>

#include <utils.h>

#include <io.h>

#include <mm.h>

#include <mm_address.h>

#include <sched.h>

#include <errno.h>

#define LECTURA 0
#define ESCRIPTURA 1

extern int zeos_ticks;

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

char buffer_so[4096];

int sys_write(int fd, char * buffer, int size) 
{
	//fd: file descriptor. In this delivery it must always be 1.
	//buffer: pointer to the bytes.
	//size: number of bytes.
	//return ’ Negative number in case of error (specifying the kind of error) and
	//the number of bytes written if OK. 

	//check fd
	int error = check_fd (fd, ESCRIPTURA);
	if (error < 0) return -EBADF;
	//check size
	if (size < 0) return -EINVAL;
	//check buffer
	if (access_ok(VERIFY_WRITE, buffer, size) == 0) return -ENOMEM;


	// USUARIO ESCRIBE > 4096 ???????????????
	error = copy_from_user(buffer, buffer_so, size);
	if (error < 0) return error;


	return sys_write_console(buffer_so, size);
}

int sys_gettime()
{
	return zeos_ticks;
}