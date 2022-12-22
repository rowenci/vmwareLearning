#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include"lock_set.c"
#include"sem_com.c"
#define STORE "/tmp/store"
void comsume()
{
	char buff,buff1,buff2;
	int com_file,fd;
	const char* ff="123";
	if((com_file=open(STORE,O_RDWR))<0)
	{
		printf("Open file error!------com\n");
		exit(1);
	}
	lock_set(com_file,F_WRLCK);
	lseek(com_file,SEEK_SET,0);
	printf("消耗的资源内容为:");
	if((read(com_file,&buff,1)==1))
	{
		fputc(buff,stdout);
		fputs("\n",stdout);
	}
	close(com_file);
	if((com_file=open(STORE,O_RDWR))<0)
	{
		printf("Open file error!------com\n");
		exit(1);
	}
	if((fd=open("123",O_CREAT|O_RDWR|O_TRUNC,0666))<0)
	{
		printf("Open buff_file error!\n");
		exit(1);
	}
	lseek(com_file,1,SEEK_SET);
	while((read(com_file,&buff1,1))==1)
	{
		write(fd,&buff1,1);
	}
	close(com_file);
	close(fd);
	if((com_file=open(STORE,O_RDWR))<0)
	{
		printf("Open file error!------com\n");
		exit(1);
	}
	if((fd=open("123",O_RDWR))<0)
	{
		printf("Open buff_file error!\n");
		exit(1);
	}
	lseek(fd,0,SEEK_SET);
	while((read(fd,&buff2,1))==1)
	{
		write(com_file,&buff2,1);
	}
	lock_set(com_file,F_UNLCK);
	close(fd);
	close(com_file);
	unlink(ff);
	
}
int main(void)
{
        unsigned short arr[2];
        int sem_id;
        sem_id=semget(ftok("/",1),2,0666|IPC_CREAT);
        arr[0]=0;
        arr[1]=100;
        init_sem(arr,sem_id);
        while(1)
        {
		int n=semctl(sem_id,0,GETVAL);
		int m=semctl(sem_id,1,GETVAL);
		if(n>0)
		{
			printf("资源数量为:%d\t空位数量为：%d\n",n,m);
		}
		else
		{
			printf("无可消费资源------------阻塞中！\n");
		}
                sem_p(sem_id,0);
                comsume();
                sem_v(sem_id,1);
                sleep(2);
        }
}

