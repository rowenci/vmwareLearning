#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char* argv[])
{
    struct addrinfo hints, *res = NULL;
    int rc;
    char buf[10];
    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_CANONNAME;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    if(argc < 2)
        sprintf(buf, "localhost");
    else
        sprintf(buf, argv[1]);
    rc = getaddrinfo(buf, NULL, &hints, &res);
    if(rc != 0)
        perror("getaddrinfo");
    else
        printf("hostname is %s\n", res->ai_canonname);
    return 0;
}