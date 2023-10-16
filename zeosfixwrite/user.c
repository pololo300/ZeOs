#include <libc.h>
#include <errno.h>
char buff[24];

int pid;

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */

  char* p = 2;
  *p = 'x';


  int i = 7;
  write(10, "I'm an user!\n", 20);
  perror();

  char time[30];
  for (int i = 0; i <10 ; ++i)
  {
    itoa(gettime(), time);
    write(1,time,10);
    write(1,"\n",10);
  }
    
  while(1) { }
}
