#ifndef HOMEWORK2_H
#define HOMEWORK2_H
#include <stdint.h>

// mod32() takes an unsigned 32 bit integer
// and returns an unsigned 32 bit integer
// arguments: 
// uint32_t x -- number to be evaluated 
// result: 
// uint32_t that represents the x mod 32 of x
uint32_t mod32(uint32_t x);

// mod16() takes an unsigned 16 bit integer
// and returns an unsigned 16 bit integer
// arguments: 
// uint16_t x -- number to be evaluated 
// result: 
// uint16_t that represents the x mod 16 of x
uint32_t mod16(uint32_t x);

// cond() takes three integers
// and returns an integer
// arguments: 
// int x -- number to be evaluated for the condition
// int y -- number to be returned if x is true
// int z -- number to be returned if x is false
// result: 
// int that is either the value of y or z
int cond(int, int, int);

// swap_bits() takes three unsigned 16 bit integers
// and returns an unsigned 16 bit integer
// arguments: 
// uint16_t x -- number to be changed
// uint16_t i -- the first bit (0-15) to be switched with j
// uint16_t j -- the second bit (0-15) to be switched with i
// result: 
// uint16_t that is x but with i and j bits switched
uint16_t swap_bits(uint16_t x, uint16_t i, uint16_t j);

// swap_bits() takes three unsigned 16 bit integers
// and returns an unsigned 16 bit integer
// arguments: 
// uint16_t x -- number to be changed
// uint16_t i -- the lower limit (0-15) of the reversal
// uint16_t j -- the upper limit (0-15) of the reversal
// result: 
// uint16_t with the lower-upper range of bits reversed
uint16_t reverse(uint16_t x, uint16_t i, uint16_t j);

// is_bin_pal() takes an unsigned 16 bit integer
// and returns an integer
// arguments: 
// uint16_t x -- number to be evaluated for a binary palindrome
// result: 
// int that is either 1 if x is a palindrome, or 0 if it is not
int is_bin_pal(uint16_t x);


// both of the following functions were from lab 2 
// and they are used as helper functions for the 3 preceeding functions
void print_binary(int16_t);
int ith_bit(int16_t, int);

#endif