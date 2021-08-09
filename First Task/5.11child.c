#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[]){
	int fd;
	int size;
	
	if(argc < 2) {
		printf("Not enough parametrs\n");
		exit(-1);
	}

	fd = atoi(argv[1]);
	size = write(fd, "Hello?", 20);
	if(size != 20){
		printf("Can\'t write full string\n");
		exit(-1);
	}
	close(fd);
	return 0;
} 
