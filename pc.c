#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

int BUFFER_SIZE;
int *buffer;
int in = 0, out = 0;

sem_t empty, full;
pthread_mutex_t mutex;


void get_time(char *time_str) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(time_str, 20, "%H:%M:%S", t);
}


void write_status(const char *message) {
    FILE *fp = fopen("pc_status.txt", "a");

    if (fp == NULL) {
        printf("Error opening pc_status.txt\n");
        return;
    }

    fprintf(fp, "%s\n", message);
    fflush(fp);
    fclose(fp);
}


void write_buffer_log() {
    FILE *fp = fopen("buffer_log.txt", "w");

    if (fp == NULL) return;

    fprintf(fp, "BUFFER: ");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (i == in && i == out) fprintf(fp, "[IO]");
        else if (i == in) fprintf(fp, "[I ]");
        else if (i == out) fprintf(fp, "[ O]");
        else fprintf(fp, "[ %d ]", buffer[i]);
    }
    fprintf(fp, "\n");

    fclose(fp);
}


void* producer(void* arg) {
    int item = 1;

    while (1) {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;

        char time_str[20];
        get_time(time_str);

        char log_msg[100];
        sprintf(log_msg, "Producer produced item %d at %s", item, time_str);

        printf("%s\n", log_msg);
        fflush(stdout);

        write_status(log_msg);
        write_buffer_log();

        pthread_mutex_unlock(&mutex);
        sem_post(&full);

        item++;
        usleep(1000000);
    }
}


void* consumer(void* arg) {
    while (1) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        int item = buffer[out];
        buffer[out] = 0;
        out = (out + 1) % BUFFER_SIZE;

        char time_str[20];
        get_time(time_str);

        char log_msg[100];
        sprintf(log_msg, "Consumer consumed item %d at %s", item, time_str);

        printf("%s\n", log_msg);
        fflush(stdout);

        write_status(log_msg);
        write_buffer_log();

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);

        usleep(2000000);
    }
}

int main() {
    system("cd");
    pthread_t prod, cons;

    printf("Enter buffer size: ");
    scanf("%d", &BUFFER_SIZE);

    buffer = (int*)malloc(sizeof(int) * BUFFER_SIZE);

    for (int i = 0; i < BUFFER_SIZE; i++)
        buffer[i] = 0;

    
    FILE *fp1 = fopen("pc_status.txt", "w");
    if (fp1 != NULL) fclose(fp1);

    FILE *fp2 = fopen("buffer_log.txt", "w");
    if (fp2 != NULL) fclose(fp2);

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    return 0;
}