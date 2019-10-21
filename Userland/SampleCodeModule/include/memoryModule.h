#ifndef MEMORYMODULE_H
#define MEMORYMODULE_H

#include <stddef.h>

void* malloc(size_t size);
void free(void* address);
void printPage(void * address);

#endif