#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <syslog.h>
#include <sys/stat.h>

int main(){
	pid_t pid, sid, pc, ppid;
	int i, fd, fp;
	char *buf = "No myfifo\n";
	char *fo;
	pid = fork(); /* 第一步*/
	if (pid< 0){
		printf("Error fork\n");
		exit(1);
	}
	else if (pid> 0){
		exit(0); /* 父进程退出*/
	}
	openlog("daemon_syslog", LOG_PID, LOG_DAEMON);
	if((sid = setsid()) < 0){
		syslog(LOG_ERR, "setsid");
		exit(1);
	} /*第二步*/
	if((sid = chdir("/")) < 0){
		syslog(LOG_ERR, "chdir");
		exit(1);
	} /*第三步*/
	umask(0); /*第四步*/
	for(i = 0; i <getdtablesize(); i++) /*第五步*/{
		close(i);
	}
	/*这时创建完守护进程，以下开始正式进入守护进程工作*/
	while(1){
		if((fd = open("/tmp/daemon.log", O_CREAT|O_WRONLY|O_APPEND, 0600)) < 0){
			syslog(LOG_ERR, "open daemon.log error");
			exit(1);
		}
		ppid = fork();
		//子进程
		if(ppid = 0){
			syslog(LOG_INFO, "Child is running !");
			while(1){
				if((fp = open("/tmp/myfifo", O_RDONLY)) < 0){
					write(fd, buf, strlen(buf));
					sleep(1);
				}else{
					syslog(LOG_INFO, "Child is going to exit");
					exit(0);
				}
			}
		}
		//父进程
		if(ppid > 0){
			do{
				pc = wait(NULL);
			}while(pc == ppid);//父进程阻塞等待子进程退出
			syslog(LOG_INFO, "Daemon noticed that child has exited");
			while(1){
				int i = 0;
				while(i >= 0){
					sprintf(fo, "Daemon message %d", i);
					write(fd, fo, strlen(fo));
					sleep(0.3);
				}
			}
		}
	}
	close(fd);
	closelog();
	exit(0);
}
