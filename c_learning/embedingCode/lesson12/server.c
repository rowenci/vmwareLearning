#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 4321
#define BUFFER_SIZE 1024
#define MAX_QUE_CONN_NM 5

int main(int argc, char* argv[])
{
    struct sockaddr_in serverSockaddr, clientSockaddr;
    int sinSize, recvbytes;
    int sockFD, clientFD;
    char buf[BUFFER_SIZE];
    if((sockFD = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        perror("socket");
    printf("socket id = %d\n", sockFD);
    
    serverSockaddr.sin_family = AF_INET;
    serverSockaddr.sin_port = htons(PORT);
    serverSockaddr.sin_addr.s_addr = INADDR_ANY;
    memset(&(serverSockaddr.sin_zero), 0, 8);

    int i = 1;
    setsockopt(sockFD, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i));
    if(bind(sockFD, (struct sockaddr *)&serverSockaddr, sizeof(struct sockaddr)) == -1)
        perror("bind");
    printf("bind success\n");
    if(listen(sockFD, MAX_QUE_CONN_NM) == -1)
        perror("listen");
    printf("listening...\n");
    if((clientFD = accept(sockFD, (struct sockaddr *)&clientSockaddr, &sinSize)) == -1)
        perror("accept");
    memset(buf, 0, sizeof(buf));
    if((recvbytes = recv(clientFD, buf, BUFFER_SIZE, 0)) == -1)
        perror("recv");
    printf("received a message : %s\n", buf);
    close(sockFD);

    return 0;
}