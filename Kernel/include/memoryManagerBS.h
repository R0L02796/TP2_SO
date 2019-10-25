#ifndef memoryManagerBS_h
#define memoryManagerBS_h

#include <stddef.h>
#include <stdint.h>
#include "lib.h"


#define SIZE_OF_MEMORY 0x10000000   //(0xFFFFFFFFFFFFFFFF-0x0000000000100000) is what i can use.
                                    // dont want to use a big number like that .
#define MAX_LEVEL 10                //0x10000000=2^28 should be if i want just one lv
#define MIN_LEVEL 3
#define INIT_CANT_OF_PAGES 100         //0x10000000/(1<<10) is what i can have as max. im going with 100 for the moment
#define MAX_CANT_OF_PAGES 1000000



typedef struct memoryList{
    int minLv;
    int freePages;
    int cantPages;
    struct page * lvVec[MAX_LEVEL-MIN_LEVEL+1];  
    int freePagesLv [MAX_LEVEL-MIN_LEVEL+1];
}memoryList;

typedef struct page{
    int free;
    size_t size;
    uint64_t * address;
    struct page * next;
    int lv;
}page;

void * malloc(size_t space);
void free(void * address);

page * findPage(void * address);
void printPage(uint64_t * address);

#endif