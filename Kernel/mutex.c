#include "mutex.h"
#include "process.h"
#include "scheduler.h"
#include "lib.h"
#include "queue.h"


static mutex mutexVec[MAX_MUTEXES];

void mutexInitialize() {
    for (int i=0; i<MAX_MUTEXES; i++)
    {
        mutex_t m = &mutexVec[i]; 
        m->blockedQueue = newQueue(sizeof(Process *));
        m->pidCreator = 0;
        m->value = 0;
        m->free = 1;
    }
  return;
}

int newMutex(char * name)
{
  int pid = getCurrentPid();
  for (int i = 0; i < MAX_MUTEXES; i++) 
  {
		if (mutexVec[i].free == 1) 
      {
			mutexVec[i].free = 0;
			int len = strlen(name);
			memcpy(mutexVec[i].name, name, len+1);
			mutexVec[i].pidLock = 0;
			mutexVec[i].value = 0;
			mutexVec[i].pidCreator = pid;
      return i;
		}
  }
  return -1;
}

int getMutex(char * mutexName) 
{
	for (int i = 0; i < MAX_MUTEXES; i++) {
		if (strcmp(mutexVec[i].name, mutexName) == 0 && mutexVec[i].free == 0) {
			return i;
		}
	}
	return createMutex(mutexName);
}

void deleteMutex(char * name)
{
    int i = getMutex(name);
    mutexVec[i].free = 1;
    mutexVec[i].pidCreator = 0;
    Process * ret;
    while (sizeQ(mutexVec[i].blockedQueue) > 0)
    {
        poll(mutexVec[i].blockedQueue,&ret);
    }
    mutexVec[i].value = 0;
}

void mutexLock(mutex_t mutex)
{
    Process * running = getCurrentProcess();
    if (!_mutexAcquire(&(mutex->value))) 
        return;

    Offer(mutex->blockedQueue, &running);
    removeProcess(running);
    running->state = BLOCKED;
    _interrupt();
}

void mutexUnlock(mutex_t mutex) 
{
  if (sizeQ(mutex->blockedQueue) > 0) 
  {
    Process * proc;
    poll(mutex->blockedQueue, &proc);
    mutex->pidCreator = proc->pid;
    proc->state = READY;
    addProcess(proc);
  } 
  else 
  {
    mutex->pidCreator= 0;
  }
  mutex->value = 0;
}
