#ifndef METHOD_H
#define METHOD_H
/*including header files*/
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
/*define pack size = 512KB*/
#define PACK_SIZE 1024*512 
#define MAX_FILENAME_LEN  128
#define THREAD_NUM 5

struct Thread_argv
{
    int sockfd;
    struct sockaddr_in sock_addr;
};

struct File_info {

    char filename[MAX_FILENAME_LEN];
    unsigned long filesize;
};

void* Thread_Fuc(void* arg);
int init_sockfd(void);
#endif // METHOD_H
