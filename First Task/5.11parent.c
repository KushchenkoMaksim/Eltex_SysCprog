#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
	int fd[2], result;
	int size;
	char resstring[20];

	/* Попытаемся создать pipe */
	if(pipe(fd) < 0) {
		printf("Can\'t create pipe\n");
		exit(-1);
	}

	/* Порождаем новый процесс */
	result = fork();
	if(result <0) {
		/* Если создать процесс не удалось, сообщаем об этом и завершаем работу */
		printf("Can\'t fork child\n");
		exit(-1);

	} else if (result > 0) {
		/* Порождаем новый процесс */
		result = fork();
		switch(result) {
			case -1:
				printf("Can\'t fork child\n");
				exit(-1);
			case 0:
				//child2 Отправляет сообщение
				close(fd[0]);
				char strfd[10];
				sprintf(strfd, "%d", fd[1]);
				if(execl("5.11child", "5.11child", strfd, NULL) < 0) {
					printf("Can't exec\n");
				}
				break;
			default:
				//parent
				close(fd[0]);
				close(fd[1]);
				break;
		}
	}
	else {
		//child1 принимает и печатает сообщение
		close(fd[1]);
		size = read(fd[0], resstring, 20);
		if(size == 0){
			printf("Can\'t read string\n");
			exit(-1);
		}
		printf("%s\n",resstring);
		close(fd[0]);
	}

	return 0;
} 
