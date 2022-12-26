#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
int main(int argc, char* argv[])
{
    pid_t pid;
    int fd;
    char *buf = "this is a daemon\n";
    pid = fork(); // 1.create child process
    if(pid < 0){
        printf("error fork\n");
        exit(1);
    }else if(pid > 0){
        exit(0); // parent process exit
    }
    setsid(); // 2.修改会话期、进程组、控制终端
    chdir("/"); // 3.修改当前目录
    // umask(0); // 4.重设文件权限掩码
    for(int i = 0; i < getdtablesize(); i++){ // 获取进程能打开的最大文件数
        close(i);
    }
    while (1){
        if((fd = open("/tmp/log/daemon.log", O_WRONLY | O_CREAT | O_APPEND, 0600)) < 0) perror("open");
        write(fd, buf, strlen(buf));
        close(fd);
        sleep(5);
    }
    return 0;
}