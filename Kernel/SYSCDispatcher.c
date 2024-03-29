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
#include "lib.h"
#include "interruptions.h"

int syscallDispatcher(uint64_t syscall, uint64_t p1, uint64_t p2, uint64_t p3, uint64_t p4, uint64_t p5, uint64_t p6) {
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
		case SEMOPEN:
			semCreate((int) p2, (char *) p1);
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
		case SET_AND_RUN_PROCESS:
			return callSetAndRunProcess((char *)p1,(int)p2,(char **)p3,(int)p4,(int (*)(int, char **))p5);
			break;
		case KILL_PROCESS:
			changeProcessState((int)p1,DEAD);
			break;
		case RUN_PROCESS:
			callRunProcess((int)p1);
			break;
		case CHANGE_PRIORITY:
			callNice(p1);
			break;
		case CHANGE_STATE:
			changeProcessState((int)p1,(processState)p2);
			break;
		case GET_PID :
			*(long int *) p1 = getCurrentPid();
			break;
		case SET_PROCESS:
			*(long int *) p6 = callSetProcess((char *)p1,(int)p2,(char **)p3,(int)p4,(int (*)(int, char **))p5);
			break;
		case END_PROCESS:
			removeProcess((long) p1);
			break;
		case PRINT_PROCESSES:
			printProcesses();
			break;
		case WAIT_PID:
			waitPid((long)p1);
		case PIPE:
			pipe((int*)p1);
			break;
		case DUP:
			dup((int)p1,(int)p2, (int)p3);
			break;
		case FDCLOSE:
			closeFileDescriptor(getProcess((int)p1),(int)p2);
			break;
		case READFD:
			readFd((int)p1,(char *)p2,(int)p3,(int)p4);
			break;
		case WRITEFD:
			writeFd((int)p1,(char *)p2,(int)p3,(int)p4);
			break;
		case BLOCK_PROCESS:
				changeProcessState((int)p1,BLOCKED);
				break;
		case UNBLOCK_PROCESS:
				changeProcessState((int)p1,READY);
				break;
	}
	return 0;
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


void callRunProcess(int pid)
{
  Process* process = getProcess(pid);
  if (process == NULL) return;
  stackCheat(process);
  addProcess(process);
}

long int callSetAndRunProcess(char *name,int argc, char **argv, int priority,int (*entry)(int, char **))
{
  Process * newProcess = createProcess(name,argc, argv, priority,entry);
  stackCheat(newProcess);
  addProcess(newProcess);
}

long int callSetProcess(char *name,int argc, char **argv, int priority,int (*entry)(int, char **))
{
  Process * newProcess = createProcess(name,argc, argv, priority,entry);
  return newProcess->pid;
}


void callNice(long int pid)
{
nice(pid);
}

void waitPid(long pid)
{
		_sti();
		while(existProcess(pid) == 1){};
		_cli();
}
