#ifndef STDLIB_H
#define STDLIB_H

#include <stdint.h>

// Prints string with formats
void printf(char * fmt, ...);

// Prints char
void putChar(char c);

// Prints decimal
void putDec(int i);

// Reads a char from buffer
char getChar();

// Trensforms a decimal to string
char * decToStr(int num, char * buffer);

// Scans from buffer and prints the contents scanned
void scanAndPrint(char * buffer);

// Compares two strings
int strCmp(char * a, char * b);

// Clears buffer
void clearBuffer(char * buffer);

// Returns absolut value of given int
int abs(int n);

// Prints string
void putStr(char * string);

void srand(unsigned long int seed);

void lcg(unsigned long int *x, unsigned long int a, int c,
         unsigned long int m) ;

void * memcpy(void * destination, const void * source, uint64_t length)
;

int strnCmp(char * a, char * b, int n);

int strLen(char *str);

int atoi(char * string);

unsigned long int rand();

#endif
