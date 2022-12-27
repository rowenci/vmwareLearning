#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATA_LEN 256
#define DELAY_TIME 1

int main(int argc, char* argv[])
{
    pid_t pid;
    int pipeFD[2];
    char buf[MAX_DATA_LEN];
    const char data[] = "pipe test program";
    int realRead, realWrite;
    memset(buf, 0, sizeof(buf));
    if(pipe(pipeFD) < 0){
        printf("pipe create error\n");
        exit(1);
    }
    if((pid = fork()) == 0){
        close(pipeFD[1]);
        printf("hello world1\n");
        if((realRead = read(pipeFD[0], buf, MAX_DATA_LEN)) > 0) // 这里子进程会被阻塞
            printf("%d bytes read from the pipe is '%s'\n", realRead, buf);
        printf("hello world2\n");
        close(pipeFD[0]);
        exit(0);
    }else if(pid > 0){
        close(pipeFD[0]);
        sleep(DELAY_TIME);
        if((realWrite = write(pipeFD[1], data, strlen(data))) != -1)
            printf("parent wrote %d bytes : '%s'\n", realRead, data);
        close(pipeFD[1]);
        waitpid(pid, NULL, 0);
        exit(0);
    }
    return 0;
}