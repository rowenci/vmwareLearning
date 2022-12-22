#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "lock_set.c"

int main(void){
	int fd;
	fd = open("locktest",O_RDWR|O_CREAT,0644);
	if(fd<0){
		printf("open file error\n");
		exit(1);
	}
	lock_set(fd,F_WRLCK);
	getchar();
	lock_set(fd,F_UNLCK);
	getchar();
	lock_set(fd,F_RDLCK);
	getchar();
	lock_set(fd,F_UNLCK);
	getchar();
	close(fd);
	exit(0);
}
