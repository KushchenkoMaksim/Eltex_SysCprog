#include <unistd.h>
#include <stdio.h>
#include <time.h>

int main(){
	FILE *f;
	close(0);
	close(1);
	close(2);
	
	while(1) {
		if( !(f = fopen("timeChecker", "wr"))) {
			return -1;
		}

		time_t rawtime;
		struct tm * timeinfo;
		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		fprintf (f, "Current local time and date: %s\n", asctime (timeinfo) );

		fclose(f);
		sleep(10);
	}
	return 0;
} 
