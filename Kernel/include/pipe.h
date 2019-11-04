#ifndef pipe_h
#define pipe_h

#include "semaphore.h"
#include "mutex.h"
#include "process.h"


#define MAX_PIPES 20
#define MAX_BUFFER_DIM 256


typedef struct Pipe
{
    int free;
    Process * creatorProcess;
    int pipeid;
    int readPosition;
    int writePosition;
    sem_t sem;
    mutex_t mutex;
    char buffer[MAX_BUFFER_DIM];
    int bufferDim;
    int users;
}Pipe;

typedef Pipe* Pipe_t;

Pipe * getPipe(int id);

int pipeRead(int pipeid, char * data, int bytes);

int pipe(int fds[2]);

void initializePipes();

int pipeWrite(int pipeid, char* data, int bytes);

void freePipe(int pipeid);

#endif