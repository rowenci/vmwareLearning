#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <syslog.h>
#include "lock_set.c"

#define MAX_BUFFER_SIZE 1024 /* 缓冲区大小*/
#define IN_FILES 3 /* 多路复用输入文件数目 */
#define TIME_DELAY 50 /* 超时值秒数 */
#define MAX(a, b) ((a > b)?(a):(b))
#define MYFIFO "/tmp/myfifo"
#define MYPIPE "/tmp/mypipe"
#define FIFO "fifo"
#define PIPE "pipe"

int main(){
    int fds[IN_FILES];
    char buf[MAX_BUFFER_SIZE];
    int i, res, real_read, maxfd;
    struct timeval tv;
    fd_set inset,tmp_inset;
    int fd_fifo, fd_pipe;
    //打开系统日志
    openlog("write_syslog", LOG_PID, LOG_DAEMON);
    /*首先以只读非阻塞方式打开两个管道文件*/
    fds[0] = 0;
    if((fds[1] = open (MYFIFO, O_RDONLY|O_NONBLOCK)) < 0){
        printf("Open myfifo error\n");
	syslog(LOG_ERR, "open myfifo error");
        return 1;
    }
    if((fds[2] = open (MYPIPE, O_RDONLY|O_NONBLOCK) ) < 0){
        printf("Open mypipe error\n");
	syslog(LOG_ERR,"open mypipe error");
        return 1;
    }
    /*取出两个文件描述符中的较大者*/
    maxfd = MAX(MAX(fds[0], fds[1]), fds[2]) ;
    /*初始化读集合 inset，并在读集合中加入相应的描述集*/
    FD_ZERO(&inset);
    for (i = 0; i < IN_FILES; i++){
        FD_SET(fds[i], &inset);
    }
    tv.tv_sec = TIME_DELAY;
    tv.tv_usec = 0;
    //打开fifo和pipe文件
    if((fd_fifo = open("fifo", O_RDONLY|O_CREAT, 0644)) < 0){
        printf("open fifo error\n");
	syslog(LOG_ERR, "open fifo error");
	exit(1);
    }
    if((fd_pipe = open("pipe", O_RDONLY|O_CREAT, 0644)) < 0){
	printf("open pipe error\n");
	syslog(LOG_ERR, "open pipe error");
	exit(1);
    }
    /*循环    测试该文件描述符是否准备就绪，并调用 select 函数对相关文件描述符做对应操作*/
    while(FD_ISSET(fds[0],&inset) && FD_ISSET(fds[1],&inset) && FD_ISSET(fds[2], &inset) ){
        /* 文件描述符集合的备份， 这样可以避免每次进行初始化 */
        tmp_inset = inset;
        res = select(maxfd + 1, &tmp_inset, NULL, NULL, &tv); 
        switch(res){
            case -1:{
                printf("Select error\n");
		syslog(LOG_ERR, "select error");
                return 1;
            }
            break;
            case 0: /* Timeout */{
                printf("Time out\n");
                return 1;
            }
            break;
            default:{
                for (i = 0; i < IN_FILES; i++){
                    if (FD_ISSET(fds[i], &tmp_inset) ){
                        memset(buf, 0, MAX_BUFFER_SIZE);
                        real_read = read(fds[i], buf, MAX_BUFFER_SIZE) ;
                        if (real_read < 0){
                            if (errno != EAGAIN){
                                return 1;
                            }
                        }
                        else if (!real_read){
                            close(fds[i]);
                            FD_CLR(fds[i], &inset);
                        }
                        else{
                            if (i == 0){/* 主程序终端控制 */
                                if ((buf[0] == 'q') || (buf[0] == 'Q')){
                                    return 1;
                                }
                            }else if(i == 1){//这时读取myfifo管道
				lock_set(fd_fifo, F_WRLCK);
				write(fd_fifo, buf, real_read);//write to fifo
				lock_set(fd_fifo, F_UNLCK);
			    }else if(i ==2){//读取mypipe管道
				lock_set(fd_pipe, F_WRLCK);
				write(fd_pipe, buf, real_read);
				lock_set(fd_pipe, F_UNLCK);
			    }
                        }
                    } /* end of if */
                } /* end of for */
            }
            break;
        } /* end of switch */
    } /*end of while */
    close(fd_fifo);
    close(fd_pipe);
    closelog();
    return 0;
}
