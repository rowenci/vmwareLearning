#include <stdio.h>
#include <unistd.h>
#include "../lock_set.c"

int main(int argc, char* argv[])
{
    int fd = open("locktest", O_RDWR | O_CREAT, 0644);
    lockSet(fd, F_WRLCK);
    getchar();
    lockSet(fd, F_UNLCK);
    getchar();
    lockSet(fd, F_RDLCK);
    getchar();
    lockSet(fd, F_UNLCK);
    getchar();
    close(fd);
    return 0;
}