#include "include/semaphoreModule.h"
#include "include/SYSCall.h"
// test
#include "include/stdlib.h"

void semCreate(char id[MAX_SEM_ID], int start) {
  systemCall((uint64_t)SEMCREATE, (uint64_t)id, (uint64_t)start, 0, 0, 0);
}
void deleteSem(char id[MAX_SEM_ID]) {
  systemCall((uint64_t)DELETESEM, (uint64_t)id, 0, 0, 0, 0);
}
void semWait(char id[MAX_SEM_ID]) {
  systemCall((uint64_t)SEMWAIT, (uint64_t)id, 0, 0, 0, 0);
}
void semPost(char id[MAX_SEM_ID]) {
  systemCall((uint64_t)SEMPOST, (uint64_t)id, 0, 0, 0, 0);
}
