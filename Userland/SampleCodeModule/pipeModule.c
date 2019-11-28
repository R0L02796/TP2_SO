#include "pipeModule.h"
#include "include/SYSCall.h"

void pipe(int fd[2]) 
{ 
    systemCall((uint64_t)PIPE, (uint64_t)fd, 0, 0, 0, 0, 0); 
}

void dup(int fd1, int fd2, long processPid) 
{
  systemCall((uint64_t)DUP, (uint64_t)fd1, (uint64_t)fd2, (uint64_t)processPid, 0, 0, 0);
}

void closeFD(long int pid, int fd) 
{
  systemCall((uint64_t)FDCLOSE, (uint64_t)pid, (uint64_t)fd, 0, 0, 0, 0);
}

void readFd(int fd, char * buf, int length, long pid)
{
  systemCall((uint64_t)READFD, (uint64_t)fd, (uint64_t)buf, length, pid, 0, 0);
}
void writeFd(int fd, char * buf, int length, long pid)
{
  systemCall((uint64_t)WRITEFD, (uint64_t)fd, (uint64_t)buf, length, pid, 0, 0);
}