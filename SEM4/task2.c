#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[],char* envp[]){
	pid_t process=fork();
	char *command="ls";
	char *args[3];
	args[0]="ls";
	args[1]="-la";
	args[2]=NULL;
	if (process==-1){
		printf("Mistake, thats why creating new process wasnt realised");
	}
	else if(process==0){
		printf("Thats me, child process = %d",(int)getpid());
		execvp(command,args);
	}
	return 0;
}

