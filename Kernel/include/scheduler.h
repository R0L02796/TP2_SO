#ifndef scheduler_h
#define scheduler_h

#include "process.h"
#include "lib.h"

void _runProcess(uint64_t rsp);
uint64_t _stackCheat(uint64_t stackBase, int (entryFunction)(int, char **), int argc,char **argv, uint64_t stackRet);

void startSchedule(int (*entryPoint)(int, char **));
ProcessSlot * newProcessSlot(Process * process);
void addProcess(Process * process);
void removeProcess(long int pid);
void schedule(uint64_t stackPointer);
void changeProcessState(int pid, processState state);
unsigned long int getCurrentPid();
struct Process * getCurrentProcess();
void stackCheat(Process * process);
void wrapper(int (*entryFunction)(int, char **), int argc, char **argv);
void printProcesses();
char * getStateFromNumber(int state);
void nice(long int pid);
int existProcess(long pid);

#endif
