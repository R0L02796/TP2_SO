#ifndef systemCall_h
#define systemCall_h
#include <stdint.h>
#include "videoModule.h"

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
#define SEMOPEN 10
#define SEMCLOSE 11
#define SEMWAIT 12
#define SEMPOST 13
#define MUTEXOPEN 14
#define MUTEXCLOSE 15
#define MUTEXLOCK 16
#define MUTEXUNLOCK 17
#define SET_AND_RUN_PROCESS 18
#define KILL_PROCESS 19
#define RUN_PROCESS 20
#define CHANGE_PRIORITY 21
#define CHANGE_STATE 22
#define GET_PID 23
#define SET_PROCESS 24
#define END_PROCESS 25
#define PRINT_PROCESSES 26
#define WAIT_PID 27
#define PIPE 28
#define DUP 29
#define FDCLOSE 30
#define READFD 31
#define WRITEFD 32
#define BLOCK_PROCESS 33
#define UNBLOCK_PROCESS 34


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

#define BUFFER_SIZE 256

// Triggers systemcall
unsigned int systemCall(uint64_t syscall, uint64_t p1, uint64_t p2, uint64_t p3, uint64_t p4, uint64_t p5, uint64_t p6);

#endif
