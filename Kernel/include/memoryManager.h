#ifndef memoryManager_h
#define memoryManager_h

#include <stddef.h>
#include <stdint.h>
#include "./lib.h"


#define SIZE_OF_MEMORY 0x10000000   //(0xFFFFFFFFFFFFFFFF-0x0000000000100000) is what i can use.
                                    // dont want to use a big number like that .
#define MAX_CANT_OF_PAGES 1000000


typedef struct memoryList{
    size_t freePages;
    size_t cantPages;
    page * first;  

}memoryList;

typedef struct page{
    int free;
    size_t size;
    uint64_t * address;
    struct page * next;
    struct page * prev;
}page;

#endif