#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	int semid; /* IPC дескриптор для семафора */
	char pathname[] = "8.7a.c"; /* Имя файла, использующееся для генерации ключа. Файл с таким именем должен существовать в текущей директории */
	key_t key; /* IPC ключ */

	/* Генерируем IPC ключ из имени файла*/
	if((key = ftok(pathname, 1)) < 0) {
		printf("Can\'t generate key\n");
		exit(-1);
	}

	/* Пытаемся создать/получить доступ к семафору синхронизации*/
	if((semid = semget(key, 1, 0666)) < 0) {
		printf("Can\'t get semid\n");
		exit(-1);
	}

	/*Удаляем семафор*/
	if(semctl(semid, 0, IPC_RMID, 0) < 0){
		printf("Can't remove semaphore memory\n");
		exit(-1);
	}
	
	return 0;
} 