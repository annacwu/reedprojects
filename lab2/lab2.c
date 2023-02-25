#include "lab2.h"
#include <stdint.h>
#include <stdio.h>

int ith_bit(int16_t x, int i) {
    int16_t m = 0b0000000000000001;
    m = m << i; 
    int16_t res = x & m;
    res = res >> i;
    if (res == 1){
        return 1;
    } else {
        return 0;
    }
}

void print_binary(int16_t x) {
    printf("The binary representation of %d is: ", x);
    for(int i = 15; i >= 0; i--){
        if (ith_bit(x,i) == 1){
            printf("1");
        } else {
            printf("0");
        }
    }
    printf("\n");
}

int16_t set_zero(int16_t x, int16_t i) {
    int16_t m = 0b0000000000000001;
    m = m << i;
    int16_t opp = ~m;
    return x & opp;
}

int16_t set_one(int16_t x, int16_t i) {
    int16_t m = 0b0000000000000001;
    m = m << i;
    return x | m;
}

int16_t replace_bit(int16_t x, int16_t i, int16_t b){
    int16_t m = 0b0000000000000001;
    m = m << i;
    if (b == 1){
        return x | m;
    } else {
        int16_t opp = ~m;
        return x & opp;  
    }
}

int16_t replace_bit(int16_t x, int16_t i, int16_t j, int16_t b){
    
}