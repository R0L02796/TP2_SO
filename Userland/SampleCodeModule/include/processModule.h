#ifndef processModule_h
#define processModule_h



long int setAndRunProcess(char* name, int (*entry)(int, char**), int argc,
                                char** argv, int priority);
void kill(long int pid);

void runProcess(long int pid);

void changePriority(long int pid, int priority);

void changeState(long int pid);

void getRunningPid();

void setProcess();

void endProcess(long int pid);

void printAllProcesses();


#endif
