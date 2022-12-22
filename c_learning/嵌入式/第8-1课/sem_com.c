int init_sem(unsigned short sem_array[2],int sem_id)
{
union semun{
int val;
struct semid_ds *buf;
unsigned short *array;
};
union semun sem_union;
sem_union.array = sem_array;   
if (semctl(sem_id, 0,SETALL, sem_union) == -1)
{
perror("Initialize semaphore");
return -1;
}
return 0;
}
/* 从系统中删除信号量的函数 */
int del_sem(int sem_id)
{
union semun{
int val;
struct semid_ds *buf;
unsigned short *array;
};
union semun sem_union;
if (semctl(sem_id, 1, IPC_RMID, sem_union) == -1)
{
perror("Delete semaphore");
return -1;
}
}
/* P 操作函数 */
int sem_p(int sem_id,int semnum)
{
struct sembuf sem_b[2];
sem_b[semnum].sem_num = semnum; /* 单个信号量的编号应该为0，即信号量集中的第一个信号量 */
sem_b[semnum].sem_op = -1; /* 表示P 操作 */
sem_b[semnum].sem_flg = SEM_UNDO; /* 系统自动释放将会在系统中残留的信号量*/
if (semop(sem_id, &sem_b[semnum], 1) == -1)
{
perror("P operation");
return -1;
}
return 0;
}
/* V 操作函数*/
int sem_v(int sem_id,int semnum)
{
struct sembuf sem_b[2];
sem_b[semnum].sem_num = semnum; /* 单个信号量的编号应该为0 */
sem_b[semnum].sem_op = 1; /* 表示V 操作 */
sem_b[semnum].sem_flg = SEM_UNDO; /* 系统自动释放将会在系统中残留的信号量*/
if (semop(sem_id, &sem_b[semnum], 1) == -1)
{
perror("V operation");
return -1;
}
return 0;
}
