#include <stdio.h>
#include <syslog.h>
#include <fcntl.h>
#include <string.h>
int main()
{
char buf[100];
sprintf(buf,"The pid is : %d",getpid());
printf("buf is: %s\n",buf);
//openlog("1.c",LOG_PID,LOG_DAEMON);
//syslog(LOG_INFO,buf);
//int fd=open("/tmp/daemon.log",O_CREAT|O_APPEND|O_WRONLY,0600);
//write(fd,buf,strlen(buf));
//write(fd,"\n",1);
//close(fd);
//exit(0);
}
