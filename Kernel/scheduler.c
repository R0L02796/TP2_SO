

static int quantum;
static ProcessSlot * current;
static int cantProcesses;

void _runProcess(uint64_t rsp);
uint64_t _stackCheat(uint64_t stackBase, int (entryFunction)(int, char *), int argc,char **argv, uint64_t stackRet);



void startSchedule(int (*entryPoint)(int, char **)) {
  quantum = 1;
  current = NULL;
	cantProcesses=0;
  initializeProcesses();
  Process * idle = createProcess("Idle", (entryFnc)idle, 0, NULL, IDLE);
  Process * shell = createProcess("shell", entryPoint, 0, NULL, HIGHP);
  if (shell == NULL) {
    // throw error
    return;
  }
  if (idle == NULL) {
    // throw error
    return;
  }
  stackCheat(shell);
  stackCheat(idle);
  addProcess(shell);
  addProcess(idle);
  current = shell;
  _runProcess(current->rsp);
}

ProcessSlot * newProcessSlot(Process * process) {
	ProcessSlot * newProcessSlot = malloc(sizeof(ProcessSlot));//revisar allocblock
	newProcessSlot->process = process;
	return newProcessSlot;
}

void addProcess(Process * process) { //adds the process to the last place in the cylic, for example: you have process 1,2,3,4,5 (currently processing the 3rd one) and you want to add the 6th process, then it would be added like this 1,2,6,3,4,5
	ProcessSlot * newProcess = newProcessSlot(process);

	if (current == NULL) {
		current = newProcess;
		current->next = current;
	}
	else {
		ProcessSlot * prevToCurrent = current;
		while (prevToCurrent->next != current)
			prevToCurrent = prevToCurrent->next;
		prevToCurrent->next = newProcess;
		newProcess->next = current;
	}
  cantProcesses++;
}

int eqProcess(Process * proc1, Process * proc2) {
	return proc1->pid == proc2->pid;
}

ProcessSlot * searchPid(ProcessSlot p, int pid){
  int i = 0;
  while(p->next->process->pid != pid && i < cantProcesses){
    p = p->next;
    i++;
  }
  if(i == cantProcesses){
    return NULL;
  }
  else
    return p;
}

void removeProcess(int pid) {
	if (pid == 0 || pid == 1 || cantProcesses < 3) {
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

	if (i == cantProcesses) {
		return;
	}

	if (eqProcess(slotToRemove->process, current->process)) {
		changeProcessState(slotToRemove->process->pid, DEAD);
		// _yield(); FIJARSE ACAAAAAAAAAAAAAA
	}
	else {
		prevSlot->next = slotToRemove->next;
		cantProcesses--;
		// deallocBlock(slotToRemove->process);
		// deallocBlock(slotToRemove); FIJARSE ESTOOOOOOOOOOOOOOOOOOOO ACAAAAAAAAAAAAAA
	}
}

ProcessSlot* findProcessReadyRec(ProcessSlot * current){ //STATIC
  if(current->process->state == READY){
    if(current->process->pid == -1){     //maintenance process
      if(cantProcesses<2){
          return current;
      }
    }
    else {
      return current;
    }
  }
  if(current->process->state == DEAD){
    ProcessSlot * aux = current;
    terminateProcess(current);
    return findProcessReadyRec(aux->next);
  }
  return findProcessReadyRec(current->next);
}

void schedule() {
	if(quantum > 0 && current->process->state == RUNNING){
    quantum--;
    return;
  }
  if(current->process->state != DEAD){
  changeProcessState(current->process->pid, READY);
  }
  current = current->next;
  current = findProcessReadyRec(current);//gives next process that is ready for execution
  changeProcessState(current->process->pid, RUNNING);
  quantum = current->process->priority;
  _runProcess(current->process->rsp);
}

// Process * getProcessById(int pid) {
// 	int i;
// 	ProcessSlot * slot = current;
//
// 	for (i=0; i < cantProcesses; i++) {
// 		if (slot->process->pid == pid)
// 			return slot->process;
// 		slot = slot->next;
// 	}
//
// 	return NULL;
// }

void changeProcessState(int pid, processState state) { //PARA BLOQUEAR Y MATAR PROCESOS, DONDE ESTA DEFINIDO processState???????
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

int getCurrentPid() {   // DIGANME SI LES SIRVE ESTA FUNCION!!!!!!!!
	return current == NULL ? -1 : current->process->pid;
}

// Process * getCurrentProcess() {   // DIGANME SI LES SIRVE ESTA FUNCION!!!!!!!!
// 	return current->process;
// }

// void * getCurrentEntryPoint() {  // DIGANME SI LES SIRVE ESTA FUNCION!!!!!!!!
// 	void * ret = current->process->entryPoint;
// 	return ret;
// }

void stackCheat(Process * process) {
  process->rsp = _stackCheat(process->stackBase, proccess->entryPoint, proccess->argc, process->argv,(uint64_t)wrapper);
}

void wrapper(int (*entryFunction)(int, char **), int argc, char **argv) {
  entryFunction(argc, argv);
  // _cli();
  current->process->state = DEAD
  _interrupt();
}

void terminateProcess(ProcessSlot * slot) {
  removeProcess(slot->process->pid);
  freeProcess(slot);
}
