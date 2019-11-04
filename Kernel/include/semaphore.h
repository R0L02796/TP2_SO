#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "./mutex.h"
#include "./queue.h"
#define MAX_SEM_ID 20
#define MAX_SEMS 50

typedef struct tSemaphore{
  char* name;
  int value;
  queue_t lockedQueue;
  mutex_t mutex;
} tSemaphore;

typedef tSemaphore* sem_t;

extern queue_t semQueue;

void semInitialize();

sem_t semCreate(int startValue, char* name);

int findSem(char * name);

int getSem(sem_t s);

void deleteSem(char * name);

void semWait(char * name);

void semPost(char * name);

#endif
