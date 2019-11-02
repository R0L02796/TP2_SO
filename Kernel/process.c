typedef struct ProcessSlot
 {
  Process * process;
  struct ProcessSlot * next;
} ProcessSlot;

static void addToProcessList(Process * process);
static ProcessSlot * removeFromProcessList(ProcessSlot * node, Process * process);


static long int pid
static ProcessSlot * processList;

Process * createProcess(char * name,int argc, char** argv,int priority, int (*entryFunction) (int, char **), int isForeground)
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
    process->foreground = isForeground;
    process->fileDescriptors[0] = 0;
    process->fileDescriptors[1] = 1;
    for (int i = 2; i < MAX_FD; i++)
    {
      process->fileDescriptors[i] = -1;
    }
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

static ProcessSlot * removeFromProcessList(ProcessSlot * node, Process * process)
 {
  if (node == NULL) return NULL;
  if (node->process->pid == process->pid) {
    ProcessSlot * aux = node->next;
    free(node);
    return aux;
  }
  node->next = removeFromProcessList(node->next, process);
  return node;
}

void startProcesses()
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
  for (int i = 0; i <= MAX_FD; i++)
  {
    closeFD(process, i);
  }
  free(process);
}

int addFileDescriptor(Process* process, int fileDescriptor)
{
  for (int i = 0; i <= MAX_FD; i++)
  {
    if (process->fileDescriptors[i] == -1)
    {
      process->fileDescriptors[i] = fileDescriptor;
      return i;
    }
  }
  return -1;
}

// void closeFileDescriptors(Process* process, int fd)
// {
//   int pipeid = process->fileDescriptors[fd];
//   if (pipeid < 2)
//     {
//       return;
//     }
//
//   process->fileDescriptors[fd] = -1;
//   pipe pipe = pipes[pipeid -2];
//   pipe->users--;
//   if (fd == process->maxFD) setMaxFD(process);
//   if (pipe->users == 0) freePipe(pipe);
// }
