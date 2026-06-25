#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h> // For O_* constants

#define SHM_SIZE 1024 // Size of shared memory segment

int main() {
    printf("--- Practical 4: Shared Memory Programming ---\n\n");

    // 1. Create a key for the shared memory segment
    key_t key = ftok("shmfile", 65);
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    // Step 1: Use shmget() to create a shared memory segment
    // IPC_CREAT creates it, 0666 gives read/write permissions
    int shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // Step 4 Preparation: Use named semaphores to synchronize access
    // Named semaphores work better across separate processes than unnamed ones
    sem_t *sem = sem_open("/my_semaphore", O_CREAT, 0666, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    // Fork to create a separate child process
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    } 
    else if (pid > 0) {
        // -----------------------------
        // PARENT PROCESS (The Writer)
        // -----------------------------
        
        // Step 2: Use shmat() to attach memory in parent process
        char *str = (char*) shmat(shmid, (void*)0, 0);
        if (str == (char *)(-1)) {
            perror("shmat parent");
            exit(1);
        }

        // Step 3: Write data from one process
        printf("[Parent Process] Writing to shared memory...\n");
        strcpy(str, "Hello from the Parent Process! Shared Memory is working.");
        printf("[Parent Process] Data written: \"%s\"\n", str);

        // Signal the child that data is ready to be read
        sem_post(sem); 

        // Wait for child to finish reading
        wait(NULL);

        // Step 5: Clean up with shmdt() (detach)
        shmdt(str);
        
        // Step 5: Clean up with shmctl(IPC_RMID) (destroy)
        shmctl(shmid, IPC_RMID, NULL);
        
        // Clean up semaphore
        sem_close(sem);
        sem_unlink("/my_semaphore");
        
        printf("\n[Parent Process] Shared memory and semaphores cleaned up. Exiting.\n");
    } 
    else {
        // -----------------------------
        // CHILD PROCESS (The Reader)
        // -----------------------------
        
        // Wait for the parent to finish writing before trying to read
        sem_wait(sem);
        
        // Step 2: Use shmat() to attach memory in child process
        char *str = (char*) shmat(shmid, (void*)0, 0);
        if (str == (char *)(-1)) {
            perror("shmat child");
            exit(1);
        }

        // Step 3: Read from another (Child reads what parent wrote)
        printf("\n[Child Process]  Reading from shared memory...\n");
        printf("[Child Process]  Data read: \"%s\"\n", str);

        // Step 5: Clean up with shmdt() (detach)
        shmdt(str);
        
        // Close semaphore in child
        sem_close(sem);
    }

    return 0;
}
