#include "Homework1.h"
#include <stdio.h> 

// computes which option will be least expensive
// via arithmetic operations
char choose_mover(int boxes) {
    int acost = 0; 
    int bcost = 0; 
    int atrips = 0;
    int btrips = 0; 
    char a = 'A';
    char b = 'B';
    if (boxes % 25 != 0){
        atrips += 1;
    } 
    atrips += (boxes/25);
    acost = atrips * 215;
    if (boxes % 31 != 0){
        btrips += 1;
    } 
    btrips += (boxes/31);
    bcost = btrips * 270;
    if (acost > bcost){
        return b;
    } else {
        return a;
    }
}

// computes the nth fibonacci number based on an inputted integer
// via for loop to compute the sequence up to that integer
int fibonacci(int n){
    int nth = 0;
    if (n >= 3) {
        int last = 1;
        int last2 = 1;
        int curr = 2;
        for (int i = 0; i < n - 2; i++){
            curr = last + last2;
            last2 = last;
            last = curr;
        } 
        nth = curr; 
        return nth;
    } else {
        return 1;
    }
}

// prints the decimal equivalent of an unsigned binary number from an inputted struct
// via multiplicaiton conversion methods
void print_three_unsigned(int_three x){
    int num = 0;
    num += x.a * 1;
    num += x.b * 2;
    num += x.c * 4;
    printf("Your number is: %d\n", num);
}

// prints the decimal equivalent of an signed binary number from an inputted struct
// via multiplicaiton conversion methods in which the 2^4 place is negative
void print_three_signed(int_three x){
    int num = 0;
    num += x.a * 1;
    num += x.b * 2;
    num += x.c * -4;
    printf("Your number is: %d\n", num);
}

// computes binary addition of two inputted structs holding 3-bit binary numbers
// through conditional bitwise operations
int_three tiny_add(int_three x, int_three y){
    int_three res;
    int carry = 0;

    if (x.a & y.a) {
        res.a = 0;
        carry = 1;
    } else {
        res.a = x.a | y.a;
    }

    if (carry == 0) {
        if (x.b & y.b) {
            res.b = 0;
            carry = 1;
        } else {
            res.b = x.b | y.b;
        }
    } else if (carry == 1) {
        res.b = 1;
        if (x.b & y.b) {
            carry = 1;
        } else if (x.b | y.b) {
            res.b = 0;
            carry = 1;
        } else {
            carry = 0;
        }
    }

    if (carry == 0) {
        if (x.c & y.c) {
            res.c = 0;
        } else {
            res.c = x.c | y.c;
        }
    } else if (carry == 1) {
        res.c = 1;
        if (x.c | y.c) {
            res.c = 0;
        }
    }

    return res;
}