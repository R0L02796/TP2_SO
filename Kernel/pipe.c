#include "include/pipe.h"
#include "scheduler.h"
#include "process.h"
#include "semaphore.h"
#include "mutex.h"

static int pipeid;
static Pipe pipes[MAX_PIPES];


void initializePipes()
{
  int i;
  for (i = 0; i < MAX_PIPES; i++)
  {
    Pipe * p = &(pipes[i]);
    p->free = 1;
    p->creatorProcess = NULL;
    p->pipeid = 2 + i;
    p->readPosition = 0;
    p->writePosition = 0;
    p->sem = NULL;
    p->mutex = NULL;
    p->bufferDim = 0;
    p->users = 0;

    for (i = 0; i < MAX_BUFFER_DIM; i++)
    {
      p->buffer[i] = 0;
    }
  }
}

int pipe(int fds[2])
{
  int i;
  for(i=0; i < MAX_PIPES; i++)
  {
      if (pipes[i].free == 1)
      {
        pipes[i].creatorProcess = getCurrentProcess();
        fds[0] = addFileDescriptor(pipes[i].creatorProcess, pipeid);
        fds[1] = addFileDescriptor(pipes[i].creatorProcess, pipeid);
        pipes[i].free = 0;
        char name[10];
        char name2[10];
        pipes[i].sem = semCreate(0, decToStr(pipes[i].pipeid, name));
        pipes[i].mutex = newMutex(decToStr(pipes[i].pipeid, name2));
        pipes[i].users = 0;
        return 0;
      }
  }
  return -1;
}

int pipeRead(int pipeid, char * data, int bytes)
{
  Pipe_t p = getPipe(pipeid);
  semWait(p->sem->name);
  mutexLock(p->mutex->name);
  int i;
  for (i = 0; i < bytes && p->bufferDim > 0; i++)
    {
    p->readPosition = p->readPosition % MAX_BUFFER_DIM;
    data[i] = p->buffer[p->readPosition++];
    p->bufferDim--;
  }
  mutexUnlock(p->mutex->name);
  return i;
}

int pipeWrite(int pipeid, char* data, int bytes)
{
  Pipe_t pipe = &pipes[pipeid -2];
  mutexLock(pipe->mutex->name);
  int i;
  for (i = 0; i < bytes; i++)
  {
    pipe->writePosition = pipe->writePosition % MAX_BUFFER_DIM;
    pipe->buffer[pipe->writePosition++] = data[i];
    pipe->bufferDim++;
  }
  if (bytes > 0 && getSem(pipe->sem) == 0)
  {
    semPost(pipe->sem->name);
  }
  mutexUnlock(pipe->mutex->name);
  return i;
}


Pipe * getPipe(int id)
{
  if(id < 2 || id > MAX_PIPES)
    return NULL;
  return &(pipes[id - 2]);
}

void freePipe(int pipeid)
{
  Pipe_t p = &(pipes[pipeid - 2]);
  p->free = 1;
  p->creatorProcess = NULL;
  p->readPosition = 0;
  p->writePosition = 0;
  char name3[10];
  char name4[10];
  deleteSem(decToStr(p->pipeid, name4)); //hacer funcion to string
  p->sem = NULL;
  deleteMutex(decToStr(p->pipeid, name3));
  p->mutex = NULL;
  p->users = 0;
  for (int i = 0; i < MAX_BUFFER_DIM; i++)
  {
    p->buffer[i] = 0;
  }
  p->bufferDim = 0;
}




