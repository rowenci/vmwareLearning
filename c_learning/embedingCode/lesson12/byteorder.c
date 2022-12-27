#include <stdio.h>

int main(int argc, char* argv[])
{
    union 
    {
        short value;
        char bytes[sizeof(short)];
    } test;
    test.value = 0x0102;
    if((test.bytes[0] == 1) && (test.bytes[1] == 2)){
        printf("big order\n");
    }else if((test.bytes[0] == 2) && (test.bytes[1] == 1)){
        printf("small order\n");
    }else{
        printf("unknown\n");
    }
    // 大、小端是指，地址的高位存的是数据的高位还是低位
    return 0;
}