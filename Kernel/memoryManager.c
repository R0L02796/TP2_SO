#include "memoryManager.h"
#include "videoDriver.h"

void newMemory();
page * newPage(uint64_t * paddress, page * prev, uint64_t * pointedAddress, size_t size);
void addPage();
page * getOptimalPage(size_t space);
void resizePage(page * p, size_t usedspace);
void joinPages(page * initialPage);
page * findPage(void * address);



static uint64_t * memoryListAddress = (uint64_t *)0x0100000; //begining of memory
static memoryList * memory;


///////////////////////////////memory acces functions//////////////////////////

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
    p->free = 1;
    joinPages(p);
}

/////////////////////////////////memory managment tools///////////////////////////

//creates the page table with one page if theres the need for more they are added later.
void newMemory()
{
    memory = (memoryList *) memoryListAddress;
    
    memory->cantPages = 0;
    memory->freePages = 0;
    page * aux = newPage(memoryListAddress + sizeof(memoryList), NULL, memoryListAddress + sizeof(memoryList) + sizeof(page) * MAX_CANT_OF_PAGES, SIZE_OF_PAGE);
    memory->first = aux;
    memory->last = aux;
}

page * newPage(uint64_t * paddress, page * prev, uint64_t * pointedAddress, size_t size)
{
    if(memory->cantPages == MAX_CANT_OF_PAGES)
        {
            //there is no more space
            return NULL;
        }
    (memory->freePages)++;

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
    memory->cantPages ++;
    memory->freePages ++;
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
                spaceToAlocate -= currentPage->size;
                while(spaceToAlocate > SIZE_OF_PAGE)
                {
                    addPage();
                    spaceToAlocate -= SIZE_OF_PAGE;
                }
                addPage();
                joinPages(currentPage);

            }
            return currentPage;
            
        }
        
        if(currentPage->free && space <= currentPage->size)//the one that its free and has space.
        {
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
    p->next = newPage((uint64_t *)p + sizeof(p), p, p->address + p->size, sizeNewPage);
    p->next->next = aux;

    memory->freePages++;
    memory->cantPages++;
    
}

//join all prevs and next free pages into one.
void joinPages(page * initialPage)
{
    page* currentp = initialPage;
    //next pages.
    while (currentp != NULL)
    {
        if (currentp->next != NULL && currentp->next->free )
        {
            initialPage->next = currentp->next->next;
            initialPage->size += currentp->next->size;
            
        }
        currentp = currentp->next;
    }
    //prev pages.
    currentp = initialPage;
    while (currentp != NULL)
    {
        if (currentp->prev != NULL && currentp->prev->free)
        {
           initialPage->prev = currentp-> prev;
           initialPage->size += currentp->prev->size;
        }
        currentp = currentp->prev;
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

void printPage(uint64_t *address) {
    newLine();
    newLine();
    newLine();

    putStr("IMPRIME");

    newLine();
    newLine();
    newLine();

 /* page * p = findPage(address);
  if (p == NULL) {
    putStr("not a page in the address inserted \n");
    newLine();
    return;
  }
  newLine();
  putStr("content \n");
  putStr((char *)p->address);
  newLine();

  putStr("address \n");
  char buff[10];
  putStr(decToStr((size_t)p->address, buff));
  newLine();

  putStr("size \n");
  char buffer[10];
  putStr(decToStr((size_t)p->size, buffer));
  newLine();

  putStr("free \n");
  if (p->free == 0) {
    putStr("no");
  } else {
    putStr("yes");
  }
  newLine();*/
}