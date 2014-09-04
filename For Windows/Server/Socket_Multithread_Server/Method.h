#ifndef METHOD_H
#define METHOD_H
/*including header files*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <fcntl.h>
#include "..\pthread_libs_for_win32\include\pthread.h"
#include "..\pthread_libs_for_win32\include\semaphore.h"

#pragma comment(lib, "..\\pthread_libs_for_win32\\lib\\pthreadVC2")

#pragma comment(lib,"ws2_32.lib")

#define PACK_SIZE 1024*512 //define pack size = 512KB
#define MAX_FILENAME_LEN  128
#define THREAD_NUM 5   

struct Thread_argv
{
    SOCKET sockfd;
    struct sockaddr_in sock_addr;
};

struct File_info {

    char filename[MAX_FILENAME_LEN];
    unsigned long filesize;
};

void* Thread_Fuc(void* arg);
SOCKET init_sockfd(void);
#endif // METHOD_H
