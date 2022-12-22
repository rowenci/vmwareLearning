#include <stdio.h>
#include <fcntl.h>

int lockSet(int fd, int type){
    struct flock lock;
    // 对全文件加锁
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_type = type;
    lock.l_pid = -1;

    // F_GETLK 获取当前的文件锁状态，存放在lock中
    fcntl(fd, F_GETLK, &lock);
    if(lock.l_type != F_UNLCK){
        if(lock.l_type == F_RDLCK)
            printf("read lock already set by %d\n", lock.l_pid);
        if(lock.l_type == F_WRLCK)
            printf("write lock already set by %d\n", lock.l_pid);
    }
    lock.l_type = type;
    if((fcntl(fd, F_SETLKW, &lock)) < 0){
        printf("lock failed : type = %d\n", lock.l_type);
        return 1;
    }
    switch(lock.l_type){
        case F_RDLCK:{
            printf("Read lock set by %d\n", getpid());
            break;
        }
        case F_WRLCK:{
            printf("Write lock set by %d\n", getpid());
            break;
        }
        case F_UNLCK:{
            printf("Release lock by %d\n", getpid());
            return 1;
        }
        default:
            break;
    }
    return 0;
}