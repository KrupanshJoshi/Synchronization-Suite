#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int data = 0;
int readCount = 0;

sem_t mutex;
sem_t writeLock;

pthread_mutex_t file_mutex;

// 🔥 Safe file write
void write_status(char *type, int id, char *state)
{
    pthread_mutex_lock(&file_mutex);

    FILE *fp = fopen("rw_status.txt", "a");
    if (fp != NULL)
    {
        fprintf(fp, "%s %d %s\n", type, id, state);
        fclose(fp);
    }

    pthread_mutex_unlock(&file_mutex);
}

// 🔹 Reader
void *reader(void *arg)
{
    int id = *(int*)arg;

    while (1)
    {
        write_status("Reader", id, "WAITING");

        sem_wait(&mutex);
        readCount++;

        if (readCount == 1)
            sem_wait(&writeLock);

        sem_post(&mutex);

        write_status("Reader", id, "READING");

        printf("Reader %d reading: %d\n", id, data);
        sleep(1);

        sem_wait(&mutex);
        readCount--;

        if (readCount == 0)
            sem_post(&writeLock);

        sem_post(&mutex);

        // 🔥 IMPORTANT FIX
        write_status("Reader", id, "WAITING");

        sleep(1);
    }
}

// 🔹 Writer
void *writer(void *arg)
{
    int id = *(int*)arg;

    while (1)
    {
        write_status("Writer", id, "WAITING");

        sem_wait(&writeLock);

        data++;

        write_status("Writer", id, "WRITING");

        printf("Writer %d writing: %d\n", id, data);
        sleep(2);

        sem_post(&writeLock);

        // 🔥 CRITICAL FIX (this was missing)
        write_status("Writer", id, "WAITING");

        sleep(2);
    }
}

int main()
{
    int r, w;

    // clear file
    FILE *fp = fopen("rw_status.txt", "w");
    fclose(fp);

    printf("Enter number of readers: ");
    scanf("%d", &r);

    printf("Enter number of writers: ");
    scanf("%d", &w);

    pthread_t readers[r], writers[w];
    int rid[r], wid[w];

    sem_init(&mutex, 0, 1);
    sem_init(&writeLock, 0, 1);
    pthread_mutex_init(&file_mutex, NULL);

    for (int i = 0; i < r; i++)
    {
        rid[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &rid[i]);
    }

    for (int i = 0; i < w; i++)
    {
        wid[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &wid[i]);
    }

    for (int i = 0; i < r; i++)
        pthread_join(readers[i], NULL);

    for (int i = 0; i < w; i++)
        pthread_join(writers[i], NULL);

    return 0;
}