#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int fd, result; 
    size_t size; 
    char resstring[14]; 
    char name[]="aaa.fifo";
    (void)umask(0);
    
    printf("Writing in fifo\n");
    
    if((fd = open(name, O_WRONLY)) < 0){
        printf("Can\'t open FIFO for writing\n");
        exit(-1);
    }
    size = write(fd, "Hello, world!", 14);
    if(size != 14) {
        printf("Can\'t write all string to FIFO\n");
        exit(-1);
    }
    close(fd);
    return 0;
}
