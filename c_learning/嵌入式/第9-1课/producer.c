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
	pid_t pid=getpid();
	char flag[1024] ;
	sprintf(flag,"The producer pid is %d.\n",pid);
	sem_id=semget(ftok("/",1),1,0666|IPC_CREAT);
	init_sem(sem_id,0);
	signal(SIGINT,SIG_IGN);
	signal(SIGQUIT,SIG_IGN);
	signal(SIGTSTP,SIG_IGN);
	/* 创建共享内存 */
	if ((shmid = shmget((key_t)KEY, BUFFER_SIZE, 0666|IPC_CREAT)) < 0)
	{
		perror("Producer_shmget");
		exit(1);
	}
	else
	{
		printf("Create shared-memory: %d\n",shmid);
	}
	if ((shm_addr = shmat(shmid, 0, 0)) == (void*)-1)
	{
		perror("Producer: shmat");
		exit(1);
	}
	else
	{
		printf(":Producer:Attach shared-memory: %p[输出内存地址]\n", shm_addr);
	}
	system("ipcs -c");
	while(strncmp(buff,"quit",4))
	{
		//memset(buff,0,BUFFER_SIZE);
		//memset(shm_addr,0,BUFFER_SIZE);
		printf("\nInput some string:\n");
		fgets(buff, BUFFER_SIZE, stdin);
		strncpy(shm_addr, buff, strlen(buff));
		strncpy(shm_addr+ strlen(buff), flag, strlen(flag));
		sem_v(sem_id);
	}
	if ((shmdt(shm_addr)) < 0)
	{
		perror("Producer: shmdt");
		exit(1);
	}
	else
	{
		printf("Producer: Deattach shared-memory\n");
	}
	printf("Finish!\n");
	exit(0);
}
