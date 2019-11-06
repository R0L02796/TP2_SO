#include "shell.h"
#include "stdlib.h"
#include <stdint.h>
#include "timeModule.h"
#include "videoModule.h"
#include "pongModule.h"
#include "memoryModule.h"
#include "soundModule.h"
#include "snakeModule.h"

int on = 1;
int i = 0;
void initShell(){

  printf("\n~~WELCOME TO LENIA'S SHELL~~\n\nPlease type 'help' to find out about our commands\n\n\n");

  char command[MAXLEN];
  while (on){
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
    }
  }
   printf("\n\n End of program");
}

int getCommand(char * command) {
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

  return INVCOM;
}

void help() {
  printf("\n\n********  Help Menu  ********\n\n");
  printf("  * clear     :       Clears screen\n");
  printf("  * invopcode :       Executes Invalid OP Code Interruption\n");
  printf("  * zerodiv   :       Executes Zero Division Interruption\n");
  printf("  * exit      :       Exits shell\n");
  printf("  * lenia     :       Beep\n");
  printf("  * time      :       Displays current time\n");
  printf("  * pong      :       Iniciates pong when user presses 'enter' which will run until\n"); 
  printf("                      end of game or until user presses 'backspace' to leave\n");
  printf("  * snake      :      Iniciates snake when user presses 'enter' which will run until\n"); 
  printf("                      end of game or until user presses 'backspace' to leave\n");


  printf("\n  Any other command will be taken as invalid\n\n");

}

void clear() {
  clearScreen();
  printf("\n~~Welcome to Lenia's Shell~~\n\n\n");
}

void time(){
  unsigned int h = getHour();
  unsigned int m = getMinute();
  unsigned int s = getSecond();
  printf("\nLocal Time: %d:%d:%d\n", h, m, s);
}

void pong() {
  startPong();
  clear();
}
void snake() {
  startSnake();
  clear();
}

void zeroDiv() {
  int a = 0;
  a = 2/a;
}

void invOpCode(){
  opCode();
}

void lenia() {

  doBeep();
  wait(20);
  noBeep();
  printf("\n");
}


void exit() {
  on = 0;
}

void invCom() {
  printf("\nInvalid command\n");
}

void memTest() {
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