#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    const char srcFile[] = "srcFile";
    const char targetFile[] = "targetFile";
    int srcFD = open(srcFile, O_RDONLY);
    int targetFD = open(targetFile, O_RDWR | O_CREAT, 0644);
    if(srcFD == -1 || targetFD == -1) perror("open");

    unsigned char buffer[1];
    int readLen;

    lseek(srcFD, 0, SEEK_SET);
    while((readLen = read(srcFD, buffer, sizeof(buffer))) > 0)
        write(targetFD, buffer, readLen);

    close(srcFD);
    close(targetFD);
    return 0;
}