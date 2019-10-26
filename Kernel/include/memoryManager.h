#ifndef memoryManager_h
#define memoryManager_h

#include <stddef.h>
#include <stdint.h>
#include "lib.h"


void * malloc(size_t space);
void free(void * address);

page * findPage(void * address);
void printPage(uint64_t * address);



////////////////////////////////////////////////FIRST FIT//////////////////////////////////////////////////////////

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


//////////////////////////////////////////////////////////BUDDY SYSTEM///////////////////////////////////////////////////
/*
#define SIZE_OF_MEMORY 0x8000000   //128 Megas
#define MAX_LEVEL 10                //0x10000000=2^28 should be if i want just one lv
#define MIN_LEVEL 3
#define INIT_CANT_OF_PAGES 0x8000000/(1<<10)     //SIZE_OF_MEMORY/(1<<MAX_LEVEL)
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
*/

#endif

