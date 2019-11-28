#ifndef pipeModule_h
#define pipeModule_h

void pipe(int fd[2]); 

void dup(int fd1, int fd2, long processPid) ; 

void closeFD(long int pid, int fd) ; 

void writeFd(int fd, char * buf, int length, long pid);

void readFd(int fd, char * buf, int length, long pid);


#endif
