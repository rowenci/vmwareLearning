#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
#include <semaphore.h>
#include "share.h"

union semun{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};
int main()
{
	int running = 1;//循环标志
	int shmid;
	void *shared_memory = (void *)0;//用来保存将来我们从shmat函数返回的共享内存首地址的指针
	shmid = shmget((key_t)1234,sizeof(struct shared_use_st),0600|IPC_CREAT);//生成共享内存标识符
	if(shmid == -1)
	{
		fprintf(stderr,"shmget failed \n");
		exit(1);
	}
	shared_memory = shmat(shmid,(void *)0,0);//把共享内存链接到进程自己的内存空间中，并且把它的首地址返回给shared_memory
	if(shared_memory == (void *)-1)
	{
		fprintf(stderr,"shmat failed \n");
		exit(1);
	}
	int semid = semget(ftok("/",1),1,0666|IPC_CREAT);
	if(semid == -1)
	{
		perror("semget");
		exit(1);
	}
	union semun semarg;
	semarg.val = 0;
	if(semctl(semid,0,SETVAL,semarg) == -1)
	{
		perror("semctl");
		exit(1);
	}
	
	sigset_t set;
	struct sigaction action;
	if(sigemptyset(&set)<0)
	{
		perror("sigempty");
		exit(1);
	}
	if (sigaddset(&set, SIGQUIT) < 0)
	{
		perror("sigaddset");
		exit(1);
	}
	if (sigaddset(&set, SIGINT) < 0)
	{
		perror("sigaddset");
		exit(1);
	}
	if (sigaddset(&set, SIGTSTP) < 0)
	{
		perror("sigaddset");
		exit(1);
	}
	if (sigismember(&set, SIGINT))
	{
		sigemptyset(&action.sa_mask);
		action.sa_handler =SIG_IGN ;
		action.sa_flags = 0;
		sigaction(SIGINT, &action, NULL);
	}
	if (sigismember(&set, SIGQUIT))
	{
		sigemptyset(&action.sa_mask);
		action.sa_handler = SIG_IGN;
		action.sa_flags = 0;
		sigaction(SIGQUIT, &action,NULL);
	}
	if (sigismember(&set, SIGTSTP))
	{
		sigemptyset(&action.sa_mask);
		action.sa_handler = SIG_IGN;
		action.sa_flags = 0;
		sigaction(SIGTSTP, &action,NULL);
	}


	pid_t pid;
	if((pid = fork()) == -1)
	{
		perror("fork error");
		exit(1);	
	}
	if(pid == 0)
	{
		struct shared_use_st *shared_stuff;
		while(shared_stuff->written_by_you == 1)
		{
			sleep(2);
			printf("waiting for writing ... \n");
		}
		
		char buffer[BUFSIZ];		
		printf("The mask of Memory is %d\n",shmid);
		printf("PMemory attached at %X \n",(int)shared_memory);
		printf("UID:%d\n",getuid());
		shared_stuff = (struct shared_use_st *)shared_memory;
		
		while(running)
		{
//新增
if(shared_stuff->written_by_you == 0)
{
			printf("Enter some text:\n");
			fgets(buffer,BUFSIZ,stdin);
}


			strncpy(shared_stuff->some_text,buffer,TEXT_SZ);
			shared_stuff->written_by_you = 1;
			//V操作
			struct sembuf semval;
			semval.sem_num = 0;
			semval.sem_op = 1;
			semval.sem_flg = SEM_UNDO;	
			if(semop(semid,&semval,1) == -1)
			{
				perror("semop");
				exit(1);
			}

			if(strncmp(buffer,"quit",4) == 0)
			{
				running = 0;
				
			}
			
		}
	}
	else
	{
		//P操作
		struct sembuf semval;
		semval.sem_num = 0;
		semval.sem_op = -1;
		semval.sem_flg = SEM_UNDO;
		if(semop(semid,&semval,1) == -1)
		{
			perror("semop");
			exit(1);
		}
		
		struct shared_use_st *shared_stuff;
		printf("The mask of Memory is %d\n",shmid);
		printf("CMemory attached at %X \n",(int)shared_memory);

		shared_stuff = (struct shared_use_st *)shared_memory;// 强制类型转换
		//shared_stuff->written_by_you = 0;//访问共享内存的信息
		while(running)
		{
			if(shared_stuff->written_by_you == 1)//如果值为1，说明生产者已经把新的信息送入共享内存中
			{
				printf("You wrote: %s",shared_stuff->some_text);
				printf("生产者的PID：%d\n",getpid());
				shared_stuff->written_by_you = 0;//置0,说明信息已从共享内存中拿走
				if(strncmp(shared_stuff->some_text,"quit",4) == 0)
				{
					running = 0;
				}
			}
		}
waitpid(pid,WNOHANG);	//新增
printf("written_by_you = %d\n",shared_stuff->written_by_you);
		if(shmdt(shared_memory) == -1)//把共享内存从自己的地址空间中分离出去
		{
			fprintf(stderr,"shmdt failed\n");
			exit(1);
		}
		if(shmctl(shmid,IPC_RMID,0) == -1)//删除共享内存
		{
			fprintf(stderr,"shmctl(IPC_rmid) failed \n");
			exit(1);
		}
		if(semctl(semid,IPC_RMID,0) == -1)
		{
			perror("semctl");
			exit(1);
		}
		exit(0);
	}
}
