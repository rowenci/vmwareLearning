#include <stdio.h>
#include <string.h>
#define src_file "products"

void read_file(char *read_file)
{
    FILE *fp;
    char ch;
    fp = fopen(read_file, "r");
    if (fp == NULL)
        printf("can't open!\n");
    else
    {
        fscanf(fp, "%c", &ch);
        while (!feof(fp))
        {
            putchar(ch);
            fscanf(fp, "%c", &ch);
        }
        fclose(fp);
    }
    printf("\n");
}
