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
void produce(int total_num)
{
	char buff[10];
	int pro_file,size;
	pro_file=open(STORE,O_RDWR|O_CREAT|O_APPEND,0644);
	if(pro_file<0)
	{
		printf("Open file error!---pro\n");
		exit(1);
	}
	sprintf(buff,"%d",(total_num%10));
	lock_set(pro_file,F_WRLCK);
	if((size=write(pro_file,buff,strlen(buff)))<0)
	{
		printf("Producer:write error\n");
		exit(1);
	}
	printf("生产-------%d\n",(total_num%10));
	lock_set(pro_file,F_UNLCK);
	close(pro_file);
}
int main(void)
{
	unsigned short arr[2];
	int sem_id;
	int num=1;
	sem_id=semget(ftok("/",1),2,0666|IPC_CREAT);
	arr[0]=0;
	arr[1]=100;
	init_sem(arr,sem_id);
	while(1)
	{
		int n=semctl(sem_id,1,GETVAL);
		if(n>0)
		{
			printf("资源数量为：%d\t空位数量为：%d\n",100-n,n);
		}
		else
		{
			printf("仓库满了！阻塞中--------\n");
		}
		sem_p(sem_id,1);
		produce(num);
		sem_v(sem_id,0);
		num++;
		sleep(1);
	}
}
