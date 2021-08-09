/*Пример обрабатывания критической секции с помощью семафора */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
	int *array; /* Указатель на разделяемую память */
	int shmid; /* IPC дескриптор для области разделяемой памяти */
	int semid; /* IPC дескриптор для семафора */
	int new = 1; /* Флаг необходимости инициализации элементов массива */
	char pathname[] = "8.7a.c"; /* Имя файла, использующееся для генерации ключа. Файл с таким именем должен существовать в текущей директории */
	key_t key; /* IPC ключ */
	long i;
	struct sembuf mybuf; /* Структура для задания операции над семафором */

	/* Генерируем IPC ключ из имени файла*/
	if((key = ftok(pathname, 0)) < 0) {
		printf("Can\'t generate key\n");
		exit(-1);
	}

	/* Пытаемся создать/получить доступ к разделяемой памяти*/
	if((shmid = shmget(key, 3 * sizeof(int), 0666|IPC_CREAT|IPC_EXCL)) < 0) {
		/* В случае возникновения ошибки пытаемся определить: возникла ли она из-за того, что сегмент разделяемой памяти уже существует, или по другой причине */
		if(errno != EEXIST) {
			/* Если по другой причине - прекращаем работу */
			printf("Can\'t create shared memory\n");
			exit(-1);
		}
		else {
			/* Если из-за того, что разделяемая память уже существует, пытаемся получить ее IPC дескриптор и, в случае удачи, сбрасываем флаг необходимости инициализации элементов массива */
			if((shmid = shmget(key, 3*sizeof(int), 0)) < 0) {
				printf("Can\'t find shared memory\n");
				exit(-1);
			}
			new = 0;
		}
	}

	/* Генерируем IPC ключ из имени файла*/
	if((key = ftok(pathname, 1)) < 0) {
		printf("Can\'t generate key\n");
		exit(-1);
	}

	/* Пытаемся создать/получить доступ к семафору синхронизации*/
	if((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
		printf("Can\'t get semid\n");
		exit(-1);
	}

	/*Инициализируем семафор еденицой*/
	mybuf.sem_op = 1;
	mybuf.sem_flg = 0;
	mybuf.sem_num = 0;

	if(semop(semid, &mybuf, 1) < 0){
		printf("Can\'t wait for condition\n");
		exit(-1);
	}

	/*Занимаем семафор, если свободен*/
	mybuf.sem_op = -1;
	mybuf.sem_flg = 0;
	mybuf.sem_num = 0;

	if(semop(semid, &mybuf, 1) < 0) {
		printf("Can\'t wait for condition\n");
		exit(-1);
	}

	/* Пытаемся отобразить разделяемую память в адресное пространство текущего процесса.*/
	if((array = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
		printf("Can't attach shared memory\n");
		exit(-1);
	}

	/* В зависимости от значения флага new либо инициализируем массив, либо увеличиваем соответствующие счетчики */
	if(new){
		array[0] = 1;
		array[1] = 0;
		array[2] = 1;
	}
	else {
		array[0] += 1;
		for(i=0; i<1000000000L; i++);
		array[2] += 1;
	}

	/* Печатаем новые значения счетчиков, удаляем разделяемую память из адресного пространства текущего процесса и завершаем работу */ 
	printf("Program 1 was spawn %d times, program 2 - %d times, total - %d times\n", array[0], array[1], array[2]);

	/*Освобождаем семафор*/
	mybuf.sem_op = 1;
	mybuf.sem_flg = 0;
	mybuf.sem_num = 0;

	if(semop(semid, &mybuf, 1) < 0){
		printf("Can\'t wait for condition\n");
		exit(-1);
	}

	if(shmdt(array) < 0){
		printf("Can't detach shared memory\n");
		exit(-1);
	}
	return 0;
} 
