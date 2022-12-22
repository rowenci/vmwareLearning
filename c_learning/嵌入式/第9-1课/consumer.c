#include<sys/types.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include"sem_com.c"
#define BUFFER_SIZE 2048
#define KEY 252674
int main()
{
	int sem_id;
	int shmid; /*共享内存标识符 */
	char *shm_addr; /* 共享内存映射地址 */
	char buff[BUFFER_SIZE];
	sem_id=semget(ftok("/",1),1,0666|IPC_CREAT);
	init_sem(sem_id,0);
	signal(SIGINT,SIG_IGN);
	signal(SIGQUIT,SIG_IGN);
	signal(SIGTSTP,SIG_IGN);
	/* 创建共享内存 */
	if ((shmid = shmget((key_t)KEY, BUFFER_SIZE,0666)) < 0)
	{
		perror("Consumer_shmget");
		exit(1);
	}
	else
	{
		printf(" Shared-memory: %d\n",shmid);
	}
	if ((shm_addr = shmat(shmid, 0, 0)) == (void*)-1)
	{
		perror("Consumer: shmat");
		exit(1);
	}
	else
	{
		printf("Consumer:Attach shared-memory: %p[输出内存地址]\n", shm_addr);
	}
	//system("ipcs -c");
	printf("Wait for producer's data.\n");
	
	while(strncmp(shm_addr,"quit",4))
	{
		sem_p(sem_id);
		strcpy(buff,shm_addr);
		printf("Consumer:%s\n",buff);
		memset(buff,0,BUFFER_SIZE);
		
	}
	if ((shmdt(shm_addr)) < 0)
	{
		perror("Consumer: shmdt");
		exit(1);
	}
	else
	{
		printf("Consumer: Deattach shared-memory\n");
	}
	if(shmctl(shmid,IPC_RMID,NULL)==-1)
	{
		perror("shmctl\n");
		exit(1);
	}
	else
	{
		printf("Delete shared-memory\n");
	}
	printf("Finish!\n");
	exit(0);
}
