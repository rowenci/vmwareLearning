#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "lock_set.c"

// 写锁
int main(){
    int fd;
    fd = open("hello", O_RDWR | O_CREAT, 0644);
    if(fd == -1)
        perror("open");
    lockSet(fd, F_WRLCK);
    getchar();
    lockSet(fd, F_UNLCK);
    getchar();
    close(fd);
    return 0;
}