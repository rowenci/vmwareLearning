#include <stdio.h>
#include <unistd.h>
int main(int argc, char* argv[])
{
    printf("the pid of this process is %d\n", getpid()); // pid
    printf("the ppid of this process is %d\n", getppid()); // gdb
    getchar();
    return 0;
}