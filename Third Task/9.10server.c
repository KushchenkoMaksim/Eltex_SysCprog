/*Программа принимает от клиента два числа и отправляет в ответ их сумму*/
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

	/* Cтруктура для получения сообщения */
	struct mymsgbuf
	{
		long mtype;
		struct {
			int pid;
			int a;
			int b;
		} data;
	} recBuf;

	/* Cтруктура для отправки сообщения */
	struct mymsgbuf2
	{
		long mtype;
		int sum;
	} sendBuf;

	/* Генерируем IPC ключ из имени файла*/

	if((key = ftok(pathname, 0)) < 0){
		printf("Can\'t generate key\n");
		exit(-1);
	}

	/* Пытаемся получить доступ по ключу к очереди сообщений*/

	if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
		printf("Can\'t get msqid\n");
		exit(-1);
	}

	/* Обрабатываем в цикле сообщения */
	while (1) {
		/*Принимаем сообщение*/
		if(msgrcv(msqid, (struct msgbuf *) &recBuf, 100, 1, 0) < 0){
			printf("Can\'t receive message from queue\n");
			exit(-1);
		}

		printf("Recieved %d and %d\n", recBuf.data.a, recBuf.data.b);

		sendBuf.mtype = recBuf.data.pid;
		sendBuf.sum = recBuf.data.a + recBuf.data.b;

		/* Отсылаем сообщение.*/
		if (msgsnd(msqid, (struct msgbuf *) &sendBuf, sizeof(sendBuf.sum), 0) < 0) {
			printf("Can\'t send message to queue\n");
			exit(-1);
		}
		
		printf("Send %d\n\n", sendBuf.sum);
	}

	return 0;
} 
