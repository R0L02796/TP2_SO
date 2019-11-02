#ifndef process_h
#define process_h


#define PROCESS_MEMORY 4096
#define MAX_FD 30


typedef enum processState {RUNNING, READY, BLOCKED, DEAD} processState;
typedef struct Process{
  int argc;
  char **argv;
  int pid;
  processState state;
  int priority;
  uint64_t stackBase;
  uint64_t stackTop;
  uint64_t rsp;
  void * entryPoint;
  int fileDescriptors[MAX_FD];
} Process;

Process * createProcess(int argc, char** argv,int priority,void* entryPoint);
void startProcesses();
void freeProcess(Process * process);
int addFileDescriptor(Process* process, int fileDescriptor);

#endif