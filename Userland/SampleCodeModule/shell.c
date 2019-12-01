#include "shell.h"
#include "include/stdlib.h"
#include <stdint.h>
#include "timeModule.h"
#include "videoModule.h"
#include "pongModule.h"
#include "memoryModule.h"
#include "soundModule.h"
#include "processModule.h"
#include "snakeModule.h"
#include "pipeModule.h"

int piddummy;
int flag = 0;
int pidkill;
int pidblock;
int pidunblock;
int pidnice;
int on = 1;
int i = 0;
void initShell(){

  printf("\n~~WELCOME TO LENIA'S SHELL~~\n\nPlease type 'help' to find out about our commands\n\n\n");

  char command[MAXLEN];
  while (on)
  {
    printf("$> ");
    clearBuffer(command);
    scanAndPrint(command);
    int com = getCommand(command);

    switch(com) {
      case HELP:
          help();
          break;

      case CLEAR:
          clear();
          break;

      case TIME:
          time();
          break;

      case PONG:
          pong();
          break;

      case ZERODIV:
          zeroDiv();
          break;

      case INVOPCODE:
          invOpCode();
          break;

      case LENIA:
          lenia();
          break;

      case EXIT:
          exit();
          break;

      case SNAKE:
          snake();
          break;

      case MEMTEST:
          memTest();
          break;

      case INVCOM:
          invCom();
          break;

      case MXTEST:
          mxTest();
          break;

      case PRINTPROC:
          ps();
          break;
      case PIPETEST:
          pipeTest();
          break;
      case LOOPS:
          looptest();
          break;
      case KILL:
          kill(pidkill);
          break;
      case BLOCK:
          block(pidblock);
          break;
      case UNBLOCK:
          unblock(pidunblock);
          break;
      case NICE:
          changePriority(pidnice);
          break;
      case DUMMY:
          piddummy = dummy();
          if(flag == 1)
          {
            waitPid(piddummy);
            flag = 0;
          }
          break;
    }
  }
   printf("\n\n End of program");
}

int getCommand(char * command) {
  int len = strLen(command);
  if(command[len-1] == '&' && command[len-2] == ' ')
  {
    flag = 1;
    command[len-2] = 0;
  }
  if (!strCmp("dummy", command)) return DUMMY;
  if (!strCmp("help", command)) return HELP;
  if (!strCmp("clear", command)) return CLEAR;
  if (!strCmp("time", command)) return TIME;
  if (!strCmp("pong", command)) return PONG;
  if (!strCmp("zerodiv", command)) return ZERODIV;
  if (!strCmp("invopcode", command)) return INVOPCODE;
  if (!strCmp("lenia", command)) return LENIA;
  if (!strCmp("exit", command)) return EXIT;
  if (!strCmp("snake", command)) return SNAKE;
  if (!strCmp("memtest", command)) return MEMTEST;
  if (!strCmp("mxtest", command)) return MXTEST;
  if (!strCmp("ps", command)) return PRINTPROC;
  if (!strCmp("pipetest", command)) return PIPETEST;
  if (!strCmp("loops", command)) return LOOPS;
  if (!strnCmp("kill ", command,5))
  {
    pidkill = atoi(command+5);
    if(pidkill == -1)
    {
      return INVCOM;
    }
  return KILL;
  }
  if (!strnCmp("block ", command,6))
  {
    pidblock = atoi(command+6);
    if(pidblock == -1)
    {
      return INVCOM;
    }
  return BLOCK;
  }
  if (!strnCmp("unblock ", command,8))
  {
    pidunblock = atoi(command+8);
    if(pidunblock == -1)
    {
      return INVCOM;
    }
  return UNBLOCK;
  }
  if (!strnCmp("nice ", command,5))
  {
    pidnice = atoi(command+5);
    if(pidnice == -1)
    {
      return INVCOM;
    }
  return NICE;
  }
  return INVCOM;
}

void help() {
  printf("\n\n********  Help Menu  ********\n\n");
  printf("  * clear     :       Clears screen\n");
  printf("  * invopcode :       Executes Invalid OP Code Interruption\n");
  printf("  * zerodiv   :       Executes Zero Division Interruption\n");
  printf("  * exit      :       Exits shell\n");
  printf("  * lenia     :       Beep\n");
  printf("  * memtest   :       Memory Test\n");
  printf("  * mxtest    :       Mutex Test, creates three processes and handles their execution one at a time using mutex\n");
  printf("  * loops     :       Creates 2 background loops to test kill, nice, block and unblock syscalls\n");
  printf("  * kill      :       Recieves process id and kills it\n");
  printf("  * nice      :       Recieves process id changes its priority to HIGH if it was low and LOW if it was high\n");
  printf("  * block     :       Recieves process id and blocks it\n");
  printf("  * unblock   :       Recieves process id and unblocks it\n");
  printf("  * dummy     :       Dummy used to test foreground assignation\n");
  printf("  * time      :       Displays current time\n");
  printf("  * pipetest  :       runs a test of pipes");
  printf("  * pong      :       Iniciates pong when user presses 'enter' which will run until\n");
  printf("                      end of game or until user presses 'backspace' to leave\n");
  printf("  * snake     :       Iniciates snake when user presses 'enter' which will run until\n");
  printf("                      end of game or until user presses 'backspace' to leave\n");

  printf("\n  Any other command will be taken as invalid\n\n");

  printf("FOREGROUND TESTING ---->  USE 'dummy &' to generate FOREGROUND dummy process\n");
  printf("IMPORTANT!!! --> every process generated in shell can be assigned FOREGROUND, but not all of our commands generate processes.\n");
  printf("ONLY TRY WITH DUMMY AND PHYLO\n");


}



void clear() 
{
  clearScreen();
  printf("\n~~Welcome to Lenia's Shell~~\n\n\n");
}

void time()
{
  unsigned int h = getHour();
  unsigned int m = getMinute();
  unsigned int s = getSecond();
  printf("\nLocal Time: %d:%d:%d\n", h, m, s);
}

void pong()
{
  startPong();
  clear();
}
void snake()
{
  startSnake();
  clear();
}

void zeroDiv() 
{
  int a = 0;
  a = 2/a;
}

void invOpCode()
{
  opCode();
}

void lenia() {

  doBeep();
  wait(20);
  noBeep();
  printf("\n");
}


void exit() 
{
  on = 0;
}

void invCom() 
{
  printf("\nInvalid command\n");
}

void memTest()
{
  /*char * mem = malloc(901);
  printf("\n -----MALLOC----\n");

  char copy[900] = "another one bites the dust\n";
  memcpy(mem, copy, sizeof(copy));

  printPage(mem);

  free(mem);
  printf("\n -----FREE----\n");

  printPage(mem);

//----------

  char * mem2 = malloc(26);
  printf("\n -----MALLOC----\n");


  printPage(mem2);
  char copy2[25] = "it works, relax";
  memcpy(mem2, copy2, sizeof(copy2));

  printPage(mem2);

  free(mem2);
  printf("\n -----FREE----\n");*/

//-------------------------

  char * mem3 = malloc(200);
  printf("\n -----MALLOC----\n");
  char copy3[25] = "hasta la vista baby";
  memcpy(mem3, copy3, sizeof(copy3));
  printPage(mem3);

  char * mem4 = malloc(200);
  printf("\n -----MALLOC----\n");
  char copy4[25] = "la vista";
  memcpy(mem4, copy4, sizeof(copy4));
  printPage(mem4);



  char * mem5 = malloc(200);
  printf("\n -----MALLOC----\n");
  char copy5[25] = "a lo lejos ";
  memcpy(mem5, copy5, sizeof(copy5));
  printPage(mem5);
}

int mutexTestProc(int n, char **argv)
{
  mutexLock("lenia");
  printf("\nnumber: %d\n", n);
  for (int i = 0; i < 50000000; i-=-1){}
  mutexUnlock("lenia");
  return n;
}

void mxProcesses()
{
  mutexOpen("lenia");
  int pid1 = setAndRunProcess("process 1", 1, NULL, 1, mutexTestProc);
  int pid2 = setAndRunProcess("process 2", 2, NULL, 1, mutexTestProc);
  int pid3 = setAndRunProcess("process 3", 3, NULL, 1, mutexTestProc);
  waitPid(pid1);
  waitPid(pid2);
  waitPid(pid3);
  mutexClose("lenia");
}

int mxTest(void)
{
  printf("\n");
  int pid = setAndRunProcess("mutex test wrapper", 0, NULL, 1, mxProcesses);
  return pid;
}

void ps()
{
  printAllProcesses();
}

int juanProcess(int n, char **argv)
{
  char buff[50] = {0};
  
  wait(20);
  
  readFd(3, buff, 45, getRunningPid());

  printf("\n%s\n",buff);

  if(strCmp(buff,"arriba arriba juan, hay que ir a la escuela") == 0)
  {
    writeFd(4, "no no mama, no rompas las bolas", 33, getRunningPid());
  }
  else
  {
    writeFd(4, "durmiendo.....", 13, getRunningPid());
  }

  return 0;
}

int mamaProcess(int n, char **argv)
{

  int fd[2];
  pipe(fd);
  long int juanPid = setAndRunProcess("juan", 0, NULL, 6, juanProcess);
  
  long int mamaPid = getRunningPid();
  dup(fd[1], 3, juanPid);
  dup(fd[0], 4, juanPid);
  
  
  writeFd(fd[1], "arriba arriba juan, hay que ir a la escuela", 45, mamaPid);
  
  char buff[50] = {0};
  
  waitPid(juanPid);
  
  readFd(fd[0], buff, 50, mamaPid);
  printf(" %s.\n", buff);
  waitPid(juanPid);
  closeFD(fd[0], mamaPid);
  closeFD(fd[1], mamaPid);
  return 0;
}

void pipeTest()
{
  long int pid = setAndRunProcess("mama", 0, NULL, 6, mamaProcess);
  waitPid(pid);

  return;
}

void loopmas()
{
  while(1)
  {
      for(int i = 0; i<100000000 ; i++){}
      putStr("+");

  }
}

void loopmenos()
{
  while(1)
  {
      for(int i = 0; i<100000000 ; i++){}
      putStr("-");
  }
}


void looptest()
{
  int pidloop1 = setAndRunProcess("loopmas", 0, NULL, 1, loopmas);
  int pidloop2 = setAndRunProcess("loopmenos", 0, NULL, 1, loopmenos);
}

void dummytest()
{
  clearScreen();
  printf("This is a dummy process, it can be BACKGROUND or FOREGROUND, if you cannot type it is because it was generated as a FOREGROUND process\n");
  printf("Wait a few seconds and it will finish executing\n");
  for(int i = 1; i< 1000000000; i++){}
  printf("bye-bye\n");
}

int dummy()
{
  int piddummy = setAndRunProcess("dummytest", 0, NULL, 6, dummytest);
  return piddummy;
}





// void nice(long int pid, int priority);
