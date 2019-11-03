#include "include/semaphore.h"
#include "include/process.h"
#include "include/scheduler.h"
#include "include/mutex.h"
#include "lib.h"

queue_t semQueue;
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

sem_t semCreate(int startValue, char* name) {
  for(int i = 0; i<MAX_SEMS; i++){
    if(semVec[i].name == NULL){
      sem_t s = &(semVec[i]);
      s->name = name;
      s->lockedQueue = queueCreate(sizeof(Process *));
      s->mutex = newMutex(name);
      return s;
    }
  }
  return NULL;
}

int findSem(char * name){
  int resp = 0;
  while(strcmp(semVec[resp].name, name) != 0 && resp < MAX_SEMS){
    resp++;
  }
  if(resp < MAX_SEMS)
    return resp;
  return -1;
}

int getSem(sem_t s) {
  return s->value;
}

void deleteSem(sem_t s) {
  mutexDelete(s->mutex->name);//tal vez deleteMutex
  queueFree(s->lockedQueue);
  s->name = NULL;
  s->lockedQueue = NULL;
  s->value = 0;
  s->mutex = NULL;
  return;
}

void semWait(sem_t s) {

  if (s == NULL) {
    return;
  }
  mutexLock(s->mutex);
  Process * running = getCurrenProcess();
  if (s->value == 0) {
    queueOffer(s->lockedQueue, &running);
    mutexUnlock(s->mutex);
    removeProcess(running);
    _interrupt();
  } else {
    s->value--;
    mutexUnlock(s->mutex);
  }
  return;
}

void semPost(sem_t s) {
  if (s == NULL) return;
  mutexLock(s->mutex);
  if (queueSize(s->lockedQueue) != 0) {
    Process * proc;
    queuePoll(s->lockedQueue, &proc);
    addProcess(proc);
  } else {
    s->value++;
  }
  mutexUnlock(s->mutex);
  return;
}
