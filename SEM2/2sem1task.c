#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
	printf("UID = %d, GID = %d\n", (int)getuid(),(int)getgid());
	return 0;
}
