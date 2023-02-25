#include <stdio.h>
#include <stdint.h>
#include "lab2.h"

int main() {
    printf("Expected result: 1\t Actual result: %d\n", ith_bit(1,0));
    printf("Expected result: 0\t Actual result: %d\n", ith_bit(4,1));
    printf("Expected result: 1\t Actual result: %d\n", ith_bit(32,5));
    print_binary(7);
    print_binary(32);
    print_binary(115);
    printf("Should return 4: %d\n",set_one(0,2));
    printf("Should return 4: %d\n",set_one(4,2));
    printf("Should return 7: %d\n",set_one(5,1));
    printf("Should return 0: %d\n",set_zero(4,2));
    printf("Should return 0: %d\n",set_zero(16,4));
    printf("Should return 0: %d\n",set_zero(64,6));
    printf("Should return 4: %d\n",replace_bit(0,2,1));
    printf("Should return 0: %d\n",replace_bit(16,4,0));

    return 0;
}