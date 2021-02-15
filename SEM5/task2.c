#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[], char* envp[])
{
    int fd[2], result;
    size_t size;
    char resstring[14];

    if (pipe(fd) < 0) {
        printf("Can\'t create pipe\n");
        exit(-1);
    }

    result = fork();
    if (result < 0) {
        printf("Can\'t fork child\n");
        exit(-1);
    } else if (result > 0) {
        close(fd[0]);
        printf("This is parent process which write data to the pipe\n");
        dup2(fd[1], 1);
        execle("/bin/ls", "ls", 0, envp);
    } else {
        close(fd[1]);
        dup2(fd[0], 0);
        printf("This is child process which counts words from the pipe\n");
        execle("/usr/bin/wc", "wc", "-w", 0, envp);
    }
    return 0;
}
