#include "include/mutex.h"
#include "include/process.h"
#include "include/scheduler.h"
#include "include/lib.h"
#include "include/queue.h"

int _mutexAcquire(int* value);
void _interrupt();
static mutex mutexVec[MAX_MUTEXES];

void mutexInitialize() 
{
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

mutex_t newMutex(char * name)
{
  int pid = getCurrentPid();
  for (int i = 0; i < MAX_MUTEXES; i++) 
  {
		if (mutexVec[i].free == 1) 
      {
			mutexVec[i].free = 0;
			int len = strLen(name);
			memcpy(mutexVec[i].name, name, len+1);
			mutexVec[i].pidLock = 0;
			mutexVec[i].value = 0;
			mutexVec[i].pidCreator = pid;
      return &(mutexVec[i]);
		}
  }
  return NULL;
}

int getMutex(char * mutexName) 
{
	for (int i = 0; i < MAX_MUTEXES; i++) {
		if (strCmp(mutexVec[i].name, mutexName) == 0 && mutexVec[i].free == 0) {
			return i;
		}
	}
	return -1;
}

void deleteMutex(char * name)
{
    int i = getMutex(name);
    if(i == - 1)
      return;
    mutexVec[i].free = 1;
    mutexVec[i].pidCreator = 0;
    Process * ret;
    while (sizeQ(mutexVec[i].blockedQueue) > 0)
    {
        poll(mutexVec[i].blockedQueue,&ret);
    }
    mutexVec[i].value = 0;
}

void mutexLock(char * name)
{
  mutex_t mutex = &(mutexVec[getMutex(name)]);
  Process * running = getCurrentProcess();
  if (!_mutexAcquire(&(mutex->value))) 
      return;

  offer(mutex->blockedQueue, &running);
  removeProcess(running->pid);
  running->state = BLOCKED;
  _interrupt();
}

void mutexUnlock(char * name) 
{
  mutex_t mutex = &(mutexVec[getMutex(name)]);
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
