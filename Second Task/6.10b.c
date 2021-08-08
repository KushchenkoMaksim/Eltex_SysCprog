#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
	char *text; /* Указатель на разделяемую память */
	int shmid; /* IPC дескриптор для области разделяемой памяти */
	key_t key; /* IPC ключ */
	struct shmid_ds shmData; /* Данные о созданной разделяемой памяти */
	int memSize;

	/* Генерируем IPC ключ из имени файла*/
	if((key = ftok("6.10a.c", 0)) < 0){
		printf("Can\'t generate key\n");
		exit(-1);
	}

	/* Пытаемся получить доступ к разделяемой памяти*/
	if((shmid = shmget(key, 1, 0666)) < 0) {
		printf("Can\'t read shared memory\n");
		exit(-1);
	}

	/* Получаем структуру с данными. Нас интересует размер */
	if(shmctl(shmid, IPC_STAT, &shmData) < 0) {
		printf("Can\'t read shared memory\n");
		exit(-1);
	}
	memSize = shmData.shm_segsz;

	/* Пытаемся получить доступ к разделяемой памяти*/
	if((shmid = shmget(key, memSize, 0666)) < 0) {
		printf("Can\'t read shared memory\n");
		exit(-1);
	}

	/* Пытаемся отобразить разделяемую память в адресное пространство текущего процесса.*/
	if((text = (char *)shmat(shmid, NULL, 0)) == (char *)(-1)) {
		printf("Can't attach shared memory\n");
		exit(-1);
	}

	printf("Text is:\n%s\n", text);
	
	if(shmdt(text) < 0){
		printf("Can't detach shared memory\n");
		exit(-1);
	}
	if(shmctl(shmid, IPC_RMID, 0) < 0){
		printf("Can't remove shared memory\n");
		exit(-1);
	}
	return 0;
} 