#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <syslog.h>
#include <sys/types.h>
#include "lock_set.c"

#define MYPIPE "/tmp/mypipe" /* 有名管道文件名*/
#define MACO "/tmp/maco"
#define MAX_BUFFER_SIZE  PIPE_BUF /*PIPI_BUF为管道的缓冲区大小，在limits.h中定义*/

int main(int argc, char * argv[]){
	int fd, fp, pipe;
	char buff[MAX_BUFFER_SIZE];
	int nwrite, real_read_len;
	openlog("write_syslog", LOG_PID, LOG_DAEMON);
	if(argc <= 1){
		printf("没有输入字符\n");
		//打开管道
		if((fd = open(MYPIPE, O_WRONLY)) < 0){
			if((pipe = mkfifo("/tmp/mypipe", 0777)) < 0){
				printf("create mypipe error\n");
				syslog(LOG_ERR, "create mypipe error");
				exit(1);
			}
		}
		//打开maco
		printf("读取maco文件...\n");
		if((fp = open(MACO, O_RDONLY)) < 0){
			printf("open maco error\n");
			syslog(LOG_ERR, "open maco error");
			exit(1);
		}
		//加读锁
		lock_set(fp, F_RDLCK);
		while((real_read_len = read(MACO, buff, sizeof(buff))) > 0){
			write(MYPIPE, buff, real_read_len);
			printf("Write '%s' to FIFO\n", buff);
		}
		lock_set(fp, F_UNLCK);
		close(fd);
		close(fp);
		closelog();
		exit(0);
	}
	/* 以只写阻塞方式打开pipe 管道*/
	fd = open(MYPIPE, O_WRONLY);
	if (fd == -1){
		printf("Open pipe file error\n");
		syslog(LOG_ERR, "open mypipe error");
		exit(1);
	}
	/*向管道中写入字符串*/
	if ((nwrite = write(fd, buff, MAX_BUFFER_SIZE)) > 0){
		printf("Write '%s' to FIFO\n", buff);
	}
	close(fd);
	closelog();
	exit(0);
}
