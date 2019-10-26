#ifndef memoryManager_h
#define memoryManager_h

#include <stddef.h>
#include <stdint.h>
#include "lib.h"


#define SIZE_OF_MEMORY 0x8000000   //128Megas
#define SIZE_OF_PAGE 0x8000000/1000000
#define MAX_CANT_OF_PAGES 1000000


typedef struct memoryList{
    size_t freePages;
    size_t cantPages;
    struct page * first;  
    struct page * last;
}memoryList;

typedef struct page{
    int free;
    size_t size;
    uint64_t * address;
    struct page * next;
    struct page * prev;
}page;

void * malloc(size_t space);
void free(void * address);

page * findPage(void * address);
void printPage(uint64_t * address);

#endif