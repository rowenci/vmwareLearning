#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define THREAD_NUMBER 3 /* 线程数 */
#define REPEAT_NUMBER 5 /* 每个线程中的小任务数 */
#define DELAY_TIME_LEVELS 10 /*小任务之间的最大时间间隔*/

sem_t sem[THREAD_NUMBER];

void *thrd_func(void *arg){
    int thrd_num = (int)arg;
    int delay_time = 0;
    int count = 0;
    /* 对信号量进行P操作 */
    sem_wait(&sem[thrd_num]);
    printf("Thread %d is starting\n", thrd_num);
    for (count = 0; count < REPEAT_NUMBER; count++){
        /* 得到1-10的随机数 */
        delay_time = rand()% DELAY_TIME_LEVELS  + 1;
        sleep(delay_time);
        printf("\tThread %d: job %d delay = %d\n",thrd_num, count, delay_time);
    }
    printf("Thread %d finished\n", thrd_num);
    pthread_exit(NULL);
}

int main(void){
    pthread_t thread[THREAD_NUMBER];
    int no = 0, res;
    void * thrd_ret;
    srand(time(NULL));
    for (no = 0; no < THREAD_NUMBER; no++){
        /* 创建3个初值为0的局部信号量*/
        sem_init(&sem[no], 0, 0) ;
        res = pthread_create(&thread[no], NULL, thrd_func, (void*)no);
        if (res != 0){
            printf("Create thread %d failed\n", no);
            exit(res);
        }
    }
    printf("Create treads success\n Waiting for threads to finish...\n");
    /* 对最后创建的线程的信号量进行V 操作 */
    sem_post(&sem[THREAD_NUMBER - 1]);
    for(no = THREAD_NUMBER - 1; no >= 0; no--){
        res = pthread_join(thread[no], &thrd_ret) ;
        if (!res){
            printf("Thread %d joined\n", no);
        }
        else{
            printf("Thread %d join failed\n", no);
        }
        /* 对前一个线程进行V操作 */
        sem_post(&sem[(no - 1) % THREAD_NUMBER]);
    }
    for (no = 0; no < THREAD_NUMBER; no++){
        /* 删除信号量 */
        sem_destroy(&sem[no]);
    }
    return 0;
}
