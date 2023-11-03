#include <libc.h>
#include <errno.h>

char buff[24];

int pid;

void perror (void);

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */

	int error;


	//write
	char buff[128] = "\n\nIt works :D\n";
	error = write(1,buff,strlen(buff));
	if (error < 0) perror();
	//------------------

	//perror								//uncomment to test
	char buff1[128] = "\nPerror:\n";
	error = write(1,buff1,strlen(buff1));
	if(write(1,buff,-1) == -1) perror(); 
	//-----------------

	//gettime								//uncomment to test
	char buff2[128] = "\nGettime: ";
	error = write(1,buff2,strlen(buff2));
	char buffer[8];
	int time = gettime();
	itoa(time,buffer);
	write(1,buffer,strlen(buffer));
	
	//------------------

	//test page fault						//uncomment to test
	// char* p = 0;
	// *p = 'x';
	//------------------

	//getpid
	
	char buff3[128] = "\nGetpid: ";
	error = write(1,buff3,strlen(buff3));
	char buffer1[8];
	int p = getpid();
	itoa(p,buffer1);
    write(1,buffer1,strlen(buffer1));
    
	//------------------
    
  while(1) { 
  	//write(1,"c",1);
  }
}



