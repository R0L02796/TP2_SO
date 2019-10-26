#include "memoryManager.h"
#include "videoDriver.h"

//choose a memory manager from the ones bellow by uncometing the include of the one you choose

//----------------------------------------------FIRST FIT--------------------------------------------------//
/*
void newMemory();
page * newPage(uint64_t * paddress, page * prev, uint64_t * pointedAddress, size_t size);
void addPage();
page * getOptimalPage(size_t space);
void resizePage(page * p, size_t usedspace);
void joinPages(page * initialPage);
page * findPage(void * address);



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
    resizePage(optPage, space);
   
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

    p->free = 1;
    joinPages(p);
}

/////////////////////////////////memory managment tools//////////////////////////////////////////


//creates the page table with one page if theres the need for more they are added later.
void newMemory()
{
    memory = (memoryList *) memoryListAddress;
    
    memory->cantPages = 1;
    memory->freePages = 1;
    page * aux = newPage(memoryListAddress + sizeof(memoryList), NULL, memoryListAddress + sizeof(memoryList) + sizeof(page) * MAX_CANT_OF_PAGES, SIZE_OF_PAGE);
    memory->first = aux;
    memory->last = aux;
}

page * newPage(uint64_t * paddress, page * prev, uint64_t * pointedAddress, size_t size)
{
    
    page * p = (page *) paddress;
    p->address = pointedAddress;
    p->free = 1;
    p->size = size;
    p->next = NULL;
    p->prev = prev;

    return p;
}

//adds page in the end of the list.
void addPage()
{
    if(memory->cantPages == MAX_CANT_OF_PAGES)
        {
            //there is no more space
            return;
        }
    (memory->cantPages) ++;
    (memory->freePages) ++;
    memory->last->next = newPage((uint64_t *) (memory->last) + sizeof(page), memory->last, memory->last->address +  memory->last->size , SIZE_OF_PAGE);
    memory->last = memory->last->next;
}


//get the best page to alocate the space.
page * getOptimalPage(size_t space)
{
    
    int possiblePages = memory->freePages;
    page * currentPage = memory->first;
    int spaceToAlocate = space;

     while (possiblePages)
    {
        if (possiblePages == 1 && currentPage == memory->last)//if its the last one free and possible it should be the optimal
        {
            if(space > currentPage->size)
            {
                currentPage->free = 0;
                (memory->freePages)--;
                spaceToAlocate -= currentPage->size;
                while(spaceToAlocate > SIZE_OF_PAGE)
                {
                    addPage();
                    spaceToAlocate -= SIZE_OF_PAGE;
                }
                addPage();//because its alwais greater than size we need 1 more page.
                joinPages(currentPage);

            }
            currentPage->free = 0;
            (memory->freePages)--;
            return currentPage;
            
        }
        
        if(currentPage->free && space <= currentPage->size)//the one that its free and has space.
        {
            currentPage->free = 0;
            (memory->freePages)--;
            return currentPage;
        }
        else if(currentPage->free)//there isnt enough free space in the page.
        {
            possiblePages--;
            currentPage = currentPage->next;
        }
        else
            currentPage = currentPage->next;
    }

    if(possiblePages == 0)
    {
        addPage();
        currentPage = memory->last;
        currentPage->free = 0;
        (memory->freePages)--;
        spaceToAlocate -= SIZE_OF_PAGE;
        while(spaceToAlocate > SIZE_OF_PAGE)
        {
            addPage();
            spaceToAlocate -= SIZE_OF_PAGE;
        }
        addPage();//because its alwais greater than size we need 1 more page.
        joinPages(currentPage);
        return currentPage;
    }
    return NULL;
}

//if the page recieved is full leave it as it is. if not separate it in two pages one full and one empty.
void resizePage(page * p, size_t usedspace)
{
    if (memory->cantPages == MAX_CANT_OF_PAGES)
    {
        return;
    }
    int sizeNewPage = p->size - usedspace;
    p->size = usedspace;
    page * aux = p->next;
    p->next = newPage((uint64_t *)p + sizeof(page), p, p->address + p->size, sizeNewPage);
    p->next->next = aux;
    if(p == memory->last)
    {
        memory->last = p->next;
    }

    memory->freePages++;
    memory->cantPages++;
    
}

//join all prevs and next free pages into one.
void joinPages(page * initialPage)
{
    page* currentp = initialPage->next;
    //next pages.
    while (currentp != NULL && currentp->free )
    {        
        initialPage->size = initialPage->size + currentp->size;
        (memory->freePages)--;
        (memory->cantPages)--;
        if (currentp == memory->last)
        {
            memory->last = initialPage;
        }
        initialPage->next = currentp->next;
        currentp = currentp->next;
    }
    if(initialPage->next != NULL)
    {
        initialPage->next->prev = initialPage;
    }
    //prev pages.
    currentp = initialPage->prev;
    while (currentp != NULL && currentp->free)
    {
        initialPage->size += currentp->size;
        (memory->freePages)--;
        (memory->cantPages)--;
        if (currentp == memory->first)
        {
            memory->first = initialPage;
        }
        initialPage->prev = currentp->prev;
        currentp = currentp->prev;
    }
    if(initialPage->prev != NULL)
    {
        initialPage->prev->next = initialPage;
    }
   
}

page * findPage(void * address)
{
    page * current = memory->first;
    while (current!=NULL)
    {
        if (current->address == address)
        {
            return current;
        }
        current = current->next;
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
*/
//----------------------------------------------BUDDY SYSTEM--------------------------------------------------//

void newMemory();
page * newPage(uint64_t * paddress, uint64_t * pointedAddress, size_t size, int level);
void addLv();
page * getOptimalPage(size_t space);
void resizePage(page * p, size_t usedspace);
void joinPages(page * initialPage);
page * findPage(void * address);
page * getPage(int lv);
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
        current->next = newPage((uint64_t *)current + sizeof(page), current->address + current->size, (1<<MAX_LEVEL), MAX_LEVEL);
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
           putStr("addlv");
           addLv(memory->minLv);
       }
    if (memory->freePagesLv[optLv-MIN_LEVEL] == 0 && optLv > memory->minLv)
    {
        addLv(optLv + 1);
    }
    
    page * p = getPage(optLv);
    p->free = 0;
    (memory->freePages)--;
    (memory->freePagesLv[p->lv - MIN_LEVEL]) --;
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
    putStr("space its to big to allocate");
    return -1;

}

void addLv(int l)
{
    int level = l;
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
            if(level == MAX_LEVEL)
            {
                putStr("no more space in memory");
                return;
            }
            level++;
            putStr("aumento lv");
            current = memory->lvVec[level-MIN_LEVEL]; 
        }
        current = current->next;

    }

    while (level >= l)
    {
        if(current->lv == MIN_LEVEL)
        {
        putStr("lv already minimum cant be reduced to more than minimal");
        return;
        }
        if(current->lv == l)
        {
            if(l==memory->minLv)
                (memory->minLv) --;
            newLine();
        char buffer14[10];
        putStr("free pages antes rem\t");
        putStr(decToStr(memory->freePagesLv[current->lv-MIN_LEVEL], buffer14));
        newLine();

        remove(current, current->lv);

        newLine();
        char buffer14[10];
        putStr("free pages despues rem\t");
        putStr(decToStr(memory->freePagesLv[current->lv-MIN_LEVEL], buffer14));
        newLine();


        (current->lv)--;

        newLine();
        char buffer14[10];
        putStr("free pages antes add\t");
        putStr(decToStr(memory->freePagesLv[current->lv-MIN_LEVEL], buffer14));
        newLine();

        add(current, current->lv);

        newLine();
        char buffer14[10];
        putStr("free pages despues add\t");
        putStr(decToStr(memory->freePagesLv[current->lv-MIN_LEVEL], buffer14));
        newLine();

        resizePage(current, current->size/2);

        newLine();
        char buffer14[10];
        putStr("free pages despues resize\t");
        putStr(decToStr(memory->freePagesLv[current->lv-MIN_LEVEL], buffer14));
        newLine();
            return;
        }
        remove(current, current->lv);
        (current->lv)--;
        add(current, current->lv);
        resizePage(current, current->size/2);
        level--;
    }
}


void remove(page * p, int lv)
{
    page * currentPage = memory->lvVec[lv-MIN_LEVEL];
    if (currentPage == NULL)
    {
        putStr("the page trying to remove doesnt exist");
        return;
    }
    if(currentPage->address == p->address)
    {
        memory->lvVec[lv-MIN_LEVEL] = currentPage->next;
        memory->freePagesLv[currentPage->lv - MIN_LEVEL]--;
        memory->freePages--;
        memory->cantPages--;
        return;  
    }
    while (currentPage->next!=NULL)
    {
        if(currentPage->next->address == p->address)
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
        memory->freePagesLv[currentPage->lv - MIN_LEVEL]++;
        memory->freePages ++;
        memory->cantPages ++;
        return;
    }
    
    while (currentPage->next!=NULL)
    {
        currentPage = currentPage->next;
    }
    currentPage->next = p;
    memory->freePagesLv[currentPage->lv - MIN_LEVEL]++;
    memory->freePages ++;
    memory->cantPages ++;
    return;
}

page * getPage(int lv)
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
    memory->freePagesLv[p->lv-MIN_LEVEL]++;
    
    
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

    char buffer6[10];
    putStr("Level \t");
    putStr(decToStr(memory->minLv, buffer6)); 
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