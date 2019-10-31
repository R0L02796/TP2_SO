typedef struct ProcessSlot
 {
  Process * process;
  struct ProcessSlot * next;
} ProcessSlot;

static void addToProcessList(Process * process);


static unsigned long int pid
static ProcessSlot * processList;

Process * createProcess(char * name,int argc, char** argv,int priority, int (*entryFunction) (int, char **))
{
    Process * process = malloc(sizeof(process));
    process->name = name;
    process->pid = pid++;
    process->entryFunction = entryFunction;
    process->argc = argc;
    process->argv = argv;
    process->stackBase = (uint64_t) malloc(PROCESS_MEMORY) + PROCESS_MEMORY - 1;
    process->stackTop = process->stackBase - PROCESS_MEMORY + 1;
    process->rsp = process->stackBase;
    process->priority = priority;
    process->state = READY;
    addToProcessList(process);
    return process;
}

static void addToProcessList(Process * process)
{
  ProcessSlot * aux = malloc(sizeof(ProcessSlot));
  aux->process = process;
  aux->next = processList;
  list = aux;
}

Process * getFromProcessList(unsigned long int pid)
{
  ProcessSlot * aux = processList;
  while (aux != NULL) {
    if (aux->process->pid == pid) {
      return aux->process;
    }
    aux = aux->next;
  }
  return NULL;
}

static ProcessSlot * removeFromProcessList(ProcessSlot * node, Process * process)
 {
  if (node == NULL) return NULL;
  if (node->process == process) {
    ProcessSlot * aux = node->next;
    free(node);
    return aux;
  }
  node->next = removeFromProcessList(node->next, process);
  return node;
}

void initializeProcesses()
 {
  pid = 0;
  processList = NULL;
}

void freeProcess(Process * process)
{
  if (process == NULL) return;
  // _cli();
  processList = removeFromProcessList(processList, process);
  // _sti();
  free((Process*)process->stackTop);
  // for (int i = 0; i <= process->maxFD; i++) {
  //   closeFD(process, i);
  // }
  free(process);
}

// void getProcessData(Process * process, ProcessData * data)
// {
//   data->name = malloc(strlen(process->name) + 1);
//   memcpy(data->name, process->name, strlen(process->name) + 1);
//   data->memory = process->stackBase - process->stackTop;
//   data->pid = process->pid;
//   if (process->status == BLOCKED) {
//     data->status = "Blocked";
//   } else {
//     data->status = "Ready  ";
//   }
//   if (process->priority == HIGHP) {
//     data->priority = "High   ";
//   } else if (process->priority == LOWP) {
//     data->priority = "Low    ";
//   } else {
//     data->priority = "Minimum";
//   }
// }
