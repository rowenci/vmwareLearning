#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "lock_set.c"

// 读锁
int main(int argc, char* argv[])
{
    int fd;
    fd = open("hello", O_RDWR | O_CREAT, 0644);
    if(fd == -1)
        perror("open");
    lockSet(fd, F_RDLCK);
    getchar();
    lockSet(fd, F_UNLCK);
    getchar();
    close(fd);
    
    return 0;
}