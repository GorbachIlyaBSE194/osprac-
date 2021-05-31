#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <stdlib.h>

void escapeEvent(int nsig) {
  int status;
  pid_t pid;

  while ((pid = waitpid(-1, &status, 0))>= 0){
    if ((status & 0xff) == 0) {
        printf("Процесс %d был завершён сигналом %d\n", pid, status >> 8);
    } else if ((status & 0xff00) == 0) {
      if(status & 0x80){
        printf("Процесс %d убит сигналом %d с core файлом\n", pid, status &0x7f);
      }
      else{
        printf("Процесс %d убит сигналом %d без core файла\n", pid, status &0x7f);
      }
    }
  }
}

int main(void) {
  pid_t pid;
  int i,j;

  (void) signal(SIGCHLD, escapeEvent);

  for (i=0; i<10; i++) {
    if ((pid = fork()) < 0) {
      printf("Can\'t fork child 1\n");
      exit(1);
    } else if (pid == 0) {
      for (j=1; j < 10000000; j++);
      exit(200 + i);
    }
  }

  while(1);
  return 0;
}
