#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int N;
pthread_mutex_t *forks;

pthread_mutex_t file_mutex;

float deadlock_prob = 0.5;
int iteration = 0;

int global_deadlock = 0;

void update_learning() {
    if (deadlock_prob > 0.05)
        deadlock_prob -= 0.01;
}

void* philosopher(void* num) {
    int id = *(int*)num;

    while (1) {

        pthread_mutex_lock(&file_mutex);
        FILE *f1 = fopen("status.txt", "a");
        fprintf(f1, "Process %d WAITING\n", id);
        fclose(f1);
        pthread_mutex_unlock(&file_mutex);

        usleep(150000);

       
        if (global_deadlock) {
            pthread_mutex_lock(&file_mutex);
            FILE *fd = fopen("status.txt", "a");
            fprintf(fd, "Process %d DEADLOCK\n", id);
            fclose(fd);
            pthread_mutex_unlock(&file_mutex);

            usleep(200000);
            continue;
        }

        
        int deadlock_mode = ((float)rand()/RAND_MAX < deadlock_prob);

        if (!deadlock_mode) {
            // SAFE execution
            if (id % 2 == 0) {
                pthread_mutex_lock(&forks[id]);
                pthread_mutex_lock(&forks[(id + 1) % N]);
            } else {
                pthread_mutex_lock(&forks[(id + 1) % N]);
                pthread_mutex_lock(&forks[id]);
            }
        } else {
            
            pthread_mutex_lock(&forks[id]);
            usleep(50000);
            pthread_mutex_lock(&forks[(id + 1) % N]);
        }

        
        pthread_mutex_lock(&file_mutex);
        FILE *f2 = fopen("status.txt", "a");
        fprintf(f2, "Process %d USING\n", id);
        fclose(f2);
        pthread_mutex_unlock(&file_mutex);

        usleep(150000);

        pthread_mutex_unlock(&forks[id]);
        pthread_mutex_unlock(&forks[(id + 1) % N]);

        usleep(100000);
    }
}


void* learning_thread(void* arg) {

    FILE *fp = fopen("rl_results.txt", "w");

    while (iteration < 50) {

        int deadlocks = 0;

        for (int i = 0; i < 50; i++) {
            if ((float)rand()/RAND_MAX < deadlock_prob)
                deadlocks++;
        }

        printf("Iteration %d -> Deadlocks: %d\n", iteration+1, deadlocks);
        fprintf(fp, "%d %d\n", iteration+1, deadlocks);
        fflush(fp);

      
        if (!global_deadlock && iteration > 10) {
            int trigger = rand() % 20;
            if (trigger == 0) {
                global_deadlock = 1;

                FILE *fd = fopen("status.txt", "a");
                fprintf(fd, "⚠ SYSTEM DEADLOCK OCCURRED ⚠\n");
                fclose(fd);
            }
        }

        update_learning();
        iteration++;

        sleep(1);
    }

    fclose(fp);
    return NULL;
}


int main() {
    srand(time(NULL));

    printf("Enter number of processes: ");
    scanf("%d", &N);

    printf("Running AI Dining Philosophers...\n");

    // clear files
    FILE *f = fopen("status.txt", "w"); fclose(f);
    FILE *f2 = fopen("rl_results.txt", "w"); fclose(f2);

    pthread_t *threads = malloc(N * sizeof(pthread_t));
    pthread_t learner;

    int *ids = malloc(N * sizeof(int));
    forks = malloc(N * sizeof(pthread_mutex_t));

    pthread_mutex_init(&file_mutex, NULL);

    for (int i = 0; i < N; i++)
        pthread_mutex_init(&forks[i], NULL);

    // philosopher threads
    for (int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, philosopher, &ids[i]);
    }

    // learning thread
    pthread_create(&learner, NULL, learning_thread, NULL);

    for (int i = 0; i < N; i++)
        pthread_join(threads[i], NULL); 

    pthread_join(learner, NULL);

    return 0;
}