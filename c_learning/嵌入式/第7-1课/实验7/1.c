#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <syslog.h>
#include <sys/stat.h>

/**
 * 守护进程的子进程信号处理
 * @param sig 
 */

void my_func(int sig) {
    int fd = open("/tmp/daemon.log", O_WRONLY | O_CREAT | O_APPEND, 0600);
    if (fd < 0) {
        syslog(LOG_INFO, "打开文件出错");
        exit(-1);
    }
    char *msg = "If you want to quit, please try SIGINT\n";
    write(fd, msg, strlen(msg));
    close(fd);
}

/**
 * 守护进程信号处理
 * @param sig 
 */
void my_func2(int sig) {
    FILE *fp = fopen("/tmp/daemon.log", "a");
    if (fp == NULL) {
        syslog(LOG_INFO, "打开文件出错");
        exit(-3);
    }
    char *str_log = "请使用SIGINT或者SIGQUIT退出程序\n ";
    fputs(str_log, fp);
    fclose(fp);
}

/**
 * 每隔1s写入数据
 */
void write_log() {
    int times = 1;
    while (1) {

        FILE *fp = fopen("/tmp/daemon.log", "a");
        if (fp == NULL) {
            syslog(LOG_INFO, "打开文件出错");
            exit(-3);
        }
        char str_log[100];
        sprintf(str_log, "Daemon message %d\n", times++);
        fputs(str_log, fp);
        fclose(fp);
        sleep(1);
    }
}


int main() {
    pid_t pid = fork();
    pid_t pid1;
    if (pid < 0) {
        printf("子进程创建失败\n");
        exit(-1);
    }
    if (pid > 0) {
        exit(0);
    }

    if (pid == 0) {
        openlog("test6", LOG_PID, LOG_DAEMON);
        setsid();
        chdir("/");
        umask(0);
        for (int i = 0; i < getdtablesize(); ++i) {
            close(i);
        }
        pid1 = fork();

        if (pid1 < 0) {
            char *err_log = "创建守护进程的子进程失败\n";
            syslog(LOG_INFO, err_log);
            exit(-1);
        }
        /***********守护进程的子进程 start****************/
        if (pid1 == 0) {
            syslog(LOG_INFO, "Child is Running!\n");
            struct sigaction action, action2;

            action.sa_handler = my_func;
            sigemptyset(&action.sa_mask);
            action.sa_flags = 0;


            action2.sa_handler = SIG_DFL;
            sigemptyset(&action2.sa_mask);
            action2.sa_flags = 0;


            sigaction(SIGQUIT, &action, NULL);
            sigaction(SIGINT, &action2, NULL);
            alarm(15);
            pause();
        }
        /************护进程的子进程 end*********************/

        /**********守护进程 start************/

        sigset_t set;
        struct sigaction action1, action2, action3;
        if (sigemptyset(&set) < 0) {
            syslog(LOG_INFO, "初始化信号集合出错\n");
            exit(-2);
        }
        if (sigaddset(&set, SIGINT) < 0) {
            syslog(LOG_INFO, "添加信号出错\n");
            exit(-2);
        }
        if (sigaddset(&set, SIGQUIT) < 0) {
            syslog(LOG_INFO, "添加信号出错\n");
            exit(-2);
        }
        if (sigaddset(&set, SIGTSTP) < 0) {
            syslog(LOG_INFO, "添加信号出错\n");
            exit(-2);
        }

        if (sigismember(&set, SIGINT)) {
            action1.sa_handler = SIG_DFL;
            sigemptyset(&action1.sa_mask);
            action1.sa_flags = 0;
            sigaction(SIGINT, &action1, NULL);
        }

        if (sigismember(&set, SIGQUIT)) {
            action2.sa_handler = SIG_DFL;
            sigemptyset(&action2.sa_mask);
            action2.sa_flags = 0;
            sigaction(SIGQUIT, &action2, NULL);
        }

        if (sigismember(&set, SIGTSTP)) {
            action3.sa_handler = my_func2;
            sigemptyset(&action3.sa_mask);
            action3.sa_flags = 0;
            sigaction(SIGTSTP, &action3, NULL);
        }


        if (sigprocmask(SIG_BLOCK, &set, NULL) < 0) {
            syslog(LOG_INFO, "设置阻塞失败\n");
            exit(-4);
        } else {
            waitpid(pid1, NULL, 0);
        }
        if (sigprocmask(SIG_UNBLOCK, &set, NULL) < 0) {
            syslog(LOG_INFO, "设置非阻塞失败\n");
        }
        write_log();
        /***********守护进程 end**********/

    }


}