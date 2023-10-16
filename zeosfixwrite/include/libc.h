/*
 * libc.h - macros per fer els traps amb diferents arguments
 *          definició de les crides a sistema
 */
 
#ifndef __LIBC_H__
#define __LIBC_H__

#include <stats.h>
#include <io.h>

extern int errno;

int write(int fd, char *buffer, int size);

void itoa(int a, char *b);

int strlen(char *a);
int strcpy(char *s, char*t);

int write (int fd, char * buffer, int size);
int gettime();

int getpid();

int fork();

void exit();

void perror();


#endif  /* __LIBC_H__ */
