#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
	pid_t process=fork();
	if (process==-1){
		printf("Mistake, thats why creating new process wasnt realised");
	}
	else if(process==0){
		printf("Thats me, child process = %d",(int)getpid());
	}
	else {
		printf("Thats parent process = %d", (int)getpid());
	}
	return 0;
}

