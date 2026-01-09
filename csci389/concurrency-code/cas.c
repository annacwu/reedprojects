#include <pthread.h>

// for testing, I made this atomic with pthread and took a mutex as an extra parameter, 
// but I'm removing it and assuming everything will be done atomically when you are testing it 
int cas(int* loc, int old, int new_val) {
    // pthread_mutex_lock(mut);
    if (*loc == old) {
        *loc = new_val;
        // pthread_mutex_unlock(mut);
        return 1;
    }
    // pthread_mutex_unlock(mut);
    return 0; 
}

