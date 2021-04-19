#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/sem.h>

int V(int semid, struct sembuf* buf) {
    buf->sem_op = 1;
    buf->sem_flg = 0;
    buf->sem_num = 0;

    return semop(semid, buf, 1);
}

int P(int semid, struct sembuf* buf) {
    buf->sem_op = -1;
    buf->sem_flg = 0;
    buf->sem_num = 0;

    return semop(semid, buf, 1);
}
/**
 * Если семафора нет, его надо создать, после создания
 * Начальное значение семафора-0 
 * Функция V увеличивает значение семафора на 1, функция P уменьшает его значение на 1
 * Если значение < 0, то либо родитель либо ребенок ждут
 */
int main()
{
    int parent[2], result;
    size_t size = 0;
    key_t key;
    char pathname[] = "task1.c";
    struct sembuf buffer;

    int semid;
    char resstring[14];

    // пытаемся создать pipe
    if (pipe(parent) < 0) {
        printf("Can\'t create pipe\n");
        exit(-1);
    }
    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    // Если семафора нет-создаем его, если есть, берем его
    if ((semid = semget(key, 1, 0666)) < 0) {
        printf("Semaphore not found. Trying to create...\n");
        if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
            printf("Can\'t get semid\n");
            exit(-1);
        }
        printf("Create successful!\n");
    }


    result = fork();

    if (result < 0) {
        printf("Can\'t fork child\n");
        exit(-1);
    }
    // Часть отца:
    else if (result > 0) {
        int N;
        printf("введите натуральное число n большее или равное двум:\n");
        scanf("%d", &N);
        if (N < 2) {
            printf("n должен быть больше или равен 2.\n");
            exit(-1);
        }

        for (size_t i = 0; i < N; i++)
        {
             // Отправка сообщения ребенку
            if (write(parent[1], "For my son!", 14) != 14) {
                printf("Can\'t write all string\n\r");
                exit(-1);
            }

            printf("Отец %d отправил рёбенку смс.\n\r", i + 1);
	    
            // Открываем семафор на чтение для ребенка и отправляем отца дожидаться 
            V(semid, &buffer);
            P(semid, &buffer);
             // Читаем сообщение пришедшее от ребенка отцу
            size = read(parent[0], resstring, 14);

            if (size != 14) {
                printf("Can\'t read from child\n\r");
                exit(-1);
            }

            printf("Отец получил сообщение от ребенка: %s\n\r", resstring);
        }
        close(parent[0]);
    }
    else {
        // Часть ребёнка
        int counter = 0;
        // Здесь происходит чтение сообщений отца
        while(1){
            // Ждем пока отец отправит ребенку смс 
            P(semid, &buffer);
            size = read(parent[0], resstring, 14);

            if (size < 0) {
                close(parent[1]);
                close(parent[0]);

                printf("Конец\n");  
                return 0;
            }

            // Текст смс пришедший ребенку от отца
            printf("Ребенок %d получил смс от отца: %s\n\r", ++counter, resstring);

            // Отправляем отцу ответное сообщение
            if (write(parent[1], "For my father!", 14) != 14) {
                printf("Can\'t write all string\n\r");
                exit(-1);
            }
            // Увеличиваем значение семафора на 1 тем самым разрешая отцу читать письмо ребенка
            V(semid, &buffer);
        }
    }
    return 0;
}
