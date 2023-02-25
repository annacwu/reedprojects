#include "Homework2.h"
#include <stdint.h>
#include <stdio.h>

// computes the % function without arithmetic operators
// via bitwise operations
uint32_t mod32(uint32_t x){
    uint32_t mask = 0b11111;
    uint32_t mod = mask & x;
    return mod;
}

// computes the % function without arithmetic operators
// via bitwise operations
uint32_t mod16(uint32_t x){
    uint32_t mask = 0b1111;
    uint32_t mod = mask & x;
    return mod;
}

// computes the if/else statement without conditional statements
// via arithmetic operations
int cond(int x, int y, int z){
    int a = x != 0;
    int b = x == 0;
    return (a * y) + (b * z);
}

// switches the values of the ith and jth bits of a binary number
// via bitwise operations and shifting
uint16_t swap_bits(uint16_t x, uint16_t i, uint16_t j){
    // simple mask that will be used throughout program
    uint16_t m = 0b1;

    // gets the 1 or 0 value of the ith bit
    m = m << i;
    uint16_t im = m & x;
    uint16_t ith = im >> i;
    m = m >> i;
    //printf("%d\n", ith);


    // gets the 1 or 0 value of the jth bit
    m = m << j;
    uint16_t jm = m & x; 
    uint16_t jth = jm >> j;
    m = m >> j;
    //printf("%d\n", jth);

    uint16_t res = x;
    // changes the ith bit based on value of jth bit
    if (jth == 1){
        m = m << i;
        res = res | m;
        m = m >> i;
    } else {
        m = m << i;
        uint16_t oppm = ~m;
        res = res & oppm;
        m = m >> i;
       // print_binary(res);
    }
    
    // changes the jth bit based on value of ith bit
    if (ith == 1){
        m = m << j;
        res = res | m; 
        m = m >> j;
    } else {
        m = m << j;
        uint16_t oppm = ~m;
        res = res & oppm;
        m = m >> j;
    }
    return res;
}

// reverses the bits of a binary number within a specified range
// via loops and function calls
uint16_t reverse(uint16_t x, uint16_t i, uint16_t j){
    int reps = j - i;
    // printf("%d\n", reps);
    if (reps == 1){
        for (int n = 0; n < reps; n++){
            //print_binary(x);
            x = swap_bits(x,i,j);
            i += 1;
            j -= 1;
        }
    } else {
        for (int n = 0; n < reps - 1; n++){
        //print_binary(x);
        x = swap_bits(x,i,j);
        i += 1;
        j -= 1;
    }
    }
    //print_binary(x);
    return x;
   
}

// checks if a binary number is a palindrome
// via loops and function calls
int is_bin_pal(uint16_t x){
    int upper = 0;
    int lower = 0; 
    uint16_t x2 = x;

    // checks to see where the first 1 is in the number
    // and sets that place equal to the lower limit
    int ran = 0;
    for (int i = 0; i < 16; i++){
        if (ith_bit(x2,0)){
            lower = i;
            // printf("lower: %d\n", lower);
            break;
        }
        x2 =  x2 >> 1; 
        // print_binary(x2);
        ran += 1;
    }

    // checks to see where the last 1 is in the number
    // and sets that place equal to the upper limit
    for (int i = ran; i < 16; i++){
        x2 =  x2 >> 1; 
        // print_binary(x2);
        if (x2 == 0){
            upper = i;
            // printf("upper: %d\n", upper);
            break;
        }
    }

    // calls reverse to see if it is the same forward and back
    uint16_t check = reverse(x, lower, upper);
    if (check == x){
        return 1;
    } else {
        return 0;
    }
       
}

// helper functions from lab 2
void print_binary(int16_t x) {
    // printf("The binary representation of %d is: ", x);
    for(int i = 15; i >= 0; i--){
        if (ith_bit(x,i) == 1){
            printf("1");
        } else {
            printf("0");
        }
    }
    // printf("\n");
}
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


