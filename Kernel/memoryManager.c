#include "memoryManager.h"

void newMemory(uint64_t * adrress);
page * newPage(uint64_t * paddress,page * prev);



static const uint64_t * startAdrress = (uint8_t *)0x0100000; //begining of memory
static memoryList * memory;


///////////////////////////////memory acces functions//////////////////////////

void * malloc(size_t space)
{
    page * optPage = getOptimalPage(space);
    
    return optPage->address;
}


/////////////////////////////////memory managment tools///////////////////////////

void newMemory(uint64_t * adrress)
{
    memory = (memoryList *) adrress;
    
    memory->cantPages = 0;
    memory->freePages = 0;
    memory->first = newPage(adrress + sizeof(memoryList),NULL);
}

page * newPage(uint64_t * paddress,page * prev)
{
    if(memory->cantPages = MAX_CANT_OF_PAGES)
        {
            //there is no more space
            return NULL;
        }
    (memory->freePages)++;

    page * page = paddress;
    page->address = paddress;
    page->free = 1;
    page->size = SIZE_OF_MEMORY/MAX_CANT_OF_PAGES;
    page->next = NULL;
    page->prev = prev;

    return page;
}

//get the best page to alocate the space.
page * getOptimalPage(size_t space)
{
    int pageCant = cantOfPagesFor(space);
    page * optPage = findPages(pageCant);
    resizePage(optPage);

    return optPage;
}

//find how many pages you need to alocate the space given.
int cantOfPagesFor(size_t space)
{
    
}


//finds the cant of pages if they are continuos one another if not order them so that they are.
page * findPages(int cant)
{

}

//if the page recieved is full leave it as it is. if not separate it in two pages one full and one empty.
void resizePage(page * page)
{

}

//join cant pages into one 
void joinPages(page * firstPage, int cant)
{
    
}
