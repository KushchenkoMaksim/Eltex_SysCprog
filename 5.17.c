#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char * argv[]){
	int fd;
	size_t size;
	char resstring[14];
	char name[20] = "lol";

	if( argc < 2 ) {
		write(2, "No mode passed. Expected \"read\" or \"write\".\n", 44); 
		return -1;
	}

	(void)umask(0);

	//Пытаемся создать fifo
	if(mkfifo(name, 0666) && errno != EEXIST) {
		write(2, "Can't create fifo.\n", 19); 
		return -1;
	}
	
	if(!strcmp(argv[1], "read")) {
		//read
		if((fd = open(name, O_RDONLY)) < 0){
			write(2, "Can't open fifo for reading\n", 28);
			exit(-1);
		}

		size = read(fd, resstring, 14);
		if(size != 14){
			write(2, "Can't read all string from FIFO\n", 32);
			close(fd);
			exit(-1);
		}
		printf("%s\n", resstring);
		close(fd);
	}
	else if (!strcmp(argv[1], "write")) {
		//write
		if((fd = open(name, O_WRONLY)) < 0){
			write(2, "Can't open fifo for writting\n", 29);
			exit(-1);
		}

		size = write(fd, "Hello, world!", 14);
		if(size != 14){
			write(2, "Can't write all string to FIFO\n", 31);
			close(fd);
			exit(-1);
		}
		close(fd);
	}
	else {
		write(2, "Wrong mode passed. Expected \"read\" or \"write\".\n", 47);
		return -1;
	}

	return 0;
} 
