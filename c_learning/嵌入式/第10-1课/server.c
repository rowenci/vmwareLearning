#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define BUFFER_SIZE 512

struct message{
	long msg_type;
	char msg_text[BUFFER_SIZE];
};

int main(){
	int qid1;
	int qid2;
	pid_t pid;
	key_t key1;
	key_t key2;
	struct message msg1;
	struct message msg2;
	if((pid = fork()) == -1){
		perror("fork");
		exit(1);
	}
	if((key1 = ftok(".", 1)) == -1){
		perror("ftok");
		exit(1);
	}
	if((key2 = ftok(".", 2)) == -1){
		perror("ftok");
		exit(1);
	}
	if((qid1 = msgget(key1, IPC_CREAT|0666)) == -1){
		perror("msgget");
		exit(1);
	}
	if((qid2 = msgget(key2, IPC_CREAT|0666)) == -1){
		perror("msgget");
		exit(1);
	}
	//子进程接收信息
	if(pid == 0){
		do{
			memset(msg2.msg_text, 0, BUFFER_SIZE);
			if(msgrcv(qid2, &msg2, BUFFER_SIZE, 0, 0) < 0){
				perror("msgrcv");
				exit(1);
			}
			printf("子进程接收信息：the message from process %d : %s", msg2.msg_type, msg2.msg_text);
		} while (strncmp(msg2.msg_text, "quit", 4));
		exit(1);
	}
	printf("open queue %d\n", qid1);
	while(1){
		printf("主进程发送信息：enter message to queue:");
		if((fgets(msg1.msg_text, BUFFER_SIZE, stdin)) == NULL){
			printf("no message");
			exit(1);
		}
		msg1.msg_type = getpid();
		if((msgsnd(qid1, &msg1, strlen(msg1.msg_text), 0)) < 0){
			perror("message send");
			exit(1);
		}
		if(strncmp(msg1.msg_text, "quit", 4) == 0){
			msgctl(qid1, IPC_RMID, NULL);
			msgctl(qid2, IPC_RMID, NULL);
			break;
		}
	}
	exit(0);
}
