#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char* argv[])
{
    pid_t pc, pr;
    pc = fork();
    if(pc < 0) perror("fork");
    if(pc == 0){
        // 子进程
        sleep(5);
        exit(0);
    }else{
        do{
            pr = waitpid(pc, NULL, WNOHANG);    // 非阻塞方式
            // pr = wait(NULL); // 阻塞方式
            if(pr == 0){
                printf("the child process has not exited, waitpid return value is %d\n", pr);
                sleep(1);
            }
        }while(pr == 0);
        if(pr == pc) printf("get child exited, process id is %d\n", pr);
        else printf("error occured\n");
    }
    return 0;
}