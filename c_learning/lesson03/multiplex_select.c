#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#define MAX_BUFFER_SIZE 1024
#define IN_FILES 3
#define TIME_DELAY 60
#define MAX(a, b) ((a > b) ? a : b)

int main(int argc, char* argv[])
{
    int fds[IN_FILES];
    char buffer[MAX_BUFFER_SIZE];
    int res, readLen, maxFD;
    struct timeval tv;
    fd_set inset, temp_inset;

    fds[0] = 0;
    if((fds[1] = open("in1", O_RDONLY | O_NONBLOCK)) == -1)
        perror("open");
    if((fds[2] = open("in2", O_RDONLY | O_NONBLOCK)) == -1)
        perror("open");

    maxFD = MAX(MAX(fds[0], fds[1]), fds[2]);
    FD_ZERO(&inset);

    for(int i = 0; i < IN_FILES; i++)
        FD_SET(fds[i], &inset);
    tv.tv_sec = TIME_DELAY;
    tv.tv_usec = 0;

    while (FD_ISSET(fds[0], &inset) && FD_ISSET(fds[1], &inset) && FD_ISSET(fds[2], &inset)){
        temp_inset = inset;
        res = select(maxFD + 1, &temp_inset, NULL, NULL, &tv);
        switch(res){
            case -1:{
                printf("select error\n");
                return 1;
                break;
            }
            case 0:{
                printf("time out\n");
                return 1;
                break;
            }
            default:{
                for(int i = 0; i < IN_FILES; i++){
                    if(FD_ISSET(fds[i], &temp_inset)){
                        memset(buffer, 0, MAX_BUFFER_SIZE);
                        readLen = read(fds[i], buffer, MAX_BUFFER_SIZE);
                        if(readLen < 0){
                            if(errno != EAGAIN)
                                return 1;
                        }else if(!readLen){
                            close(fds[i]);
                            FD_CLR(fds[i], &inset);
                        }else{
                            if(i == 0){
                                if(buffer[0] == 'q' || buffer[0] == 'Q')
                                    return 1;
                            }
                            else{
                                buffer[readLen] = '\0';
                                printf("from file in%d(fd = %d) : %s", i, fds[i], buffer);
                            }
                        }
                    }
                }
                break;
            }
        }
    }
    
    return 0;
}