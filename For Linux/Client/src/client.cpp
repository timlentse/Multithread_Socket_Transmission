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
#include <sys/stat.h>
#include <fcntl.h>
#include <libgen.h>

#define PACK_SIZE 1024*512 //define pack size = 512KB
#define MAX_FILENAME_LEN  128

struct File_info {

    char filename[MAX_FILENAME_LEN];
    unsigned long filesize;
};

unsigned long Obtain_File_Size(const char *path)
{
    unsigned long Len = 0;
    struct stat statbuff;
    if(stat(path, &statbuff) < 0) {
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

    int server_sockfd;
    struct sockaddr_in server_addr;
    struct hostent *host;
    int portno;
    if((host = gethostbyname(hostname))==NULL)
    {
        fprintf(stderr,"ERROR: Bad IP address errorno= %s \n",strerror(errno));
        exit(1);
    }

    if((portno = atoi(port))<0)
    {
        fprintf(stderr,"ERROR: Wrong Inpout,Check your Port NO!\n");
        exit(1);
    }

    /* create socket */
    if((server_sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        fprintf(stderr,"ERROR: Failed to obtain Socket Descriptor! %s\n",strerror(errno));
        exit(1);
    }

    /* file server address */
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(portno);
    server_addr.sin_addr=*((struct in_addr *)host->h_addr);

    /* ask for connection*/
    if(connect(server_sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)
    {
        fprintf(stderr,"ERROR: Failed to connect to the host! %s\n",strerror(errno));
        exit(1);
    }
    return server_sockfd ;
}

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("Usage: %s [Server_IP] [Recv_PORT](5555) [Filename].\n",argv[0]);
        return 0;
    }

    // set output buffer
    setvbuf(stdout, NULL, _IONBF, 0);
    fflush(stdout);

    char* filePath = argv[3];
    if(access(filePath, F_OK) != 0)
    {
        fprintf(stderr,"ERROR: File not Found! %s\n",strerror(errno));
        exit(1);
    }

    int server_sockfd ,actully_read;
    char buffer[1024] = {'\0'};
    /*establish connection*/
    server_sockfd = establish_connection( argv[1], argv[2]);

    /*recv a confirm message */
    if( (actully_read = read (server_sockfd, buffer, 1024))==-1)
    {
        fprintf(stderr,"ERROR: Fail to read: %s\n",strerror(errno));
        exit(1);
    }
    buffer[actully_read]='\0';
    printf("[Client]: %s Received !: \n",buffer);

    /*time to send file information */
    bzero(buffer,1024);
    struct File_info file_info;
    // obtain the size of file
    unsigned long File_Len = Obtain_File_Size(filePath);
    file_info.filesize = File_Len;
    printf("[Client]: File size = %lu bytes \n", File_Len);


    // Get file base name (without dirname) and send to server
    char* base_name = basename(filePath);
    strcpy(file_info.filename,  base_name);
    printf("[Client]: The base name = %s \n", base_name);
    if (write(server_sockfd, &file_info, sizeof(file_info)) == -1) {
        fprintf(stderr,"ERROR: Fail to send \n");
        exit(1);
    }

    /* send file */
    printf("[Client]: File path = %s \n", filePath);
    FILE* file = fopen(filePath, "rb");
    if(file == NULL) {
        fprintf(stderr,"ERROR:Fail to Open file!\n");
        exit(1);
    }

    char chip[PACK_SIZE] = {'\0'};
    while((File_Len = fread(chip, sizeof(char), PACK_SIZE, file)) > 0)
    {
        printf("[Client]: %lu bytes had Sent...\n", File_Len);
        write(server_sockfd, chip, File_Len);
        bzero(chip,PACK_SIZE);
    }

    /* close the socket descriptor*/
    close(server_sockfd);
    return 0;
}

