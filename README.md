# CSC-308: Process Synchronization Practicals.
This repository contains the source code for the Week 6 Process Synchronization and Inter-Process Communication (IPC) laboratory exercises. 

## Author
**Name: Ekwe Roselinemaria Adaku.

Registration Number: 2023514039.

Department: Computer Science**.

## Project Overview
These programs demonstrate various methods of handling concurrency, synchronization, and shared memory in a Linux environment using C and the `pthread` and `sem.h` libraries.

## Practical Exercises

1. **Practical 1: Mutex Lock Demonstration**
   - Demonstrates the necessity of mutual exclusion in multi-threaded programs to prevent race conditions.
   - Compares unprotected increments vs. mutex-protected increments.

2. **Practical 2: Producer-Consumer Simulation**
   - Implements a bounded-buffer solution using semaphores.
   - Simulates synchronization between a producer thread and a consumer thread.

3. **Practical 3: Mutex vs. Counting Semaphore**
   - Compares the performance and behavior of binary mutexes versus counting semaphores.
   - Demonstrates resource pooling allowing multiple threads to enter a critical section simultaneously.

4. **Practical 4: Shared Memory Programming**
   - Utilizes System V IPC to establish shared memory between a parent and child process.
   - Uses semaphores to ensure the child process waits for the parent to finish writing before reading.

## Environment.
OS: Ubuntu on Windows Subsystem for Linux (WSL)

Compiler: GCC

Language: C

## How to Compile and Run.
All programs require the `pthread` library. Use `gcc` to compile:

```bash
# Example for Practical 1:
gcc -pthread -o practical1 practical1_mutex.c
./practical1
