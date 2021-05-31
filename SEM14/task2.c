#include <signal.h>
#include <stdio.h>

void keyPressed(int nsig) {
  if(nsig == SIGINT) {
    printf("Клавишы CTRL + C нажаты");
  }
  else if(nsig == SIGQUIT) {
    printf("Клавишы CTRL + 4 нажаты");
  }
}

int main(void) {
  (void)signal(SIGINT, keyPressed);
  (void)signal(SIGQUIT, keyPressed);

  while(1);
  return 0;
}
