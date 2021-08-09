/* Программа записывает свой исходный код в разделяемую память */
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
	FILE* fp; /* Файл с кодом */

	/* Генерируем IPC ключ из имени файла*/
	if((key = ftok("6.10a.c", 0)) < 0){
		printf("Can\'t generate key\n");
		exit(-1);
	}

	/* Считаем размер файла */
	int fileSize = 1;
	char c;
	if ((fp = fopen("6.10a.c", "r")) == NULL) {
		printf("Could not open file 6.10.c\n");
		return 0;
	}

	for (c = getc(fp); c != EOF; c = getc(fp), ++fileSize);
	/* Возвращаем каретку в начало для считывания */
	fseek(fp, 0, SEEK_SET); 

	/* Пытаемся создать/получить доступ к разделяемой памяти*/
	if((shmid = shmget(key, fileSize, 0666|IPC_CREAT)) < 0) {
		printf("Can\'t create shared memory\n");
		exit(-1);
	}

	printf("Shared memory id: %d\n", shmid);

	/* Пытаемся отобразить разделяемую память в адресное пространство текущего процесса.*/
	if((text = (char *)shmat(shmid, NULL, 0)) == (char *)(-1)) {
		printf("Can't attach shared memory\n");
		exit(-1);
	}

	/*Записываем код программы в разделяемую память*/
	int i = 0;
	for (c = getc(fp); c != EOF; c = getc(fp), ++i) {
		text[i] = c;
	}
	text[i] = '\0';

	fclose(fp);
	if(shmdt(text) < 0){
		printf("Can't detach shared memory\n");
		exit(-1);
	}
	return 0;
} 
