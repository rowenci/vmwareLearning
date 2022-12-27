#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    pid_t sonPID;
    sonPID = fork();
    if(sonPID == -1){
        printf("fork error\n");
        return -1;
    }else if(sonPID == 0){
        // 子进程
        printf("the returned value is %d, my pid is %d in son process\n", sonPID, getpid());
    }else{
        // 父进程
        printf("the returned value is %d, my pid is %d in father process\n", sonPID, getpid());
    }
    sleep(60);
    return 0;
}