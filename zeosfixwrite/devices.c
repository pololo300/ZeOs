#include <io.h>
#include <utils.h>
#include <list.h>

// Queue for blocked processes in I/O 
struct list_head blocked;

int sys_write_console(char *buffer,int size)
{
  int printed_chars = 0;
  

  while (*buffer != '\0' && printed_chars < size)
  {
    printed_chars++;
    printc(*buffer++);
  }
  return printed_chars;
}
