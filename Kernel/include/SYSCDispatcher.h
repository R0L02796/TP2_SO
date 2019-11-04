#ifndef SYSCDispatcher_H_
#define SYSCDispatcher_H_

#define READ 0
#define WRITE 1
#define WAIT 2
#define BALL 3
#define RECTANGLE 4
#define BEEP 5
#define CURSOR 6
#define MALLOC 7
#define FREE 8
#define PRINTPAGE 9
#define SET_AND_RUN_PROCESS 10
#define KILL_PROCESS 11
#define RUN_PROCESS 12
#define CHANGE_PRIORITY 13
#define CHANGE_STATE 14
#define GET_PID 15
#define SET_PROCESS 16
#define END_PROCESS 17
#define SEMOPEN 18
#define SEMCLOSE 19
#define SEMWAIT 20
#define SEMPOST 21
#define MUTEXOPEN 22 
#define MUTEXCLOSE 23
#define MUTEXLOCK 24
#define MUTEXUNLOCK 25
#define SET_AND_RUN_PROCESS 26
#define KILL_PROCESS 27
#define RUN_PROCESS 28
#define CHANGE_PRIORITY 29
#define CHANGE_STATE 30
#define GET_PID 31
#define SET_PROCESS 32
#define END_PROCESS 33
#define PRINT_PROCESSES 34



#define GET 0
#define SET 1

#define KEY 0
#define TIME 1
#define SCREENSIZE 2
#define DO 3
#define UNDO 4

#define CHARACTER 0
#define DRAWCHAR 1
#define CLEAR 2
#define STRING 3


#define HOUR 0
#define MINUTE 1
#define SECOND 2

// Handles General Systemcalls
void syscallDispatcher(uint64_t syscall, uint64_t p1, uint64_t p2, uint64_t p3, uint64_t p4, uint64_t p5);

// Handles Read syscalls
void read(uint64_t mode, uint64_t p1, uint64_t p2);

// Handles Write syscalls
void write(uint64_t mode, uint64_t p1, uint64_t p2, uint64_t p3, uint64_t p4);

// Calls Time Driver
void getTime(unsigned int * dest, uint64_t time);

// Call beep functions
char beepon();
void beepoff();

#endif
