#ifndef CONCURRENCY_H
#define CONCURRENCY_H
#include <stdio.h>
#include <stdint.h>

typedef struct {
    // N threads allowed at once
    int count;
    // memory location of the value cas is checking
    int* cas_loc;
} semaphore_t;

void sem_init(semaphore_t* sem, int initial_count, int* loc);
void sem_acquire(semaphore_t* sem);
void sem_release(semaphore_t* sem);

void work_parallel(
uint32_t* inputs,
uint32_t* outputs,
uint32_t length,
uint32_t num_workers,
uint32_t (*function)(uint32_t));

void controller_init();
void controller_destroy();
uint32_t acquire(int pid, uint32_t resource);
void release(int pid, uint32_t resource);

#endif