#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int N;
pthread_mutex_t *forks;

int *individual_cycles;   // track per philosopher
int deadlock_triggered = 0;

pthread_mutex_t count_mutex;

void* philosopher(void* num) {
    int id = *(int*)num;

    while (1) {

        if (deadlock_triggered) {
            while (1);   // freeze system
        }

        FILE *file1 = fopen("status.txt", "a");
        fprintf(file1, "Process %d WAITING\n", id);
        fclose(file1);

        sleep(rand()%2 + 1);

        // NORMAL SAFE EXECUTION
        if (!deadlock_triggered) {
            if (id % 2 == 0) {
                pthread_mutex_lock(&forks[id]);
                pthread_mutex_lock(&forks[(id + 1) % N]);
            } else {
                pthread_mutex_lock(&forks[(id + 1) % N]);
                pthread_mutex_lock(&forks[id]);
            }
        }

        FILE *file2 = fopen("status.txt", "a");
        fprintf(file2, "Process %d USING\n", id);
        fclose(file2);

        sleep(rand()%2 + 1);

        pthread_mutex_unlock(&forks[id]);
        pthread_mutex_unlock(&forks[(id + 1) % N]);

        // ✅ INCREMENT ONLY THIS PHILOSOPHER'S CYCLE
        pthread_mutex_lock(&count_mutex);
        individual_cycles[id]++;

        // ✅ CHECK: have ALL completed ≥6 cycles?
        int all_done = 1;
        for (int i = 0; i < N; i++) {
            if (individual_cycles[i] < 4) {
                all_done = 0;
                break;
            }
        }

        if (!deadlock_triggered && all_done) {
            deadlock_triggered = 1;

            FILE *file_dead = fopen("status.txt", "a");
            fprintf(file_dead, "DEADLOCK OCCURRED\n");
            fclose(file_dead);
        }
        pthread_mutex_unlock(&count_mutex);

        // 🔴 DEADLOCK PHASE (all enter together)
        if (deadlock_triggered) {
            pthread_mutex_lock(&forks[id]);

            FILE *file_dead = fopen("status.txt", "a");
            fprintf(file_dead, "Process %d DEADLOCK\n", id);
            fclose(file_dead);

            while (1);   // freeze
        }
    }
}

int main() {
    srand(time(NULL));
    printf("Enter number of processes: ");
    scanf("%d", &N);

    pthread_t *thread = malloc(N * sizeof(pthread_t));
    int *phil = malloc(N * sizeof(int));
    forks = malloc(N * sizeof(pthread_mutex_t));
    individual_cycles = calloc(N, sizeof(int));

    pthread_mutex_init(&count_mutex, NULL);

    for (int i = 0; i < N; i++)
        pthread_mutex_init(&forks[i], NULL);

    for (int i = 0; i < N; i++) {
        phil[i] = i;
        pthread_create(&thread[i], NULL, philosopher, &phil[i]);
    }

    for (int i = 0; i < N; i++)
        pthread_join(thread[i], NULL);

    return 0;
}