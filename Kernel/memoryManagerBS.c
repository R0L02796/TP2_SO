#include "memoryManagerBS.h"
#include "videoDriver.h"

void newMemory();
page * newPage(uint64_t * paddress, uint64_t * pointedAddress, size_t size, int level);
void addLv();
page * getOptimalPage(size_t space);
void resizePage(page * p, size_t usedspace);
void joinPages(page * initialPage);
page * findPage(void * address);
getPage(int lv);
int getOptimalLv(int space);
void decreseLv(page * p);
void add(page * p, int lv);
void remove(page * p, int lv);





static uint64_t * memoryListAddress = (uint64_t *)0x1000000; //begining of memory
static memoryList * memory;


///////////////////////////////memory access functions//////////////////////////

void * malloc(size_t space)
{
    if ( memory != (memoryList *) memoryListAddress ) 
    {
        newMemory();
    }
    page * optPage = getOptimalPage(space);   
    return optPage->address;
}

void free(void * address)
{
    page * p = findPage(address);
    if (p->free)
    {
       return;
    }
    (memory->freePages)++;
    (memory->freePagesLv[p->lv - MIN_LEVEL])++;
    p->free = 1;
}

/////////////////////////////////memory managment tools///////////////////////////

void newMemory()
{
    memory = (memoryList *) memoryListAddress;
    
    memory->minLv = MAX_LEVEL;
    memory->freePages = INIT_CANT_OF_PAGES;
    memory->cantPages = INIT_CANT_OF_PAGES;
    memory->freePagesLv[MAX_LEVEL - MIN_LEVEL] = INIT_CANT_OF_PAGES;
    memory->lvVec[MAX_LEVEL - MIN_LEVEL] = newPage(memoryListAddress + sizeof(memoryList), memoryListAddress + sizeof(memoryList) + sizeof(page) * MAX_CANT_OF_PAGES , (1<<MAX_LEVEL), MAX_LEVEL);
    for(int i = 0; i < (MAX_LEVEL - MIN_LEVEL ); i++)
    {
        memory->lvVec[i] = NULL;
        memory->freePagesLv[i] = 0;
    }
    page * current = memory->lvVec[MAX_LEVEL-MIN_LEVEL];
    for(int i=1; i < INIT_CANT_OF_PAGES; i++)
    {
        current->next = newPage(current + sizeof(page), current->address + current->size, (1<<MAX_LEVEL), MAX_LEVEL);
        current = current->next;
   }


}

page * newPage(uint64_t * paddress, uint64_t * pointedAddress, size_t size, int level)
{
    
    page * p = (page *) paddress;
    p->address = pointedAddress;
    p->free = 1;
    p->size = size;
    p->next = NULL;
    p->lv = level;
    return p;
}

//get the best page to alocate the space.
page * getOptimalPage(size_t space)
{
    int optLv = getOptimalLv(space);
    while (optLv < memory->minLv)
       {
           addLv();
       }
    page * p = getPage(optLv);
    p->free = 0;
    (memory->freePages)--;
    return p;
}


int getOptimalLv(int space)
{
    int currentLvSize;
    for(int k = MIN_LEVEL; k <= MAX_LEVEL; k++)
    {
        currentLvSize = 1 << k;
        if (space <= currentLvSize)
        {
            return k;
        }
        
    }
}

void addLv()
{
    int level = memory->minLv;
    page * current = memory->lvVec[level-MIN_LEVEL];
    if (memory->freePages == 0)
        {
            putStr("No Space in Memory");
            return;//not enough space
        }
    while (current->free == 0)
    {
        if (memory->freePagesLv[level-MIN_LEVEL] == 0)
        {   
            if(level = MAX_LEVEL)
            {
                putStr("no more space in memory");
                return;
            }
            level++;
            current = memory->lvVec[level-MIN_LEVEL]; 
        }
        current = current->next;

    }
    while (level >= memory->minLv)
    {
        decreseLv(current);
    }
}
void decreseLv(page * p)
{
    if(p->lv == MIN_LEVEL)
    {
        putStr("lv already minimum cant be reduced to more than minimal");
        return;
    }
    if(p->lv == memory->minLv)
        (memory->minLv) --;
    remove(p, p->lv);
    (p->lv)--;
    add(p, p->lv);
    resizePage(p, p->size/2);
}

void remove(page * p, int lv)
{
    page * currentPage = memory->lvVec[lv-MIN_LEVEL];
    if (currentPage == NULL)
    {
        putStr("the page trying to remove doesnt exist");
        return;
    }
    
    while (currentPage->next!=NULL)
    {
        if(currentPage->next->address==p->address)
        {
            currentPage->next = currentPage->next->next;
            memory->freePagesLv[currentPage->lv - MIN_LEVEL]--;
            memory->freePages--;
            memory->cantPages--;
            return;
        }
    }
    putStr("the page trying to remove doesnt exist");
    return;
}

void add(page * p, int lv)
{
    page * currentPage = memory->lvVec[lv-MIN_LEVEL];
    if (currentPage == NULL)
    {
        memory->lvVec[lv-MIN_LEVEL] = p;
    }
    
    while (currentPage->next!=NULL)
    {
        currentPage = currentPage->next;
    }
    currentPage->next = p;
    
    return;
}

getPage(int lv)
{
    page * currentPage = memory->lvVec[lv-MIN_LEVEL];
    while (currentPage->free == 0)
    {
        currentPage = currentPage->next;
    }   
    return currentPage;
}

//if the page recieved is full leave it as it is. if not separate it in two pages one full and one empty.
void resizePage(page * p, size_t usedSpace)
{
    if (memory->cantPages == MAX_CANT_OF_PAGES)
    {
        putStr("max quantity of pages reached");
        return;
    }
    int sizeNewPage = p->size - usedSpace;
    p->size = usedSpace;
    page * aux = p->next;
    p->next = newPage((uint64_t *)p + sizeof(page), p->address + p->size, sizeNewPage, p->lv);
    p->next->next = aux;
    memory->freePages++;
    memory->cantPages++;
    (memory->freePagesLv[p->lv-MIN_LEVEL])++;
    
}


page * findPage(void * address)
{
    page * current;
    for (size_t i = 0; i <= MAX_LEVEL-MIN_LEVEL; i++)
    {
        current = memory->lvVec[i];
        while (current!=NULL)
        {
            if (current->address == address)
            {
                return current;
            }
            current = current->next;
        }
        
    }
    return NULL; //address isnt a adrress pointed by a page
}

void printPage(uint64_t *address) 
{
    page * p = findPage(address);
    if (p == NULL) 
    {
        putStr("not a page in the address inserted \n");
        newLine();
        return;
    }
    newLine();
    char buffer[10];
    putStr("cant of pages \t");
    putStr(decToStr(memory->cantPages, buffer));
    newLine();
    char buffer1[10];
    putStr("cant of free pages \t");
    putStr(decToStr(memory->freePages, buffer1)); 
    newLine();

    putStr("content \t");
    putStr((char *)p->address);
    newLine();

    putStr("address \t");
    char buffer2[10];
    putStr(decToStr((size_t)p->address, buffer2));
    newLine();

    putStr("size \t");
    char buffer3[10];
    putStr(decToStr((size_t)p->size, buffer3));
    newLine();

    putStr("free \t");
    if (p->free == 0) 
    {
        putStr("no");
    } 
    else 
    {
        putStr("yes");
    }
    newLine();

}