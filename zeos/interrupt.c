/*
 * interrupt.c -
 */
#include <types.h>
#include <interrupt.h>
#include <segment.h>
#include <hardware.h>
#include <io.h>
#include <errno.h>

#include <zeos_interrupt.h>

Gate idt[IDT_ENTRIES];
Register    idtR;

extern int zeos_ticks;

char char_map[] =
{
  '\0','\0','1','2','3','4','5','6',
  '7','8','9','0','\'','¡','\0','\0',
  'q','w','e','r','t','y','u','i',
  'o','p','`','+','\0','\0','a','s',
  'd','f','g','h','j','k','l','ñ',
  '\0','º','\0','ç','z','x','c','v',
  'b','n','m',',','.','-','\0','*',
  '\0','\0','\0','\0','\0','\0','\0','\0',
  '\0','\0','\0','\0','\0','\0','\0','7',
  '8','9','-','4','5','6','+','1',
  '2','3','0','\0','\0','\0','<','\0',
  '\0','\0','\0','\0','\0','\0','\0','\0',
  '\0','\0'
};

void setInterruptHandler(int vector, void (*handler)(), int maxAccessibleFromPL)
{
  /***********************************************************************/
  /* THE INTERRUPTION GATE FLAGS:                          R1: pg. 5-11  */
  /* ***************************                                         */
  /* flags = x xx 0x110 000 ?????                                        */
  /*         |  |  |                                                     */
  /*         |  |   \ D = Size of gate: 1 = 32 bits; 0 = 16 bits         */
  /*         |   \ DPL = Num. higher PL from which it is accessible      */
  /*          \ P = Segment Present bit                                  */
  /***********************************************************************/
  Word flags = (Word)(maxAccessibleFromPL << 13);
  flags |= 0x8E00;    /* P = 1, D = 1, Type = 1110 (Interrupt Gate) */

  idt[vector].lowOffset       = lowWord((DWord)handler);
  idt[vector].segmentSelector = __KERNEL_CS;
  idt[vector].flags           = flags;
  idt[vector].highOffset      = highWord((DWord)handler);
}

void setTrapHandler(int vector, void (*handler)(), int maxAccessibleFromPL)
{
  /***********************************************************************/
  /* THE TRAP GATE FLAGS:                                  R1: pg. 5-11  */
  /* ********************                                                */
  /* flags = x xx 0x111 000 ?????                                        */
  /*         |  |  |                                                     */
  /*         |  |   \ D = Size of gate: 1 = 32 bits; 0 = 16 bits         */
  /*         |   \ DPL = Num. higher PL from which it is accessible      */
  /*          \ P = Segment Present bit                                  */
  /***********************************************************************/
  Word flags = (Word)(maxAccessibleFromPL << 13);

  //flags |= 0x8F00;    /* P = 1, D = 1, Type = 1111 (Trap Gate) */
  /* Changed to 0x8e00 to convert it to an 'interrupt gate' and so
     the system calls will be thread-safe. */
  flags |= 0x8E00;    /* P = 1, D = 1, Type = 1110 (Interrupt Gate) */

  idt[vector].lowOffset       = lowWord((DWord)handler);
  idt[vector].segmentSelector = __KERNEL_CS;
  idt[vector].flags           = flags;
  idt[vector].highOffset      = highWord((DWord)handler);
}


void clock_handler();
void keyboard_handler();
void pagefault_handler();
void syscall_handler_sysenter();
void writeMSR(long msr, long high_value, long low_value);

void clock_routine()
{
  zeos_show_clock();
  ++zeos_ticks;
}

char buff[1];

void keyboard_routine()
{
  unsigned char data = inb(0x60);
  if ( (data & 0x80) != 0x00)  // check is make or break 
    return;

  char c = (char) char_map[(int)data & 0x7F];
  if ( c == '\0' )
    c = 'C';

  printc_xy(72, 1, c);
}

void pagefault_routine(unsigned long error, unsigned long address)
{ 
  printk_color("\n\nProcess generates a PAGE FAULT exception at EIP: 0x", B_RED, F_WHITE);
  
  unsigned long aux;
  char c;

  //address = address << 12; //only need last 20 bits of address
  
  // divide 20 bits long into 4 bits parts, then convert these parts into chars and print them  
  for (short i = 0; i < 8; i++) {
    aux = address >> 28;          //aux = 4 higher bits of address

    if (aux < 10) c = '0' + aux;  //convert aux to ASCII
    else c = 'A' + (aux - 10);

    printc_color(c, B_RED, F_WHITE);
    address = address << 4;       //shift left address 4 bits to use the 4 higher bits the next iteration
  }

  while(1);
}

void setIdt()
{
  /* Program interrups/exception service routines */
  idtR.base  = (DWord)idt;
  idtR.limit = IDT_ENTRIES * sizeof(Gate) - 1;
  
  set_handlers();

  /* ADD INITIALIZATION CODE FOR INTERRUPT VECTOR */

  setInterruptHandler(32, clock_handler, 0);
  setInterruptHandler(33, keyboard_handler, 0);
  setInterruptHandler(14, pagefault_handler, 0);
  

  writeMSR(0x174, 0, __KERNEL_CS);
  writeMSR(0x175, 0, INITIAL_ESP);
  writeMSR(0x176, 0, (long)&syscall_handler_sysenter);


  set_idt_reg(&idtR);
}
