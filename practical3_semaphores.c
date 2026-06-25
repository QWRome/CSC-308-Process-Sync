#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/time.h>

#define NUM_THREADS 6

// Synchronization tools.
pthread_mutex_t my_mutex;
sem_t counting_semaphore;

// Shared resource simulation.
void use_resource(int thread_id, const char* sync_type) {
    // We simulate work by having the thread sleep for 1 second.
    // This is like querying a database or using a printer.
    printf("Thread %d entering resource using %s\n", thread_id, sync_type);
    sleep(1); 
    printf("Thread %d leaving resource using %s\n", thread_id, sync_type);
}

// MUTEX APPROACH (1 Thread at a time).
void *mutex_task(void *arg) {
    int thread_id = *(int *)arg;
    
    pthread_mutex_lock(&my_mutex);     // ONLY 1 thread gets past this point
    use_resource(thread_id, "MUTEX");
    pthread_mutex_unlock(&my_mutex);   // Let the next thread in
    
    return NULL;
}

// SEMAPHORE APPROACH (N Threads at a time).
void *semaphore_task(void *arg) {
    int thread_id = *(int *)arg;
    
    sem_wait(&counting_semaphore);     // Up to N threads get past this point
    use_resource(thread_id, "SEMAPHORE");
    sem_post(&counting_semaphore);     // Let the next thread in
    
    return NULL;
}

// Helper to calculate time
double get_time() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return (t.tv_sec) + (t.tv_usec / 1000000.0);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    double start_time, end_time;

    printf(" ---- Practical 3: Mutex vs. Counting Semaphore. ---- \n\n");

    // Initialize synchronization tools.
    pthread_mutex_init(&my_mutex, NULL);
    // Initialize semaphore to allow exactly 4 threads simultaneously.
    sem_init(&counting_semaphore, 0, 4); 

    // TEST 1: STRICT MUTEX.
    printf("Starting Mutex Test (1 Thread at a time)...\n");
    start_time = get_time();
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, mutex_task, &thread_ids[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    end_time = get_time();
    printf("Mutex Total Time: %.2f seconds.\n", end_time - start_time);

    // TEST 2: COUNTING SEMAPHORE.
    printf("\nStarting Semaphore Test (4 Threads simultaneously)...\n");
    start_time = get_time();
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, semaphore_task, &thread_ids[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    end_time = get_time();
    printf("Semaphore Total Time: %.2f seconds.", end_time - start_time);

    // Clean up
    pthread_mutex_destroy(&my_mutex);
    sem_destroy(&counting_semaphore);
    
    // Answering the slide's discussion question:
    printf("\n\nDiscussion Conclusion:\n");
    printf("The Mutex strictly serialized the threads, causing a bottleneck.\n");
    printf("The Counting Semaphore allowed a 'pool' of resources (4 at once), ");
    printf("which significantly improved throughput while still maintaining control.\n");
    printf("Therefore, a counting semaphore is used instead of a mutex when managing a pool of identical resources ");
    printf("rather than protecting a single piece of shared data.\n");
    return 0;
}
