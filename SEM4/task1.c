#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[],char* envp[]){
	printf("this is command line arguements (argv[]):\n");
	for(int i=0; i<argc; i++){
		printf("argv[%d]=%s\n",i, argv[i]);
	}
	printf("\n");
	int k=0;
	while(envp[k]!=NULL){
		printf("envp[%d]=%s\n",k, envp[k]);
		k++;
	}
	printf("\n");
	return 0;
}
