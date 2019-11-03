#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "./mutex.h"
#include "./queue.h"
#define MAX_SEM_ID 20
#define MAX_SEMS 50

typedef struct {
  char* name;
  int value;
  queue_t lockedQueue;
  mutex_t mutex;
} tSemaphore;

typedef tSemaphore* sem_t;

// typedef struct SemData {
//   char id[MAX_SEM_ID];
//   sem_t sem;
// }SemData;
//NO SE USA O SI?

extern queue_t semQueue;

void semInitialize();

sem_t semCreate(int startValue, char* name);

int findSem(char * name);

int getSem(sem_t s);

void deleteSem(sem_t s);

void semWait(sem_t s);

void semPost(sem_t s);

#endif
