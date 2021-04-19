#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/sem.h>


//Операция А- добавляет значение к семафору
//Операция D- убавляет значение семафора
//Операция Z- проверка на 0
/**
Чтобы ребенок не мог читать и писать, выполняем операцию D(1), идет проверка на 0, затем ребенок берется за дело, добвляя в семафор операцией А(2) 2 единицы,затем отец, заходя в цикл он уменьшает значение семафора на 1, и потом, выходя из цикла, он также уменьшает значение семафора на 1, сравнение с нулем, и опять по кругу. 
 */
void create_semop(int sem_id, int sem_op) {
  struct sembuf mybuf;
  mybuf.sem_num = 0;
  mybuf.sem_flg = 0;
  mybuf.sem_op  = sem_op;

  if (semop(sem_id, &mybuf, 1) < 0) {
    printf("Can\'t wait for condition\n");
    exit(-1);
  }
}

/**
 *  Просто увеличиваем значение
 */
void A(int sem_id, int value) {
  create_semop(sem_id, value);
}

/**
 * Уменьшаем значение и ждём >= 0
 */ 
void D(int sem_id, int value) {
  create_semop(sem_id, -value);
}

/**
 * Проверка на 0.
 */
void Z(int sem_id) {
  create_semop(sem_id, 0);
}


int main()
{
    // Инициализируем: 
    int parent[2], result;
    size_t size = 0;
    key_t key;
    char pathname[] = "task1_fixed.c";
    struct sembuf buffer;

    int semid;
    char resstring[14];

    // Создаём Pipes родителя и ребёнка:
    if (pipe(parent) < 0) {
        printf("Невозможно создать pipe\n\r");
        exit(-1);
    }

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Ключ не сгенерирован.\n");
        exit(-1);
    }

    // Создаём семафорчик
    if ((semid = semget(key, 1, 0666|IPC_CREAT|IPC_EXCL)) < 0) {
        if (errno != EEXIST) {
        printf("Can\'t create semaphore set\n");
        exit(-1);
        } else if ((semid = semget(key, 1, 0)) < 0) {
        printf("Can\'t find semaphore\n");
        exit(-1);
       }
    } else {
        A(semid, 2);
    }

    int N;
    printf("Введите натуральное число n большее или равное двум: \n");
    scanf("%d", &N);
    if (N < 2) {
        printf("N должен быть больше или равен 2.\n");
        exit(-1);
    }


    result = fork();

    if (result < 0) {
        printf("Не удалось совершить fork\n\r");
        exit(-1);
    }

    // Пул отца:
    else if (result > 0) {
        for (size_t i = 0; i < N; i++)
        {
            D(semid, 1);
            if (i != 0) {
                // Читаем сообщение ребёнка.
                size = read(parent[0], resstring, 14);
                if (size < 0) {
                printf("Can\'t read string from pipe\n");
                exit(-1);
                }
                printf("Сообщение №%d. Отец читает сообщение:%s\n", i, resstring);
            }

            // Пишем сообщение ребёнку.
            size = write(parent[1], "Privet sinok!", 14);
            
            if (size != 14) {
                printf("Can\'t write all string to pipe\n");
                exit(-1);
            }
            D(semid, 1);
        }
        close(parent[0]);
    }
    else {
        //Пул ребенка
        int counter = 0;
        // Читаем отца:
        for (int i = 0; i < N; ++i) {
            Z(semid);

            // Читаем сообщение присланное отцом
            size = read(parent[0], resstring, 14);
            if (size < 0) {
                printf("Can\'t read string from pipe\n");
                exit(-1);
            }
            printf("Сообщение №%d. Ребенок читает сообщение:%s\n", ++counter, resstring);

            // Пишем сообщение отцу
            size = write(parent[1], "Privet otec", 14);
            if (size != 14) {
                printf("Can\'t write all string to pipe: %d\n", size);
                exit(-1);
            }

            A(semid, 2);
        }

        close(parent[1]);
        close(parent[0]);
    }
    return 0;
}
