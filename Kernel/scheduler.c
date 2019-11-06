#include "include/scheduler.h"
#include "include/process.h"
#include "interruptions.h"
#include "include/videoDriver.h"
#include "include/memoryManager.h"



static int quantum;
static ProcessSlot * current;
static int cantProcesses;

typedef int (*entryIdle)();
void _runProcess(uint64_t rsp);
static ProcessSlot* findProcessReadyRec(ProcessSlot * current);


void startSchedule(int (*entryPoint)(int, char **))
{
  quantum = 1;
  current = NULL;
	cantProcesses=0;
  startProcesses();
  // Process * idle = createProcess("idle", 0, NULL,LOWP, (entryIdle)idle);
  Process * shell = createProcess("shell",0, NULL, HIGHP,entryPoint);
  if (shell == NULL)
   {
    return;
  }
  // if (idle == NULL)
  // {
  //   return;
  // }
  stackCheat(shell);
  // stackCheat(idle);
  addProcess(shell);
  //_cli();
  // addProcess(idle);
  current->process = shell;
  //_sti();
  _runProcess(current->process->rsp);

}

ProcessSlot * newProcessSlot(Process * process)
{
	ProcessSlot * newProcessSlot = malloc(sizeof(ProcessSlot));
	newProcessSlot->process = process;
	return newProcessSlot;
}

void addProcess(Process * process)
 {
	ProcessSlot * newProcess = newProcessSlot(process);

	if (current == NULL) {
		current = newProcess;
		current->next = current;
	}
	else
  {
		ProcessSlot * prevToCurrent = current;
		while (prevToCurrent->next != current)
			prevToCurrent = prevToCurrent->next;
		prevToCurrent->next = newProcess;
		newProcess->next = current;
	}
  cantProcesses++;
}

void removeProcess(long int pid)
{
	if (pid == 0 || pid == 1 || cantProcesses < 3)
  	{
		return;
	}
	ProcessSlot * prevSlot = current;
	ProcessSlot * slotToRemove = current->next;
	int i = 0;
	while (slotToRemove->process->pid != pid && i < cantProcesses) {
		prevSlot = slotToRemove;
		slotToRemove = slotToRemove->next;
		i++;
	}
	if (i == cantProcesses)
  	{
		return;
	}
	else
	{
			prevSlot->next = slotToRemove->next;
			cantProcesses--;
		freeProcess(slotToRemove);
	}
	return;
}

static ProcessSlot* findProcessReadyRec(ProcessSlot * current)
{ //STATIC
  if(current->process->state == READY)
  {
    if(current->process->pid == 0) // idle process
	{
      if(cantProcesses<2)
	  {
          return current;
      }
    }
    else
	{
      return current;
    }
  }
  if(current->process->state == DEAD)
  {
    ProcessSlot * aux = current;
    removeProcess(current->process->pid);
    return findProcessReadyRec(aux->next);
  }
  return findProcessReadyRec(current->next);
}

void schedule(uint64_t stackPointer)
{
	current->process->rsp = stackPointer;
	if(quantum > 0 && current->process->state == RUNNING)
	{
		quantum--;
		return;
  	}
	if(current->process->state != DEAD){
		changeProcessState(current->process->pid, READY);
	}
	current = current->next;
	current = findProcessReadyRec(current);//gives next process that is ready for execution
 	quantum = current->process->priority;
	_runProcess(current->process->rsp);

}


void changeProcessState(int pid, processState state)
{
	int i;
 	ProcessSlot * slot = current;

	for (i = 0; i < cantProcesses; i++) {
		if (slot->process->pid == pid) {
			slot->process->state = state;
			return;
		}

		slot = slot->next;
	}

	return;
}

unsigned long int getCurrentPid()
{
	return current == NULL ? -1 : current->process->pid;
}

Process * getCurrentProcess()
{
	return current->process;
}

void stackCheat(Process * process)
{
  process->rsp = _stackCheat(process->stackBase, process->entryFunction, process->argc, process->argv,(uint64_t)wrapper);
}

void wrapper(int (*entryFunction)(int, char **), int argc, char **argv)
{
  entryFunction(argc, argv);
  // _cli();
  current->process->state = DEAD;
  _interrupt();
}

void printProcesses()
{
	int i;
	ProcessSlot * s = current;
	newLine();
	for(i = 0; i < cantProcesses; i++)
  	{
		Process * p = s->process;
		putStr("  |  Name: ");
		putStr(p->name);
		putStr("PID: ");
    char buffer[10];
		putStr(decToStr(p->pid,buffer));
    //if(p->foreground)
    //{
    //  putStr("Foreground process");
    //}
    //else
    //{
    // putStr("Background process");
    //}
    putStr("  | Priority: ");
    char buffer1[10];
    putStr(decToStr(p->priority, buffer1));
		putStr("  |  State: ");
		putStr(getStateFromNumber(p->state));
		putStr("  |  RSP: 0x");
    char buffer2[20];
		putStr(decToStr((int)p->rsp, buffer2));
		putStr("  |  Stack Base: 0x");
		char buffer3[20];
		putStr(decToStr((int)p->stackBase, buffer3));
		putStr("  |  Stack Top: 0x");
    char buffer4[10];
		putStr(decToStr((int)p->stackTop, buffer4));
    newLine();
		s = s->next;
	}
}

char * getStateFromNumber(int state)
{
	char * s;
	switch(state) {
			case 0: s="RUNNING";
					break;
			case 1:s="READY";
					break;
			case 2:s= "BLOCKED";
					break;
			case 3: s="DEAD";
					break;
			default: s="other";
		}
		return s;
}

static void idle()
{
  while(1)
  {
    _hlt();
  }
}
