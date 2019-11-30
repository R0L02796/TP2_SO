#include "processModule.h"
#include "SYSCall.h"

long int setAndRunProcess(char* name,int argc,char** argv, int priority, int (*entry)(int, char**))
{
  long int pid = systemCall((uint64_t)SET_AND_RUN_PROCESS, (uint64_t)name,
  (uint64_t)argc, (uint64_t)argv, (uint64_t)priority,(uint64_t)entry,(uint64_t)&pid);
  return pid;
}

void kill(long int pid)
{
  systemCall((uint64_t)KILL_PROCESS, (uint64_t)pid, 0, 0, 0, 0, 0);
}


void runProcess(long int pid)
{
  systemCall((uint64_t)RUN_PROCESS, (uint64_t)pid, 0, 0, 0, 0, 0);
}

void changePriority(long int pid)
{
  systemCall((uint64_t) CHANGE_PRIORITY,(uint64_t)pid,0,0,0,0, 0);
}

void changeState(long int pid)
{
  systemCall((uint64_t) CHANGE_STATE,(uint64_t)pid,0,0,0,0, 0);
}

long int getRunningPid()
{
  long int pid;
  systemCall((uint64_t) GET_PID,(uint64_t) &pid,0,0,0,0, 0);
  return pid;
}

long int setProcess(char* name,int argc,char** argv, int priority, int (*entry)(int, char**))
{
  long int pid;
  systemCall((uint64_t)SET_PROCESS, (uint64_t)name,
  (uint64_t)argc, (uint64_t)argv, (uint64_t)priority,(uint64_t)entry, (uint64_t)&pid);
  return pid;
}

void endProcess(long int pid)
{
  systemCall((uint64_t) END_PROCESS,(uint64_t)pid,0,0,0,0, 0);
}

void printAllProcesses()
{
  systemCall((uint64_t) PRINT_PROCESSES,0,0,0,0,0, 0);
}

void waitPid(long pid)
{
  systemCall((uint64_t) WAIT_PID, (uint64_t)pid, 0, 0, 0, 0, 0);
}

void block(long int pid)
{
  systemCall((uint64_t)BLOCK_PROCESS, (uint64_t)pid, 0, 0, 0, 0, 0);
}

void unblock(long int pid)
{
  systemCall((uint64_t)UNBLOCK_PROCESS, (uint64_t)pid, 0, 0, 0, 0, 0);
}
