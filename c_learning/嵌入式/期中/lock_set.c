int lock_set(int fd, int type)
{
struct flock lock;
lock.l_whence = SEEK_SET;
lock.l_start = 0;
lock.l_len = 0;
lock.l_type = type;    //初始化l_type
lock.l_pid = -1;    //初始化l_pid
/* 判断文件是否可以上锁*/
fcntl(fd, F_GETLK, &lock);
if (lock.l_type != F_UNLCK)
{
/* 判断文件不能上锁的原因*/
if (lock.l_type == F_RDLCK) /* 该文件已有读取锁*/
{
printf("Read lock already set by %d\n", lock.l_pid);
}
else if (lock.l_type == F_WRLCK) /* 该文件已有写入锁*/
{
printf("Write lock already set by %d\n", lock.l_pid);
}
}
/* F_GETLK获取的是文件fd的原始锁定状态，并将这个状态记录在lock结构体中，这个值和初始化的lock.l_type没有关系。因为lock_set()函数的第二个参数要求给文件fd添加type类型的锁，所以这里需要再次给lock.l_type赋值为type，以便后面调用fcntl函数时，可通过F_SETLKW或者F_SETLK将lock.l_type的设定值赋给文件fd，为文件fd加上符合要求的锁*/
lock.l_type = type;
/* 根据不同的type 值进行阻塞式上锁或解锁*/
if ((fcntl(fd, F_SETLKW, &lock)) < 0)
{
printf("Lock failed:type = %d\n", lock.l_type);
return 1;
}
switch(lock.l_type)
{
case F_RDLCK:
{
printf("Read lock set by %d\n", getpid());
}
break;
case F_WRLCK:
{
printf("Write lock set by %d\n", getpid());
}
break;
case F_UNLCK:
{
printf("Release lock by %d\n", getpid());
return 1;
}
break;
default:
break;
}/* end of switch */
return 0;
}
