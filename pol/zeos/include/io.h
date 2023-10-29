#ifndef __IO_H__
#define __IO_H__

#include <types.h>

#define F_BLACK 0x0
#define F_BLUE 0x1
#define F_GREEN 0x2
#define F_CYAN 0x3
#define F_RED 0x4
#define F_MAGENTA 0x5
#define F_BROWN 0x6
#define F_LIGHT_GREY 0x7
#define F_DARK_GREY 0x8
#define F_LIGHT_BLUE 0x9
#define F_LIGHT_GREEN 0xA
#define F_LIGHT_CYAN 0xB
#define F_LIGHT_RED 0xC
#define F_LIGHT_MAGENTA 0xD
#define F_YELLOW 0xE
#define F_WHITE 0xF

#define B_BLACK 0x0
#define B_BLUE 0x1
#define B_GREEN 0x2
#define B_CYAN 0x3
#define B_RED 0x4
#define B_MAGENTA 0x5
#define B_BROWN 0x6
#define B_LIGHT_GREY 0x7


/** Screen functions **/
/**********************/

Byte inb (unsigned short port);
void newline();
void scroll();
void printc(char c);
void printc_color(char c, int bc, int fc);
void printc_xy(Byte x, Byte y, char c);
void printk(char *string);
void printk_color(char *string, int bc, int fc);

#endif  /* __IO_H__ */
