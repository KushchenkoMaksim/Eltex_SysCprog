/*Удаляем очередь сообщений */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	int msqid; /* IPC дескриптор для очереди сообщений */
	char pathname[] = "9.10server.c"; /* Имя файла, использующееся для генерации ключа.*/
	key_t key; /* IPC ключ */

	/* Генерируем IPC ключ из имени файла*/

	if((key = ftok(pathname, 0)) < 0){
		printf("Can\'t generate key\n");
		exit(-1);
	}

	/* Пытаемся получить доступ по ключу к очереди сообщений*/

	if((msqid = msgget(key, 0666)) < 0){
		printf("Can\'t get msqid\n");
		exit(-1);
	}

	if(msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL) < 0) {
		printf("Can\'t remove msg queue\n");
		exit(-1);
	}

	return 0;
} 
