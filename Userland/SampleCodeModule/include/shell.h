#ifndef SHELL_H
#define SHELL_H

#define INVCOM 0
#define HELP   1
#define CLEAR   2
#define TIME   3
#define PONG   4
#define ZERODIV   5
#define INVOPCODE   6
#define ALEPUTO   7
#define LENIA   8
#define SHELLSHOCK   9
#define EXIT   10
#define SNAKE 11
#define MEMTEST 12
#define MXTEST 13
#define PRINTPROC 14
#define PIPETEST 15
#define LOOPS 16
#define KILL 17
#define BLOCK 18
#define UNBLOCK 19
#define NICE 20
#define DUMMY 21
#define PHYLOS 22

#define MAXLEN 256

#define HUNGRY 50
#define THINKING 51
#define EATING 52

// Initializes shell
void initShell();

//Gets command ready to use in a switch function
int getCommand(char * command);

// Executes the 'help' command. Displays help menu
void help();

// Executes the 'clear' command. Clears screen
void clear();

// Executes the 'time' command. Displays local time
void time();

// Executes the 'pong' command. Initializes the pong game
void pong();

// Executes the 'zerodiv' command. Triggers a Zero Division Exception
void zeroDiv();

// Executes the 'invopcode' command. Triggers an Invalid OP Code Exception
void invOpCode();

// Executes the 'lenia' command. Makes beep sound
void lenia();

// Executes the 'exit' command. Exits the kernel
void exit();

// Displays the message for when a command was not recognized
void invCom();

// Executes the 'snake' command. Initializes the snake game
void snake();

// Executes a mutex test with multiple processes
int mxTest();

// Declaration of external function
void opCode();

void memTest();

void ps();

void pipeTest();

void loopmas();

void loopmenos();

void looptest();

void dummytest();

int dummy();

void test(int phnum);

// take up chopsticks
void take_fork(int phnum);

// put down chopsticks
void put_fork(int phnum);

void philo(int n, char **argv);

void philosophers();

//
// void nice(long int pid, int priority);

#endif
