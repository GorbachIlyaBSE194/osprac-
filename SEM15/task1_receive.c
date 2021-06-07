#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int value;
int byte;

void first(int sig) {
  byte <<= 1;
}

void second(int sig) {
  value |= byte;
  byte <<= 1;
}

int main() {
  pid_t this_pid, sender_pid;
  value = 0;
  byte = 1;
  (void) signal(SIGUSR1, first);
  (void) signal(SIGUSR2, second);

  this_pid = getpid();
  printf("This receiver pid = %d\n", this_pid);

  printf("Sender pid = ");
  scanf("%d", &sender_pid);

  while (1) {
	if(byte == 0) {
      printf("Received number = %d\n", value);
      break;
    }
  }
}
