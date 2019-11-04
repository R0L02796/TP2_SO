#include "include/semaphore.h"
#include "include/process.h"
#include "include/scheduler.h"
#include "include/mutex.h"
#include "queue.h"
#include "lib.h"

void _interrupt();
static tSemaphore semVec[MAX_SEMS] = {NULL};

void semInitialize() {// Quiero que el vector de semaforos empiece lleno
    for (int i=0; i<MAX_SEMS; i++)
    {
        sem_t s = &(semVec[i]);
        s->name = NULL;
        s->lockedQueue = NULL;
        s->value = 0;
        s->mutex = NULL;
    }
  return;
}

sem_t semCreate(int startValue, char* name) 
{
  for(int i = 0; i<MAX_SEMS; i++){
    if(semVec[i].name == NULL){
      sem_t s = &(semVec[i]);
      s->name = name;
      s->lockedQueue = newQueue(sizeof(Process *));
      s->mutex = newMutex(name);
      return s;
    }
  }
  return NULL;
}

int findSem(char * name){
  int resp = 0;
  while(strCmp(semVec[resp].name, name) != 0 && resp < MAX_SEMS)
  {
    resp++;
  }
  if(resp < MAX_SEMS)
    return resp;
  return -1;
}

int getSem(sem_t s) {
  return s->value;
}

void deleteSem(char * name) {
  sem_t s = &semVec[findSem(name)];
  deleteMutex(s->mutex->name);//tal vez deleteMutex
  freeQ(s->lockedQueue);
  s->name = NULL;
  s->lockedQueue = NULL;
  s->value = 0;
  s->mutex = NULL;
  return;
}

void semWait(char * name) 
{
  sem_t s = &semVec[findSem(name)];
  if (s == NULL) 
  {
    return;
  }
  mutexLock(s->mutex->name);
  Process * running = getCurrentProcess();
  if (s->value == 0) 
  {
    offer(s->lockedQueue, &running);
    mutexUnlock(s->mutex->name);
    removeProcess(running->pid);
    _interrupt();
  } 
  else 
  {
    s->value--;
    mutexUnlock(s->mutex->name);
  }
  return;
}

void semPost(char * name) 
{
  sem_t s = &semVec[findSem(name)];
  if (s == NULL) return;
  mutexLock(s->mutex->name);
  if (sizeQ(s->lockedQueue) != 0) 
  {
    Process * proc;
    poll(s->lockedQueue, &proc);
    addProcess(proc);
  } else {
    s->value++;
  }
  mutexUnlock(s->mutex->name);
  return;
}
