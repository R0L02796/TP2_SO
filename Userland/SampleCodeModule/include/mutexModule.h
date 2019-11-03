#ifndef mutexModule_h
#define mutexModule_h

#include <stddef.h>
#define MAX_MUTEX_ID 40

void * mutexOpen(char id[MAX_MUTEX_ID]);

void mutexClose(char id[MAX_MUTEX_ID]);

void mutexLock(char id[MAX_MUTEX_ID]);

void mutexUnlock(char id[MAX_MUTEX_ID]);

#endif
