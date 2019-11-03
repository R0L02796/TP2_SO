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
      if (pipes[i].free = 1)
      {
        pipes[i].creatorProcess = getCurrentProcess();
        fds[0] = addFileDescriptors(pipes[i].creatorProcess, pipeid);
        fds[1] = addFileDescriptors(pipes[i].creatorProcess, pipeid);
        pipes[i].free = 0;
        pipes[i].sem = semOpen(toString(pipes[i].pipeid));
        pipes[i].mutex = newMutex(toString(pipes[i].pipeid));
        return 0;
      }
  }
}

int pipeRead(int pipeid, char * data, int bytes)
  {
    Pipe_t pipe = &pipes[pipeid - 2];
    semWait(pipe->sem);
    mutexLock(pipe->mutex);
    int i;
    for (i = 0; i < bytes && pipe->bufferDim > 0; i++)
     {
      pipe->readPosition = pipe->readPosition % MAX_BUFFER_DIM;
      data[i] = pipe->buffer[pipe->readPosition++];
      pipe->bufferDim--;
    }
    mutexUnlock(pipe->mutex);
    return i;
  }

int pipeWrite(int pipeid, char* data, int bytes)
 {
  Pipe_t pipe = &pipes[pipeid -2];
  mutexLock(pipe->mutex);
  int i;
  for (i = 0; i < bytes; i++)
  {
    pipe->writePosition = pipe->writePosition % MAX_BUFFER_DIM;
    pipe->buffer[pipe->writePosition++] = data[i];
    pipe->bufferDim++;
  }
  if (bytes > 0 && semGetValue(pipe->sem) == 0)
  {
    semPost(pipe->sem);
  }
  mutexUnlock(pipe->mutex);
  return i;
}


void freePipe(int pipeid)
 {
    Pipe_t p = &(pipes[pipeid - 2]);
    p->free = 1;
    p->creatorProcess = NULL;
    p->readPosition = 0;
    p->writePosition = 0;
    deleteSem(toString(p->pipeid)); //hacer funcion to string
    p->sem = NULL;
    deleteMutex(toString(p->pipeid));
    p->mutex = NULL;

    for (int i = 0; i < MAX_BUFFER_DIM; i++)
    {
      p->buffer[i] = 0;
    }
    p->bufferDim = 0;
}




// void closeFileDescriptors(Process* process, int fd)
// {
//   int pipeid = process->fileDescriptors[fd];
//   if (pipeid < 2)
//     {
//       return;
//     }
//
//   process->fileDescriptors[fd] = -1;
//   pipe pipe = pipes[pipeid -2];
//   pipe->users--;
//   if (fd == process->maxFD) setMaxFD(process);
//   if (pipe->users == 0) freePipe(pipe);
// }
