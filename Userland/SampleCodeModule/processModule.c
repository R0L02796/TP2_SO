#include "processModule.h"
#include "SYSCall.h"

long int setAndRunProcess(char* name,int argc,char** argv, int priority, int (*entry)(int, char**))
{
  unsigned long int pid = systemCall((uint64_t)SET_AND_RUN_PROCESS, (uint64_t)name,
                 (uint64_t)argc, (uint64_t)argv, (uint64_t)priority,(uint64_t)entry);
  return pid;
}

void kill(long int pid)
{
  systemCall((uint64_t)KILL_PROCESS, (uint64_t)pid, 0, 0, 0, 0);
}


void runProcess(long int pid)
{
  systemCall((uint64_t)RUN_PROCESS, (uint64_t)pid, 0, 0, 0, 0);
}

void changePriority(long int pid, int priority)
{
  systemCall((uint64_t) CHANGE_PRIORITY,(uint64_t)pid,(uint64_t) priority,0,0,0);
}

void changeState(long int pid)
{
  systemcall((uint64_t) CHANGE_STATE,(uint64_t)pid,0,0,0,0);
}

void getRunningPid()
{
  systemcall((uint64_t) GET_PID,0,0,0,0,0);
}

void setProcess()
{
  systemcall((uint64_t) SET_PROCESS,0,0,0,0,0);
}

void endProcess(long int pid)
{
  systemCall((uint64_t) END_PROCESS,(uint64_t)pid,0,0,0,0);
}

void printAllProcesses()
{
  systemCall((uint64_t) PRINT_PROCESSES,0,0,0,0,0);
}
