#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h> 
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
struct MSG{
long mtype;//消息类型
 char buf[100];//消息数据
};
struct MSG_read{
    long mtype_read;//消息类型
    char buf_read[100];//消息数据
};

void *read2(void *arg)
{

       //1.获取key
        key_t key = ftok("/",'a');
    if(key==-1)
    {
        perror("ftok");
        exit(-1);
    }
  int msqid = msgget(key,IPC_CREAT|0666);       //2.通过key创建消息队列
  if(msqid==-1)
    {
        perror("msgget");
        exit(-2);
    }
 struct MSG msg1;
     while(1)
     {
        int res = msgrcv(msqid,&msg1,sizeof(msg1.buf),1,0);   //收类型为2 的信息     
       if(res==-1){
            perror("msgrcv");
            exit(-3);
        }
          if(strncmp(msg1.buf,"quit",4) == 0)
      {     
          exit(0);
       }

         printf("%s",msg1.buf);
     }
  
}
void *write2(void *arg)
{
 
    key_t key = ftok("/",'b');
    if(key==-1)
    {
        perror("ftok");
        exit(-1);
    }
 
 
     int msqid = msgget(key,IPC_CREAT|0666);       //2.通过key创建消息
      if(msqid==-1)
    {
        perror("msgget");
        exit(-2);
    }
 
    struct MSG_read msg1;
    msg1.mtype_read = 2;
    while(1)
    {
        
        
        fgets(msg1.buf_read,100,stdin);
  //  if(strncmp(msg1.buf_read,"quit",4) == 0)
    // {
      //  exit(0);
    // }
 
        int res = msgsnd(msqid,&msg1,sizeof(msg1.buf_read),0);
 
    }
}
int main()
{
	pthread_t id1;
	pthread_t id2;
       //创建线程
       int res = pthread_create(&id1,0,write2,0);
    if(res) 
{
        printf("%s\n",strerror(res));
}
 int res2 = pthread_create(&id2,0,read2,0);
    if(res2)
{
        printf("%s\n",strerror(res2));
}
pthread_join(id1,0);
 pthread_join(id2,0);
 
	return 0;
}
