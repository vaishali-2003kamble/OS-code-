#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 5 // size of buffer

sem_t mutex, empty, full; // semaphores for synchronization
int buffer[BUFFER_SIZE]; // buffer for storing produced items
int in = 0, out = 0; // indices for inserting and removing items from buffer

void *producer(void *arg) {
    int item;
    while (1) {
        item = rand() % 100; // generate a random item
        sem_wait(&empty); // wait until buffer is not full
        sem_wait(&mutex); // acquire mutex lock
        buffer[in] = item; // insert item into buffer
        in = (in + 1) % BUFFER_SIZE; // update index for next insertion
        printf("Producer produced item %d\n", item);
        sem_post(&mutex); // release mutex lock
        sem_post(&full); // increment number of items in buffer
        sleep(rand() % 3); // sleep for a random amount of time
    }
    pthread_exit(NULL);
}

void *consumer(void *arg) {
    int item;
    while (1) {
        sem_wait(&full); // wait until buffer is not empty
        sem_wait(&mutex); // acquire mutex lock
        item = buffer[out]; // remove item from buffer
        out = (out + 1) % BUFFER_SIZE; // update index for next removal
        printf("Consumer consumed item %d\n", item);
        sem_post(&mutex); // release mutex lock
        sem_post(&empty); // increment number of empty slots in buffer
        sleep(rand() % 3); // sleep for a random amount of time
    }
    pthread_exit(NULL);
}

int main() {
    int num_producers, num_consumers;
    printf("Enter the number of producers: ");
    scanf("%d", &num_producers);
    printf("Enter the number of consumers: ");
    scanf("%d", &num_consumers);

    sem_init(&mutex, 0, 1); // initialize mutex semaphore to 1
    sem_init(&empty, 0, BUFFER_SIZE); // initialize empty semaphore to buffer size
    sem_init(&full, 0, 0); // initialize full semaphore to 0

    pthread_t producers[num_producers], consumers[num_consumers];
    int i;

    for (i = 0; i < num_producers; i++) {
        pthread_create(&producers[i], NULL, producer, NULL);
    }

    for (i = 0; i < num_consumers; i++) {
        pthread_create(&consumers[i], NULL, consumer, NULL);
    }

    for (i = 0; i < num_producers; i++) {
        pthread_join(producers[i], NULL);
    }

    for (i = 0; i < num_consumers; i++) {
        pthread_join(consumers[i], NULL);
    }

    sem_destroy(&mutex); // destroy mutex semaphore
    sem_destroy(&empty); // destroy empty semaphore
    sem_destroy(&full); // destroy full semaphore

    return 0;
}
