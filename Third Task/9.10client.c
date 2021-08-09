#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main()
{
	int msqid; /* IPC дескриптор для очереди сообщений */
	char pathname[] = "9.10server.c"; /* Имя файла, использующееся для генерации ключа.*/
	key_t key; /* IPC ключ */
	int i; /* Cчетчик цикла*/

	/* Cтруктура для отправки сообщения */
	struct mymsgbuf
	{
		long mtype;
		struct {
			int pid;
			int a;
			int b;
		} data;
	} sendBuf;

	/* Cтруктура для получения сообщения */
	struct mymsgbuf2
	{
		long mtype;
		int sum;
	} recBuf;

	srand(time(0));

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

	/* Обрабатываем в цикле 5 сообщений */
	for (i = 1; i <= 5; i++){
		/* Заполняем структуру для нашего сообщения */
		sendBuf.mtype = 1;
		sendBuf.data.pid = getpid();
		sendBuf.data.a = rand() % 100;
		sendBuf.data.b = rand() % 100;

		printf("Send %d and %d\n", sendBuf.data.a, sendBuf.data.b);

		/* Отсылаем сообщение.*/
		if (msgsnd(msqid, (struct msgbuf *) &sendBuf, sizeof(sendBuf.data), 0) < 0) {
			printf("Can\'t send message to queue\n");
			exit(-1);
		}

		/*Принимаем ответ*/
		if(msgrcv(msqid, (struct msgbuf *) &recBuf, 5, sendBuf.data.pid, 0) < 0) {
			printf("Can\'t receive message from queue\n");
			exit(-1);
		}
		
		printf("Recieved %d\n", recBuf.sum);
		sleep(2);
	}

	return 0;
} 
