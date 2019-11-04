#ifndef semaphoreModule_h
#define semaphoreModule_h

#include <stddef.h>
#define MAX_SEM_ID 20

void semCreate(char id[MAX_SEM_ID], int start);
void deleteSem(char id[MAX_SEM_ID]);
void semWait(char id[MAX_SEM_ID]);
void semPost(char id[MAX_SEM_ID]);

#endif
