#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
	int fd[2], result;
	size_t size;
	char resstring[14];

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
				//child2
				close(fd[0]);
				size = write(fd[1], "Hello?", 14);

				if(size != 14){
					printf("Can\'t write full string\n");
					exit(-1);
				}
				close(fd[1]);
				break;
			default:
				//parent
				close(fd[0]);
				close(fd[1]);
				break;
		}
	}
	else {
		//child1
		close(fd[1]);
		size = read(fd[0], resstring, 14);

		if(size == 0){
			printf("Can\'t read string\n");
			exit(-1);
		}

		printf("%s\n",resstring);
		close(fd[0]);
	}

	return 0;
} 
