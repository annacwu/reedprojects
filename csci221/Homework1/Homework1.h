#ifndef HOMEWORK1_H
#define HOMEWORK1_H

// chose_mover() takes a positive integer 
// and returns a character
// arguments: 
// int boxes -- number of boxes to evaluate
// result: 
// char corresponding to cheaper option for boxes
char choose_mover(int);

// fibonacci() takes a nonnegative integer n
// and returns the nth number in the fibonacci sequence
// arguments: 
// int n -- the nth value to be computed
// result: 
// int representing the nth number in the sequence
int fibonacci(int);

// struct int_three represents a 3-bit binary number
// c is the 2^2 place
// b is 2^1 place
// a is 2^0 place
typedef struct int_threes {
    int c;
    int b;
    int a;
} int_three;

// print_three_unsigned() takes an int_three struct
// and prints its decimal equivalent
// arguments:
// struct int_three -- represents a 3-bit number
// assumed to be positive
// result: 
// printf statement with int decimal equivalent
void print_three_unsigned(int_three);

// print_three_signed() takes an int_three struct
// and prints its signed decimal equivalent
// arguments:
// struct int_three -- represents a 3-bit number
// pos or neg depending on 2s complement
// result: 
// printf statement with int decimal equivalent
void print_three_signed(int_three);

// tiny_add() takes two int_three structs
// and prints their binary sum
// arguments:
// struct int_three x -- represents a 3-bit number
// struct int_three y -- represents another 3-bit number
// result: 
// int_three struct sum of x and y
int_three tiny_add(int_three , int_three);

#endif