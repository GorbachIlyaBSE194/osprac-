#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255

int main(void)
{
  int msqid;
  char pathname[]="task11-2a.c";
  key_t  key;
  int len, maxlen;

  struct mymsgbuf
  {
    long mtype;
    struct 
    {
      float finfo;
      short sinfo;
    } info;
  } mybuf;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get msqid\n");
    exit(-1);
  }

  printf("task2.\n");

  for (size_t i = 1; i <= 5; i++)
  {
    len = sizeof(mybuf.info);
    // 1, так как ожидаем сообщения от первого чела.
    if (msgrcv(msqid, (struct msgbuf *) &mybuf, len, 1, 0) < 0) {
      printf("Can\'t receive message from queue\n");
      exit(-1);
    }
    printf("Параметры прочитанного: Тип сообщения: %ld, finfo = %f, sinfo = %i\n", mybuf.mtype, mybuf.info.finfo, mybuf.info.sinfo);
  }

  printf("Отправка сообщений.\n");
  for (int i = 1; i <= 6; i++) {
    //
    // Fill in the structure for the message and
    // determine the length of the informative part.
    //
    mybuf.mtype = 2;
    mybuf.info.sinfo = 31.07;
    mybuf.info.finfo = 2000;
    len = sizeof(mybuf.info);
    //
    // Send the message. If there is an error,
    // report it and delete the message queue from the system.
    //
    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }
  }

  printf("exit.\n");
  
  return 0;
}
