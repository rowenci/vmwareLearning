#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREAD_NUMBER 3
#define REPEAT_NUMBER 5
#define DELAY_TIME_LEVELS 10

void* threadFunc(void *arg){
    int threadNum = (int)arg;
    int delayTime = 0;
    printf("thread %d is starting\n", threadNum);
    for(int count = 0; count < REPEAT_NUMBER; count++){
        delayTime = rand() % DELAY_TIME_LEVELS + 1;
        sleep(delayTime);
        printf("\tthread %d : job %d delay = %d\n", threadNum, count, delayTime);
    }
    printf("thread %d is finished\n", threadNum);
    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    pthread_t thread[THREAD_NUMBER];
    int res;
    srand(time(0));
    for(int i = 0; i < THREAD_NUMBER; i++){
        res = pthread_create(&thread[i], NULL, threadFunc, i);
        if(res != 0){
            printf("create thread %d failed\n", i);
            exit(res);
        }
    }
    printf("create threads success\n waiting for threads to finish...\n");
    for(int i = 0; i < THREAD_NUMBER; i++){
        res = pthread_join(thread[i], NULL);
        if(!res) printf("thread %d joined\n", i);
        else printf("thread %d join failed\n", i);
    }
    return 0;
}