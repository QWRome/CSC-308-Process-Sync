#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4
#define ITERATIONS 1000000

// Shared counters
long counter_without_mutex = 0;
long counter_with_mutex = 0;

// Step 1: Initialize a pthread_mutex_t variable.
pthread_mutex_t my_mutex;

// Function where threads increment without protection (Will cause errors)
void *increment_without_mutex(void *arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        counter_without_mutex++; // Unprotected shared resource.
    }
    return NULL;
}

// Function where threads increment WITH protection (Safe)
void *increment_with_mutex(void *arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        // Step 3: Protect the counter with pthread_mutex_lock/unlock
        pthread_mutex_lock(&my_mutex);
        counter_with_mutex++; //Protected shared resource.
        pthread_mutex_unlock(&my_mutex);
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    long expected_total = NUM_THREADS * ITERATIONS;

    printf("Practical 1: Mutex Lock Demonstration.\n");
    printf("Expected final counter value: %ld\n\n", expected_total);

    // --- TEST 1: WITHOUT MUTEX ---
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, increment_without_mutex, NULL);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // Step 4 & 5: Compare results and observe incorrect values
    printf("Result WITHOUT Mutex: %ld\n", counter_without_mutex);
    if (counter_without_mutex < expected_total) {
        printf(" -> Observation: Data lost due to race conditions!\n\n");
    }

    // --- TEST 2: WITH MUTEX ---
    pthread_mutex_init(&my_mutex, NULL); // Initialize lock
    
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, increment_with_mutex, NULL);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    pthread_mutex_destroy(&my_mutex); // Clean up lock

    printf("Result WITH Mutex:    %ld\n", counter_with_mutex);
    if (counter_with_mutex == expected_total) {
        printf(" -> Observation: Mutual exclusion successful. No data lost.\n");
    }

    return 0;
}
