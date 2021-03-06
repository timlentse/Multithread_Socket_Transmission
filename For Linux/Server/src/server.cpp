/**
@description: Socket demo for Multithreading transmission in Linux
@author: timlentse(tinglenxan@gmail.com)
@Day: 2014-09
*/
#include "Method.h"
int main(int argc, char *argv[]) {
    /*clear stdout stream*/
    setvbuf (stdout, NULL, _IONBF, 0);
    fflush  (stdout);
    /* Variable Definition */
    pthread_t a_thread[THREAD_NUM];
    struct Thread_argv arg;
    struct sockaddr_in client_addr;
    int server_sockfd = init_sockfd();
    arg.sockfd = server_sockfd;
    arg.sock_addr = client_addr;

    /*create 5 threads to receive file*/
    for(int i(0); i<THREAD_NUM; ++i)
    {
        if(pthread_create(&a_thread[i], NULL, Thread_Fuc, (void*)&arg) !=0)
        {
            fprintf(stderr,"ERROR: create thread fail ... %s\n", strerror(errno));
            exit(1);
        }
    }
    /*wait for a thread ends*/
    for(int i=0; i<THREAD_NUM; ++i)
    {
        if(pthread_join(a_thread[i] , NULL) != 0)
        {
            fprintf(stderr,"ERROR: the thread id : %d ends failes, %s\n", i, strerror(errno));
            exit(1);
        }
    }
	/*wait for 5 seconds*/
    sleep(5); 
    close(server_sockfd);
    return 0;
}
