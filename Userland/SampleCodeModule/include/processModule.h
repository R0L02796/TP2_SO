#ifndef processModule_h
#define processModule_h



long int setAndRunProcess(char* name, int argc, char** argv, int priority,int (*entry)(int, char**));

void kill(long int pid);

void runProcess(long int pid);

void changePriority(long int pid);

void changeState(long int pid);

long int getRunningPid();

long int setProcess(char* name,int argc,char** argv, int priority, int (*entry)(int, char**));

void endProcess(long int pid);

void waitPid(long pid);

void printAllProcesses();

void block(long int pid);

void unblock(long int pid);


#endif
