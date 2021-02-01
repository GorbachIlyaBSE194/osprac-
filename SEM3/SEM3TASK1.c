#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
	printf("pid= %d, ppid= %d",(int)getpid(),(int)getppid());
	return 0;
}
