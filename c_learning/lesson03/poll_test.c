#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <poll.h>

#define MAX_BUFFER_SIZE 1024 /* 缓冲区大小 */
#define IN_FILES 3 /* 多路复用输入文件数目 */
#define TIME_DELAY 60 /* 超时时间秒数 */
#define MAX(a, b) ((a > b)?(a):(b))

int main(void){
    struct pollfd fds[IN_FILES];
    char buf[MAX_BUFFER_SIZE];
    int i, res, real_read, maxfd;

    fds[0].fd = 0;
    if((fds[1].fd = open ("in1", O_RDONLY | O_NONBLOCK) ) < 0)
    {
        printf("Open in1 error\n");
        return 1;
    }
    if((fds[2].fd = open ("in2", O_RDONLY | O_NONBLOCK) ) < 0)
    {
        printf("Open in2 error\n");
        return 1;
    }
    /*监听三个文件的读事件*/
    for (i = 0; i < IN_FILES; i++)
    {
        fds[i].events = POLLIN;
    }
    /*判断三个文件的读事件是否有效，循环监听三个文件*/
    while(fds[0].events || fds[1].events || fds[2].events)
    {
        if (poll(fds, IN_FILES, 0)  < 0)
        {
            printf("Poll error\n");
            return 1;
        }
        for (i = 0; i< IN_FILES; i++)
        {
            if (fds[i].revents)
            {
                memset(buf, 0, MAX_BUFFER_SIZE);
                real_read = read(fds[i].fd, buf, MAX_BUFFER_SIZE);
                if (real_read < 0)
                {
                    if (errno != EAGAIN)
                    {
                        return 1;
                    }
                }
                else if (!real_read)
                {
                    close(fds[i].fd);
                    fds[i].events = 0;
                }
                else
                {
                    if (i == 0)
                    {
                        if ((buf[0] == 'q') || (buf[0] == 'Q'))
                        {
                        return 1;
                        }
                    }
                    else
                    {
                        buf[real_read] = '\0';
                        printf("%s", buf);
                    }
                } /* end of if real_read*/
            } /* end of if revents */
        } /* end of for */
    } /*end of while */
    exit(0);
}
