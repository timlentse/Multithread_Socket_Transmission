#include "Method.h"
/*the thread function*/
void* Thread_Fuc(void* arg) {
    while(1)
    {
        /* server now waiting for connection  */
        int sin_size = 0;
        int new_sockfd = 0;
        struct Thread_argv t_argv = *(struct Thread_argv*)arg;
        printf("[Server]: Ready!\n");
        sin_size = sizeof(struct sockaddr_in);
        if( ( new_sockfd = accept(t_argv.sockfd,(struct sockaddr *)(&t_argv.sock_addr),(socklen_t*)&sin_size ) ) == -1) {
            fprintf(stderr,"ERROR: Fail to accept! %s\n",strerror(errno));
            exit(1);
        }

        printf("[Server]: Server get connection from %s\n",inet_ntoa(t_argv.sock_addr.sin_addr));

        char message[]="Connection established!";
        if( write(new_sockfd, message, strlen(message))==-1) {
            fprintf(stderr,"ERROR: Write Error:%s\n",strerror(errno));
            exit(1);
        }

        // read file information
        printf("[Server]: reading file info...!\n");
        struct File_info file_info;
        int info_read = 0;

        /*here I have to use the exactly size(136 bytes) of file_info as an argument
         * if i use sizeof(file_info) as the argument, it results in errors and likely tends
         *  to lose some message ,that is what I don't konw  */
        if((info_read = read(new_sockfd, &file_info, 136)))
        {
            /*extract file info*/
            printf("[Server]: filename: %s file_size: %lu\n", file_info.filename,file_info.filesize );

            if((strlen(file_info.filename) == 0) || (file_info.filesize == 0))
            {
                fprintf(stderr,"ERROR: Read file info error!\n File_name or file_size is zero!\n");
                close(new_sockfd);
                exit(1);
            }
        }
        else {
            fprintf(stderr,"ERROR: Read file info error!\n");
            close(new_sockfd);
            close(t_argv.sockfd);
            exit(1);
        }

        char* file_name = file_info.filename;
        unsigned long al_read = 0;
        unsigned long file_len  = file_info.filesize;
        // start to write file
        printf("[Server]: Writing to file...!\n");
        FILE* file = fopen(file_name, "wb+");
        if(file == NULL)
        {
            fprintf(stderr,"ERROR:Fail to open file !\n");
            close(new_sockfd);
            exit(1);
        }

        char chip[PACK_SIZE] = {'\0'};
        while(al_read <= file_len) {
            int red_len = read(new_sockfd, chip, PACK_SIZE);
            if (red_len) {
                int wri_len = fwrite(chip, sizeof(char), red_len, file);
                al_read += red_len;

                printf("[Server]: %d bytes written...\n", wri_len);
                printf("[Server]: totally read %lu bytes...\n", al_read);
            }
            else {
                printf("[Server]: Finished!...%d\n", red_len);
                break;
            }
        }

        printf("[Server]: File name: %s, File length:%ld ,Already received:%ld\n", file_name, file_len, al_read);

        /*close socket */
        fclose(file);
        close(new_sockfd);
    }

    return (void*)NULL;
}

/*to initialize a socket*/
int init_sockfd(void) {
    /* Variable Definition */
    int server_sockfd;
    struct sockaddr_in server_addr;
    int portno;
    if((portno=atoi("5555"))<0)
    {
        fprintf(stderr,"ERROR: WRONG PORT\n");
        exit(1);
    }

    /* create new socket descriptor */
    if((server_sockfd = socket(AF_INET,SOCK_STREAM,0))==-1) {
        fprintf(stderr,"ERROR: Failed to obtain Socket Descriptor! %s\n",strerror(errno));
        exit(1);
    }

    /* fill socket address*/
    bzero(&server_addr,sizeof(struct sockaddr_in));
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    server_addr.sin_port=htons(portno);

    /* binding*/
    if(bind (server_sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1) {
        fprintf(stderr,"ERROR: Fail to bind! %s\n",strerror(errno));
        exit(1);
    }

    /*listening*/
    if(listen(server_sockfd,5)==-1) {
        fprintf(stderr,"ERROR: Fail to listen! %s\n",strerror(errno));
        exit(1);
    }
    return server_sockfd;
}
