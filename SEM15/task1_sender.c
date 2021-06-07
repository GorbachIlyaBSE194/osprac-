#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
  pid_t this_pid, receive_pid;
  int value;

  this_pid = getpid();
  printf("Sender pid = %d\n", this_pid);

  printf("Input receiver pid = ");
  scanf("%d", &receive_pid);

  printf("Input number = ");
  scanf("%d", &value);

  int byte = 1;
  while (byte != 0) {
    if ((value & byte) == 0) {
      kill(receive_pid, SIGUSR1);
    } else {
      kill(receive_pid, SIGUSR2);
    }
    byte = byte << 1;
  }
}
