#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "lock_set.c"
#include "read_file.c"

#define MAX_FILE_SIZE 100 * 1024 * 1024 // 定义文件的大小
#define src_file "products"
#define custom_file "customs"

int move(int read_file, int aim_file, int postion, int size)
{
    if (read_file < 0 || aim_file < 0)
    {
        printf("打开文件出错\n");
        exit(-1);
    }
    lseek(read_file, postion, SEEK_SET);
    unsigned char buff;
    int counter = 0;
    lock_set(read_file, F_RDLCK);
    lock_set(aim_file, F_WRLCK);
    while ((read(read_file, &buff, 1)) == 1 && (counter < size))
    {
        write(aim_file, &buff, 1);
        counter++;
    }
    lock_set(aim_file, F_UNLCK);
    lock_set(read_file, F_UNLCK);
}

int main(int argc, char const *argv[])
{
    int start_pos = 0;
    int size = 5;
    if (argc > 1)
    {
        sscanf(argv[1], "%d", &start_pos);
    }
    if (argc > 2)
    {
        sscanf(argv[2], "%d", &size);
    }
    read_file(src_file);
    int res_file = open(src_file, O_RDWR);
    int aim_file = open(custom_file, O_RDWR | O_CREAT, 0644);
    move(res_file, aim_file, start_pos, size);
    read_file(custom_file);
    return 0;
}
