#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255 // Message type for termination of program 11-1b.c

int main(void)
{
  int msqid;      // IPC descriptor for the message queue
  char pathname[]="task11-2a.c"; // The file name used to generate the key.
                             // A file with this name must exist in the current directory.
  key_t  key;     // IPC key
  int i,len;      // Cycle counter and the length of the informative part of the message

  struct mymsgbuf // Custom structure for the message
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
  //
  // Trying to get access by key to the message queue, if it exists,
  // or create it, with read & write access for all users.
  //
  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get msqid\n");
    exit(-1);
  }

  /* Send information */
  printf("task1.\n");
  for (i = 1; i <= 5; i++) {
    //
    // Fill in the structure for the message and
    // determine the length of the informative part.
    //
    mybuf.mtype = 1;
    mybuf.info.sinfo = 2000;
    mybuf.info.finfo = 31.07;
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

  printf("Сообщения отправлены\n");
  /* Send the last message */

  for (size_t i = 1; i <= 6; i++)
  {
    len = sizeof(mybuf.info);
    if (msgrcv(msqid, (struct msgbuf *) &mybuf, len, 2, 0) < 0) {
      printf("Can\'t receive message from queue\n");
      exit(-1);
    }
    printf("Параметры прочитанного. Тип сообщения: %ld, finfo = %f, sinfo = %i\n", mybuf.mtype, mybuf.info.finfo, mybuf.info.sinfo);
  }

  msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);

  printf("exit.\n");

  return 0;
}
