#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 4321
#define BUFFER_SIZE 1024

int main(int argc, char* argv[])
{
    int sockFD, sendBytes;
    char buf[BUFFER_SIZE];
    struct hostent *host;
    struct sockaddr_in servAddr;
    if(argc < 3){
        printf("usage : ./client hostname(or ip address) text\n");
        return 1;
    }
    if((host = gethostbyname(argv[1])) == NULL)
        perror("gethostbyname");
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s", argv[2]);
    if((sockFD = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        perror("socket");
    
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);
    servAddr.sin_addr = *((struct in_addr *)host->h_addr);
    memset(&(servAddr.sin_zero), 0, 8);

    if(connect(sockFD, (struct sockaddr *)&servAddr, sizeof(struct sockaddr)) == -1)
        perror("connect");
    if((sendBytes = send(sockFD, buf, strlen(buf), 0)) == -1)
        perror("send");
    
    close(sockFD);
    
    return 0;
}