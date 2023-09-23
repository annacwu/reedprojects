#include <stdio.h>
#include <stdlib.h>
#include "Homework1.h"

// computes nth fibonacci number using fibonacci() from Homework1.c
// but recieves input from 2nd command line argument
int main(int argc, char *argv[]) {
    int x = atoi(argv[1]);
    if (x && x >= 0) {
        printf("The fibonacci number in spot %d of the sequence is: %d.\n", x, fibonacci(x));
    } else {
        printf("Please enter a valid integer.\n");
    }
    

    return 0;
}