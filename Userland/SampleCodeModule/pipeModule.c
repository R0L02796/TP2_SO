#include "include/processModule.h"
#include <stdint.h>
#include "include/SYSCall.h"

void pipe(int fd[2]) 
{ 
    systemCall((uint64_t)PIPE, (uint64_t)fd, 0, 0, 0, 0, 0); 
}

void dup(int pid, int fd, int pos) 
{
  systemCall((uint64_t)DUP, (uint64_t)pid, (uint64_t)fd, (uint64_t)pos, 0, 0, 0);
}

void closeFD(int fd, long int pid) 
{
  systemCall((uint64_t)FDCLOSE, (uint64_t)fd, (uint64_t)pid, 0, 0, 0, 0);
}
