
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define NUM_ITEMS 10

// Shared Circular Buffer.
int buffer[BUFFER_SIZE];
int in = 0;  // Index for producer
int out = 0; // Index for consumer

// Semaphores defined in Step 4.
sem_t empty;
sem_t full;
pthread_mutex_t mutex;

// Step 2: Producer thread function.
void *producer(void *arg) {
    int item;
    for (int i = 0; i < NUM_ITEMS; i++) {
        item = rand() % 100; // Generate a random item
        
        sem_wait(&empty);    // Wait if buffer is full
        pthread_mutex_lock(&mutex); // Lock the buffer
        
        // Add item to buffer
        buffer[in] = item;
        printf("Producer generated item %d at index %d\n", item, in);
        in = (in + 1) % BUFFER_SIZE;
        
        pthread_mutex_unlock(&mutex); // Unlock the buffer
        sem_post(&full);     // Signal that buffer has new data
        
        sleep(1); // Simulate time taken to produce
    }
    return NULL;
}

// Step 3: Consumer thread function.
void *consumer(void *arg) {
    int item;
    for (int i = 0; i < NUM_ITEMS; i++) {
        sem_wait(&full);     // Wait if buffer is empty
        pthread_mutex_lock(&mutex); // Lock the buffer
        
        // Remove item from buffer
        item = buffer[out];
        printf("  Consumer consumed item %d from index %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;
        
        pthread_mutex_unlock(&mutex); // Unlock the buffer
        sem_post(&empty);    // Signal that buffer has empty space
        
        sleep(2); // Simulate time taken to consume (slower than producer)
    }
    return NULL;
}

int main() {
    pthread_t prod, cons;

    printf("---- Practical 2: Producer-Consumer Simulation ----\n");
    printf("Buffer Size: %d | Total Items: %d\n\n", BUFFER_SIZE, NUM_ITEMS);

    // Initialize Semaphores and Mutex.
    sem_init(&empty, 0, BUFFER_SIZE); // Initially, all slots are empty
    sem_init(&full, 0, 0);            // Initially, 0 slots are full
    pthread_mutex_init(&mutex, NULL);

    // Create threads
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    // Wait for threads to finish
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    // Clean up
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    printf("\nSimulation Complete.\n");
    return 0;
}
