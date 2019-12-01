#include "include/pipe.h"
#include "scheduler.h"
#include "process.h"
#include "semaphore.h"
#include "mutex.h"
#include "videoDriver.h"

static int pipeid;
static Pipe pipes[MAX_PIPES];


void initializePipes()
{
  int i;int count = 2;
  for (i = 0; i < MAX_PIPES; i++)
  {
    pipes[i].free = 1;
    pipes[i].creatorProcess = NULL;
    pipes[i].pipeid = count++;
    pipes[i].readPosition = 0;
    pipes[i].writePosition = 0;
    pipes[i].sem = NULL;
    pipes[i].mutex = NULL;
    pipes[i].bufferDim = 0;
    pipes[i].users = 0;

    for (int j = 0; j < MAX_BUFFER_DIM; j++)
    {
      pipes[i].buffer[j] = 0;
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
        char * name = "wombo";  //tendria q ser diferente siempre, PRUEBA
        char * name2 = "combo"; //este lo mismo
        pipes[i].sem = semCreate(0,  name);
        pipes[i].mutex = newMutex(name2);
        pipes[i].users = 0;
        return 0;
      }
  }
  return -1;
}

int pipeRead(int pipeid, char * data, int bytes)
{
  putStr("en read");
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
  putStr("en write");
  Pipe_t pipe = getPipe(pipeid);

  char wad[3];
  putStr(decToStr(pipe->pipeid,wad));
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
      putStr(pipe->sem->name);
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




