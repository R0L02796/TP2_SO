#ifndef MUTEX_H
#define MUTEX_H
#include "queue.h"

#define MAX_MUTEXES 50


typedef struct {
	int used;
    char name[50];
    int pidLock;
    queue_t blockedQueue;
    int pidCreator;
    int value;
    int free;    
} mutex;

typedef mutex * mutex_t;

#endif