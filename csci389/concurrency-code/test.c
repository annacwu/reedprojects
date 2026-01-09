#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "concurrency.h"

semaphore_t sem;
pthread_mutex_t mutex; 

// for semaphore test
void* sem_work(void* arg) {
    int id = *(int*)arg;

    printf("thread %d: acquiring\n", id);
    sem_acquire(&sem); 

    printf("thread %d: inside critical section!!\nopen spots: %d\n", id, *(sem.cas_loc));
    sleep(10);  // simulating some work

    printf("thread %d: releasing\nopen spots: %d\n", id, *(sem.cas_loc));
    sem_release(&sem); 

    return NULL;
}

void test_semaphore(){
   int cas_location;  
    // i was passing this in for testing but now i am assuming it is atomic
    // pthread_mutex_init(&mutex, NULL);

    // initialize semaphore to allow 3 threads at once
    sem_init(&sem, 3, &cas_location);

    pthread_t threads[10];
    int thread_ids[10];
    for (int i = 0; i < 10; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, sem_work, &thread_ids[i]);
    }
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    // pthread_mutex_destroy(&mutex);
}

// for dispatching tasks test
uint32_t times_two(uint32_t x) { 
    sleep(rand() % 10); // simulate variable task time
    return 2 * x; };

void test_dispatch() {
    uint32_t length = 10;
    uint32_t* inputs = (uint32_t*)malloc(length * sizeof(uint32_t));
    uint32_t* outputs = (uint32_t*)malloc(length * sizeof(uint32_t));

    printf("inputs: \n");
    for (uint32_t i = 0; i < 10; i++) {
        inputs[i] = i;
        printf("%u ", inputs[i]);
    }
    printf("\n");

    work_parallel(inputs, outputs, length, 2, times_two); 

    printf("outputs: \n");
    for (uint32_t i = 0; i < length; i++) {
        printf("%u ", outputs[i]);
    }
    printf("\n");

}

// for scheduling test

// 0 = network interface
// 1 = hard drive
// 2 = screen

// thread 1 is going to use 0 and 1
void* p1(void* arg) {
    int pid = *(int*)arg;

    while (1) {
        if (acquire(pid, 0) == 0) {
            if (acquire(pid, 1) == 0) {
                printf("p%d: using network and hard drive\n", pid);
                sleep(2); // simulating it taking some time

                printf("p%d releasing network\n", pid);
                release(pid, 0);
                printf("p%d releasing hard drive\n", pid);
                release(pid, 1);
                printf("p%d complete\n", pid);
                break;
            } else {
                release(pid, 0);
            }
        }
    }

    return NULL;
}

// thread 2 is going to use 1 and 2
void* p2(void* arg) {
    int pid = *(int*)arg;

    while (1) {
        if (acquire(pid, 1) == 0) {
            if (acquire(pid, 2) == 0) {
                printf("p%d: using hard drive and screen\n", pid);
                sleep(3); // simulating a slightly longer time 

                release(pid, 1);
                release(pid, 2);
                printf("p%d complete\n", pid);
                break;
            } else {
                release(pid, 1);
            }
        }
    }

    return NULL;
}

// thread 3 is going to use 2 and 0 
void* p3(void* arg) {
    int pid = *(int*)arg;

    while (1) {
        if (acquire(pid, 2) == 0) {
            if (acquire(pid, 0) == 0) {
                printf("p%d: using screen and network\n", pid);
                sleep(1); // simulate shorter time 

                release(pid, 2);
                release(pid, 0);
                printf("p%d complete\n", pid);
                break;
            } else {
                release(pid, 2);
            }
        }
    }

    return NULL;
}

void test_schedule() {
    controller_init();

    pthread_t threads[3];
    int thread_ids[3];

    thread_ids[0] = 1;
    pthread_create(&threads[0], NULL, p1, &thread_ids[0]);

    thread_ids[1] = 2;
    pthread_create(&threads[1], NULL, p2, &thread_ids[1]);

    thread_ids[2] = 3;
    pthread_create(&threads[2], NULL, p3, &thread_ids[2]);

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    controller_destroy();
}

int main() {
    test_semaphore();
    test_dispatch();
    test_schedule();
    return 0;
}