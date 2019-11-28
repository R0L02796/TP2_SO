#ifndef pipeModule_h
#define pipeModule_h

void pipe(int fd[2]); 

void dup(int pid, int fd, int pos); 

void closeFD(int fd, long int pid); 


#endif
