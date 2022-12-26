#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <syslog.h>

int main(int argc, char* argv[])
{
    pid_t pid, sid;
    int fd;
    char *buf = "this is a daemon\n";
    pid = fork(); // 1.create child process
    if(pid < 0){
        printf("error fork\n");
        exit(1);
    }else if(pid > 0){
        exit(0); // parent process exit
    }
    openlog("ademon_syslog", LOG_PID, LOG_DAEMON); // 打开系统日志服务
    if((sid = setsid()) < 0){
        syslog(LOG_ERR, "setsid");
        exit(1);
    }
    if((sid = chdir("/")) < 0){
        syslog(LOG_ERR, "chdir");
        exit(1);
    }
    for(int i = 0; i < getdtablesize(); i++){ // 获取进程能打开的最大文件数
        close(i);
    }
    while (1){
        if((fd = open("/tmp/log/daemon.log", O_WRONLY | O_CREAT | O_APPEND, 0600)) < 0){
            syslog(LOG_ERR, "open");
            exit(1);
        }
        write(fd, buf, strlen(buf));
        close(fd);
        sleep(5);
    }
    closelog();
    return 0;
}