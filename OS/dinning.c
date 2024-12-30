#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_PHILOSOPHERS 5 // number of philosophers
#define LEFT (id + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS // index of left neighbor
#define RIGHT (id + 1) % NUM_PHILOSOPHERS // index of right neighbor

sem_t chopsticks[NUM_PHILOSOPHERS]; // semaphores for chopsticks
pthread_t philosophers[NUM_PHILOSOPHERS]; // threads for philosophers

void *philosopher(void *arg) {
    int id = *(int *)arg;
    while (1) {
        printf("Philosopher %d is thinking\n", id);
        sleep(rand() % 3); // sleep for a random amount of time

        printf("Philosopher %d is hungry\n", id);
        sem_wait(&chopsticks[LEFT]); // acquire left chopstick
        printf("Philosopher %d picked up left chopstick %d\n", id, LEFT);
        sem_wait(&chopsticks[RIGHT]); // acquire right chopstick
        printf("Philosopher %d picked up right chopstick %d\n", id, RIGHT);

        printf("Philosopher %d is eating\n", id);
        sleep(rand() % 3); // sleep for a random amount of time

        sem_post(&chopsticks[RIGHT]); // release right chopstick
        printf("Philosopher %d put down right chopstick %d\n", id, RIGHT);
        sem_post(&chopsticks[LEFT]); // release left chopstick
        printf("Philosopher %d put down left chopstick %d\n", id, LEFT);
    }
    pthread_exit(NULL);
}

int main() {
    int i;
    int ids[NUM_PHILOSOPHERS];

    // initialize chopsticks
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_init(&chopsticks[i], 0, 1); // initialize each chopstick to 1
    }

    // create threads for philosophers
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    // wait for threads to finish
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // destroy semaphores
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_destroy(&chopsticks[i]);
    }

    return 0;
}
