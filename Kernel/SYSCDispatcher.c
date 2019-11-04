#include <stdint.h>
#include "keyboardDriver.h"
#include "videoDriver.h"
#include "timeDriver.h"
#include "memoryManager.h"
#include "SYSCDispatcher.h"
#include "process.h"
#include "scheduler.h"
#include "pipe.h"
#include "semaphore.h"
#include "mutex.h"


void syscallDispatcher(uint64_t syscall, uint64_t p1, uint64_t p2, uint64_t p3, uint64_t p4, uint64_t p5) {
	switch(syscall) {
		case READ:
			read(p1, p2, p3);
			break;
		case WRITE:
			write(p1, p2, p3, p4, p5);
			break;
		case WAIT:
			wait( *( (int*) p1) );
			break;
		case BALL:
			drawBall( *((Color*) p1), *((int*)p2), *((int*) p3), *((int*) p4));
			break;
		case RECTANGLE:
			drawRectangle(*((Color*) p1), *((int*) p2), *((int*) p3), *((int*) p4), *((int*) p5));
			break;
		case BEEP:
			switch(p1){
				case DO:
					beepon();
					break;
				case UNDO:
					beepoff();
					break;
			}
			break;
		case CURSOR:
			switch(p1){
				case GET:
					getCursor((int*)p2, (int*)p3);
					break;
				case SET:
					setCursor(*(int*)p2, *(int*)p3);
					break;
			}	
		case MALLOC:
			*((void **) p1) = malloc((size_t )p2); 
			break;
		case FREE:
			free((void *) p1);
			break;
		case PRINTPAGE:
			printPage((void *) p1);
			break;
		case SET_AND_RUN_PROCESS:
			_setAndRunProcess(p1,p2,p3,p4,p5);
			break;
		case KILL_PROCESS:
			_kill(p1);
			break;
		case RUN_PROCESS:
			_runProcess(p1);
			break;
		case CHANGE_PRIORITY:
			_nice(p1,p2);
			break;
		case CHANGE_STATE:
			_changeProcessState(p1,p2);
			break;
		case GET_PID :
			_getCurrentPid();
			break;
		case SET_PROCESS: 
			_setProcess(p1,p2,p3,p4,p5);
			break;
		case END_PROCESS:
			_endProcess(p1);
			break;
		case PRINT_PROCESSES:
			_printProcesses();
			break;
		case SEMOPEN:
			semCreate((char *) p1, (int) p2);
			break;
		case SEMCLOSE:
			deleteSem((char *) p1);
			break;
		case SEMWAIT:
			semWait((char *) p1);
			break;
		case SEMPOST:
			semPost((char *) p1);
			break;
		case MUTEXOPEN:
			*((void **) p2) = newMutex((char *) p1);
			break;
		case MUTEXCLOSE:
			deleteMutex((char *) p1);
			break;
		case MUTEXLOCK:
			mutexLock((char *) p1);
			break;
		case MUTEXUNLOCK:
			mutexUnlock((char *) p1);
			break;
		

	}
}

void read(uint64_t mode, uint64_t p1, uint64_t p2) {
	unsigned int * t = (unsigned int *) p1;
	char * c = (char *) p1;
	switch(mode) {
		case TIME:
			getTime(t, p2);
			break;
		case KEY:
			*c = getKey();
			break;
		case SCREENSIZE:
			getSize((int*) p1, (int*) p2);
			break;
	}
}

void write(uint64_t mode, uint64_t p1, uint64_t p2, uint64_t p3, uint64_t p4) {
	switch(mode) {
		case CHARACTER:
			printChar(*((char*) p1), *((Color*) p2));
			break;
		case DRAWCHAR:
			drawChar(*((char*) p1), *((int*) p3), *((int*) p4), *((Color*) p2));
		case CLEAR:
			clear();
			break;
		case STRING:
			putStr((char*)p1);
			break;
	}
}


void getTime(unsigned int * t, uint64_t time) {
	switch(time) {
		case HOUR:
			*t = getHour();
			break;
		case MINUTE:
			*t = getMinute();
			break;
		case SECOND:
			*t = getSecond();
			break;
	}

}


//no se que hacer con esto
void _runProcess(int pid) 
{
  Process* process = getProcess(pid);
  if (process == NULL) return;
  stackCheat(process);
  addProcess(process);
}

long int _setAndRunProcess(char *name, int (*entry)(int, char **),int argc, char **argv, int priority) 
{
  Process * newProcess = createProcess(name, entry, argc, argv, priority);
  stackCheat(newProcess);
  addProcess(newProcess);
  return newProcess->pid;
}

void _setProcess(char *name, int (*entry)(int, char **),int argc, char **argv, int priority)
{
  Process * newProcess = createProcess(name, entry, argc, argv, priority);
  return newProcess->pid;
}

void _changeProcessState(long int pid, processState state)
{
  changeProcessState(pid,state); //PARA BLOQUEAR Y MATAR PROCESOS, DONDE ESTA DEFINIDO processState???????
}

void _kill(long int pid)
{
  changeProcessState(pid,DEAD);
}

void _endProcess(long int pid)
{
  removeProcess(pid);
}

void _nice(long int pid, int priority)
{ //para cambiar prioridad
  if (pid <= 1) return;
  if (priority == HIGHP || priority == MIDP || priority == LOWP) 
  {
    nice(pid, priority);
  }
}


void _printProcesses()
{
  printProcesses();
}


long int _getCurrentPid()
{
  return getCurrentPid();
}
