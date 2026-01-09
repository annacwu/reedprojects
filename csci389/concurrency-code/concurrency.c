#include "concurrency.h"
#include "cas.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

// ------- COUNTING SEMAPHORE -----------

// sem is the address for the semaphore, 
// n is the number of threads allowed by the counting semaphore
// loc is the address where the cas lock is stored
void sem_init(semaphore_t* sem, int n, int* loc) {
    sem->count = n;
    // set the cas lock location to the max number of threads allowed
    *loc = sem->count;
    sem->cas_loc = loc;
};

// get current number of spots available, if it is 0 then keep waiting (all taken), otherwise decrement by one to indicate spot being taken
void sem_acquire(semaphore_t* sem) {
    // try to get into section with busy wait
    while (1) {
        int n = *(sem->cas_loc);
        if (n > 0){
            if (cas(sem->cas_loc, n, n - 1)) {
                return;
            }
        }
    };
};

// increment to indicate release as an open spot
void sem_release(semaphore_t* sem) {
    while (1) {
        int n = *(sem->cas_loc);
        if (cas(sem->cas_loc, n, n + 1)) {
            return;
        }
    }
};

// ------- DISPATCHING TASKS -----------

typedef struct {
    uint32_t* inputs;
    uint32_t* outputs;
    uint32_t length;
    uint32_t (*function)(uint32_t);
    uint32_t* index;        
    pthread_mutex_t* task_lock;        
} worker_args_t;

void* work_task(void *arg) {
    worker_args_t* args = (worker_args_t*)arg;

    // worker constantly working until break
    while (1){
        pthread_mutex_lock(args->task_lock);
        // if the next index i'm supposed to work on is past the end
        if (*(args->index) >= args->length) {
           pthread_mutex_unlock(args->task_lock); 
           // then break
            printf("thread %p done because no more tasks\n", pthread_self());
           break;
        }
        // otherwise store current index and increment the shared index to indicate i've taken it
        int curr_task = *(args->index);
        (*(args->index))++;
        // release the lock to let other people see that i've taken it
        pthread_mutex_unlock(args->task_lock);

        printf("thread %p doing index %d\n", pthread_self(), curr_task);
        // do the thing
        args->outputs[curr_task] = (*args->function)(args->inputs[curr_task]);
    };
    return NULL;
}

void work_parallel(
uint32_t* inputs,
uint32_t* outputs,
uint32_t length,
uint32_t num_workers,
uint32_t (*function)(uint32_t)) {
    // index for next task in array to be completed
    uint32_t i = 0;
    // mutex will be used to lock access to updating index
    pthread_mutex_t task_lock;
    pthread_mutex_init(&task_lock, NULL);

    worker_args_t args = {
        .inputs = inputs,
        .outputs = outputs,
        .length = length,
        .function = function,
        .index = &i,
        .task_lock = &task_lock
    };

    // make the threads 
    pthread_t threads[num_workers];
    for (int i = 0; i < num_workers; i++) {
        pthread_create(&threads[i], NULL, work_task, &args);
    }

    for (int i = 0; i < num_workers; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&task_lock);
};

// ------- SCHEDULING RESOURCES -----------
typedef struct {
    int resources[3]; // array of resources, each resource represented by index of array
    pthread_mutex_t lock;
} controller_t;

controller_t controller;
pthread_mutex_t mutex; 

// needed a way to make this from the testing file
void controller_init() {
    for (int i = 0; i < 3; i++) {
        // 0 means free, anything else should be a pid
        controller.resources[i] = 0;
    }
    pthread_mutex_init(&controller.lock, NULL);
};

void controller_destroy() {
    pthread_mutex_destroy(&controller.lock);
};


// where resource is the index into the resources array
uint32_t acquire(int pid, uint32_t resource) {
    // don't let other threads in while we check the resource
    pthread_mutex_lock(&controller.lock);
    // count how many resources the thread has right now
    int held = 0;
    for (int i = 0; i < 3; i++){
        // if a resource's index has a number other than 0, it is in use by that process
        if (controller.resources[i] == pid ) {
            held += 1;
        };
    };

    // if the controller is holding something and the requested resource is not free
    if (held > 0 && controller.resources[resource] != 0) {
        pthread_mutex_unlock(&controller.lock); 
        // broken the rules, because you're holding and waiting so return 1
        return 1;
    };

    // otherwise, either holding 0 resources so you can wait
    while (controller.resources[resource] != 0) {
        // release lock while waiting to allow other threads to release resources
        pthread_mutex_unlock(&controller.lock);
        sleep(1);
        // check again until it is available
        pthread_mutex_lock(&controller.lock);
    };

    // or the resource is available so you won't have to wait
    controller.resources[resource] = pid;
    pthread_mutex_unlock(&controller.lock);
    return 0; 
};

void release(int pid, uint32_t resource) {
    pthread_mutex_lock(&controller.lock); 
    // if the given process has the resource
    if (controller.resources[resource] == pid) {
        // free the resource
        controller.resources[resource] = 0;
    };
    pthread_mutex_unlock(&controller.lock);  
};