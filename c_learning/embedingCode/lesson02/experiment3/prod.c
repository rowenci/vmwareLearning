#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "lock_set.c"
#include "read_file.c"

#define MAXLEN 10
#define ALPHABET 1
#define ALPHABET_START 'a'
#define COUNT_OF_ALPHABET 26
#define DIGIT 2
#define DIGIT_START '0'
#define COUNT_OF_DIGIT 10
#define src_file "products"
char buff[MAXLEN];

int produce(int fd, int sleep_time, int nums)
{

    static int times = 0; // 标志位 标识是数字还是字母
    unsigned int sign_start, sign_count, size;

    lock_set(fd, F_WRLCK);
    for (int i = 0; i < nums; i++)
    {
        if (times >= COUNT_OF_DIGIT)
        {
            sign_start = ALPHABET_START;
            sprintf(buff, "%c", (sign_start + times - COUNT_OF_DIGIT));
            printf("product %c \n", buff[0]);
            sleep(sleep_time);
        }
        else
        {
            sign_start = DIGIT_START;
            sprintf(buff, "%c", (sign_start + times));
            printf("product %c\n", buff[0]);
            sleep(sleep_time);
        }
        times = (times + 1) % (COUNT_OF_ALPHABET + COUNT_OF_DIGIT);

        if ((write(fd, buff, strlen(buff))) < 0)
        {
            printf("Producer: write error!\n");
            return -1;
        }
    }
    lock_set(fd, F_UNLCK);
    return 0;
}

int main(int argc, char *argv[])
{
    int time_step = 0.2;
    int time_life = 10;
    if (argc > 1)
    {
        sscanf(argv[1], "%d", &time_step);
    }
    if (argc > 2)
    {
        sscanf(argv[2], "%d", &time_life);
    }

    int fd;
    if ((fd = open(src_file, O_CREAT | O_RDWR | O_APPEND, 0644)) < 0)
    {
        printf("Open Error!\n");
        exit(1);
    }
    read_file(src_file);
    produce(fd, time_step, time_life);
	close(fd);
	return 0;
}
