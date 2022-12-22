#include <stdio.h>
#include <string.h>
int main()
{
	char buf[100];
	memset(buf,0,sizeof(buf));
	read(0,buf,sizeof(buf));
//	read(1,buf,sizeof(buf));
//	read(2,buf,sizeof(buf));
	printf("buf is %s",buf);
}
