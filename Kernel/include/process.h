#ifndef process_h
#define process_h

#include "stddef.h"
#include "stdlib.h"
#include "lib.h"


#define PROCESS_MEMORY 4096
#define MAX_FD 30
#define LOWP 1
#define HIGHP 6
typedef struct ProcessSlot
{
  Process * process;
  struct ProcessSlot * next;
} ProcessSlot;


typedef enum processState {RUNNING, READY, BLOCKED, DEAD} processState;
typedef struct Process
{
  int argc;
  char * name;
  char **argv;
  long int pid;
  processState state;
  int priority;
  uint64_t stackBase;
  uint64_t stackTop;
  uint64_t rsp;
  int (*entryFunction) (int, char **);
  int foreground;
  int fileDescriptors[MAX_FD];
} Process;


Process * createProcess(char * name,int argc, char** argv,int priority, int (*entryFunction) (int, char **));
void startProcesses();
void freeProcess(Process * process);
int addFileDescriptor(Process* process, int fileDescriptor);

#endif
