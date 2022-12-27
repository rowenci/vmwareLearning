#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFFER_SIZE 1   // 缓冲区大小，用于从srcFile中读取字符并存储
#define SRC_FILE_NAME "src_file"
#define DEST_FILE_NAME1 "dest_file1"
#define DEST_FILE_NAME2 "dest_file2"
#define DEST_FILE_NAME3 "dest_file3"
#define OFFSET 5    // 读取多少的字符

int main(int argc, char* argv[])
{
    /*
        打开文件
    */
    int srcFile, destFile1, destFile2, destFile3, destFile4;
    unsigned char buff[BUFFER_SIZE];
    int realReadLen;
    srcFile = open(SRC_FILE_NAME, O_RDONLY);
    destFile1 = open(DEST_FILE_NAME1, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    destFile2 = open(DEST_FILE_NAME2, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    destFile3 = open(DEST_FILE_NAME3, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if(srcFile == -1 || destFile1 == -1 || destFile2 == -1 || destFile3 == -1) perror("open");

    // 找到对应的位置
    lseek(srcFile, -OFFSET, SEEK_END);

    // 读取并写入
    while ((realReadLen = read(srcFile, buff, sizeof(buff))) > 0)
        write(destFile1, buff, realReadLen);
    lseek(srcFile, OFFSET, SEEK_SET);
    while ((realReadLen = read(srcFile, buff, sizeof(buff))) > 0)
        write(destFile2, buff, realReadLen);
    close(srcFile);
    close(destFile1);
    return 0;
}