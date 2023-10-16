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

void perror(void) 
{
  if (errno == EINVAL){
    write(1, "\nwrite error: size < 0", 23);
  }
  else if (errno == ENOMEM){
    write(1, "\nwrite error: buffer == NULL",28);
  }
  else if (errno == EBADF){
    write(1, "\nwrite error: wrong fd",23);
  }
  else {
    write(1, "\nerror code: ", 15);
    char buff[4];
    itoa(errno, buff);
    write(1,buff,strlen(buff));
  }

}