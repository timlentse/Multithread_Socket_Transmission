// Client.cpp : 定义控制台应用程序的入口点。
//
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <winsock2.h>
#include <Windows.h>
#include <fcntl.h>
#include <iostream>
#include <Shlwapi.h>
#include <sys/types.h>
#include <sys/stat.h>

#pragma comment(lib,"ws2_32.lib")
#define MAX_FILENAME_LEN  128
using namespace std;

#define PACK_SIZE 1024*512 //define pack size = 512KB

struct File_info {

    char filename[MAX_FILENAME_LEN];
    unsigned long filesize;
};

unsigned long Obtain_File_Size(const char *path)
{
    unsigned long Len = 0;
    struct _stat statbuff;
    if(_stat(path, &statbuff) < 0) {
        printf("ERROR: Fail to stat file! \n");
        return Len;
        exit(1);
    }
    else {
        Len = statbuff.st_size;
    }
  return Len;
}


int establish_connection(char* hostname ,char* port) {

	 WSADATA wsa;
    //initialize socket
    if(WSAStartup(MAKEWORD(2,2),&wsa)!=0){
        fprintf(stderr,"Fail to initialize Socket!\n");
        exit(1);
    }

    SOCKET server_sockfd;
    struct sockaddr_in server_addr;
    struct hostent *host;
    int portno;
    if((host = gethostbyname(hostname))==NULL)
    {
            fprintf(stderr,"ERROR: Bad IP address \n");
            exit(1);
    }

    if((portno = atoi(port))<0)
    {
            fprintf(stderr,"ERROR: Wrong Input,Check your Port NO!\n");
            exit(1);
    }

    /* creat socket */
    if((server_sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
           fprintf(stderr,"ERROR: Failed to obtain Socket Descriptor!\n");
            exit(1);
    }

    /* file server address */
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(portno);
    server_addr.sin_addr.S_un.S_addr = inet_addr(host->h_name);

    /* ask for connection*/
    if(connect(server_sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)
    {
            fprintf(stderr,"ERROR: Failed to connect to the host! \n");
            exit(1);
    }
    return server_sockfd ;
}

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("Usage: %s [IP] [PORT] [Filename].\n",argv[0]);
        return 0;
    }

        // set output buffer
        setvbuf(stdout, NULL, _IONBF, 0);
        fflush(stdout);

    char* filePath = argv[3];
	 FILE* file= NULL ;
    if(fopen_s(&file,filePath,"rb") != NULL)
    {
        fprintf(stderr,"ERROR: File not Found or fail to open! \n");
        exit(1);
    }
   
    int server_sockfd ,actully_read;
    char buffer[1024] = {'\0'};
    /*establish connection*/
    server_sockfd = establish_connection( argv[1], argv[2]);

        /*recv a confirm message */
        if( (actully_read = recv (server_sockfd, buffer, 1024,0))==-1)
        {
                fprintf(stderr,"ERROR: Fail to read:\n");
                exit(1);
        }
        buffer[actully_read]='\0';
        printf("[Client]: %s Received !: \n",buffer);

    // obtain the size of file
	struct File_info file_info;
    unsigned long File_Len = Obtain_File_Size(filePath);
    printf("[Client]: File size = %lu bytes \n", File_Len);

    // Get file base name (without dirname) and send to server
	char* pSlash = strrchr(filePath,'\\');
    char* base_name = pSlash + 1;
    printf("[Client]: The base name = %s \n", base_name);
    strcpy_s(file_info.filename,  base_name);
    printf("[Client]: The base name = %s \n", base_name);

	 /*time to send file information */
    if (send(server_sockfd,(char*)&file_info, sizeof(file_info),0) == -1) {
        fprintf(stderr,"ERROR: Fail to send \n");
        exit(1);
    }

    /* send file */
    printf("[Client]: File path = %s \n", filePath);
    char chip[PACK_SIZE] = {'\0'};
    while((File_Len = fread_s(chip, sizeof(char), PACK_SIZE, file)) > 0)
    {
        printf("[Client]: %lu bytes had Sent...\n", File_Len);
        send(server_sockfd, chip, File_Len,0);
        memset(chip,0,PACK_SIZE);
    }
	/* we must close the file*/
	fclose(file);
	file=NULL;
    /* close the socket descriptor*/
    closesocket(server_sockfd);
    return 0;
}

