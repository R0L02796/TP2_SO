#ifndef MUTEX_H
#define MUTEX_H
#include "queue.h"
#include "stddef.h"

#define MAX_MUTEXES 50
#define MAX_MUTEX_NAME 40

typedef struct {
	int used;
    char name[MAX_MUTEX_NAME];
    int pidLock;
    queue_t blockedQueue;
    int pidCreator;
    int value;
    int free;    
} mutex;

typedef mutex * mutex_t;

void mutexInitialize(); 

mutex_t newMutex(char * name);

void deleteMutex(char * name); 

void mutexLock(char * name);

void mutexUnlock(char * name);
#endif