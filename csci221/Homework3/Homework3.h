#ifndef HOMEWORK3_H
#define HOMEWORK3_H
#include <stdint.h>

// union float_bits represents the float and binary versions of a number
// f is the float
// bits is the binary in terms of an integer, representing the 
// 1 sign bit, 8 exponent bits, and the 23 significand bits
union float_bits {
    float f; 
    uint32_t bits; 
};

// power2_floor() takes a float 
// and returns the closest power of 2 
// arguments: 
// float x -- number to be evaluated for closest power of 2
// result: 
// float isolating just the exponent bits of the input
float power2_floor(float);


// set_exp() takes a float and an integer
// and replaces the float's exponent with the integer
// arguments: 
// float x -- the number to be changed
// int new_exp -- the value of the exponent to be replaced
// result: 
// a float whos exponent is the inputted integer 
float set_exp(float, int);

// print_fixed_point() takes a 16 bit integer
// and prints it to the console as a fixed point
// arguments: 
// uint16_t x -- number to be evaluated like a fixed point
// result: 
// printed float based on input (evaluated as though 8 bits are negative powers of 2)
void print_fixed_pt(uint16_t);


// struct sfloat16 represents a simplified versin of a float
// exp is the exponent
// sig is the significand
typedef struct sfloat16s {
    int8_t exp;
    uint16_t sig;
} sfloat16;

// print_sfloat_dec() takes an sfloat16
// and prints its value to console
// arguments: 
// sfloat16 x -- the value to be printed
// result: 
// float version of an sfloat16 printed to console
void print_sfloat_dec(sfloat16);

// add_sfloat() takes two sfloat16s
// and adds them together to return their sum
// arguments: 
// sfloat16 x -- first value (greater than y)
// sfloat16 y -- second value (less than x)
// result: 
// sum of the two sfloats added through integer addition
sfloat16 add_sfloat(sfloat16, sfloat16);


// all of the following functions are from my personal
// c library, comprised of previous homework and lab functions

// this is adapted from the 16 bit print binary to print 32 bits
void print_binary(int32_t);
// from lab 2
int ith_bit(int32_t, int i);
// adapted from lab 2 to go through 16 bit integers
int ith_bit16(int16_t, int i);
// adapted to use 32 bit integers instead of 16 bit
int32_t replace_bit(int32_t, int32_t, int32_t);
// pow function from lab 0, adapted to take exponents of 0 
// and negative exponents 
float pow (int, int);

#endif