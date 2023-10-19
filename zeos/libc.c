/*
 * libc.c 
 */

#include <libc.h>
#include <errno.h>
#include <types.h>

void itoa(int a, char *b)
{
  int i, i1;
  char c;
  
  if (a==0) { b[0]='0'; b[1]=0; return ;}
  
  i=0;
  while (a>0)
  {
    b[i]=(a%10)+'0';
    a=a/10;
    i++;
  }
  
  for (i1=0; i1<i/2; i1++)
  {
    c=b[i1];
    b[i1]=b[i-i1-1];
    b[i-i1-1]=c;
  }
  b[i]=0;
}

int strlen(char *a)
{
  int i;
  
  i=0;
  
  while (a[i]!=0) i++;
  
  return i;
}

int errno;

void perror()
{
  write(1,"Error: ", 7);
  switch (errno) {
    case EPERM:
      write(1,"Operation not permitted", 50);
      break;
    case ENOENT:
      write(1,"No such file or directory", 50);
      break;
    case ESRCH:
      write(1,"No such process", 50);
      break;
    case EINTR:
      write(1,"Interrupted system call", 50);
      break;
    case EIO:
      write(1,"I/O error", 50);
      break;
    case ENXIO:
      write(1,"No such device or address", 50);
      break;
    case E2BIG:
      write(1,"Argument list too long", 50);
      break;
    case ENOEXEC:
      write(1,"Exec format error", 50);
      break;
    case EBADF:
      write(1,"Bad file number", 50);
      break;
    case ECHILD:
      write(1,"No child processes", 50);
      break;
    case EAGAIN:
      write(1,"Try again", 50);
      break;
    case ENOMEM:
      write(1,"Out of memory", 50);
      break;
    case EACCES:
      write(1,"Permission denied", 50);
      break;
    case EFAULT:
      write(1,"Bad address", 50);
      break;
    case ENOTBLK:
      write(1,"Block device required", 50);
      break;
    case EBUSY:
      write(1,"Device or resource busy", 50);
      break;
    case EEXIST:
      write(1,"File exists", 50);
      break;
    case EXDEV:
      write(1,"Cross-device link", 50);
      break;
    case ENODEV:
      write(1,"No such device", 50);
      break;
    case ENOTDIR:
      write(1,"Not a directory", 50);
      break;
    case EISDIR:
      write(1,"Is a directory", 50);
      break;
    case EINVAL:
      write(1,"Invalid argument", 50);
      break;
    case ENFILE:
      write(1,"File table overflow", 50);
      break;
    case EMFILE:
      write(1,"Too many open files", 50);
      break;
    case ENOTTY:
      write(1,"Not a typewriter", 50);
      break;
    case ETXTBSY:
      write(1,"Text file busy", 50);
      break;
    case EFBIG:
      write(1,"File too large", 50);
      break;
    case ENOSPC:
      write(1,"No space left on device", 50);
      break;
    case ESPIPE:
      write(1,"Illegal seek", 50);
      break;
    case EROFS:
      write(1,"Read-only file system", 50);
      break;
    case EMLINK:
      write(1,"Too many links", 50);
      break;
    case EPIPE:
      write(1,"Broken pipe", 50);
      break;
    case EDOM:
      write(1,"Math argument out of domain of func", 50);
      break;
    case ERANGE:
      write(1,"Math result not representable", 50);
      break;
    case EDEADLK:
      write(1,"Resource deadlock would occur", 50);
      break;
    case ENAMETOOLONG:
      write(1,"File name too long", 50);
      break;
    case ENOLCK:
      write(1,"No record locks available", 50);
      break;
  }

//  printk_color("Error: ", B_RED, F_WHITE);
//  printk_color(error_msg, B_RED, F_WHITE);
  write(1,"\n", 1);

   
}
