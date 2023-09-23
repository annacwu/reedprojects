#ifndef HOMEWORK4_H
#define HOMEWORK4_H
#include <stdint.h>

// print_uint8_arr() takes a uint8_t array and and integer
// and prints the values of the array
// arguments: 
// uint8_t* arr -- array to be printed
// int len -- length of array
// result: 
// formatted array printed to console
void print_uint8_arr (uint8_t*, int);

// print_uint8_arr_v2() takes a uint8_t array and and integer
// and prints the values of the array
// arguments: 
// uint8_t* arr -- array to be printed
// int len -- length of array
// result: 
// formatted array printed to console
void print_uint8_arr_v2 (uint8_t*, int);

// print_uint16_arr() takes a uint16_t array and and integer
// and prints the values of the array
// arguments: 
// uint16_t* arr -- array to be printed
// int len -- length of array
// result: 
// formatted array printed to console
void print_uint16_arr (uint16_t*, int);

// surgical_mult4() takes a float pointer
// and changes its value to be 4 times greater
// arguments: 
// float* fp -- value to be changed
// result: 
// fp x 4, not returned
void surgical_mult4(float*);

// find_substr() takes two char pointers (strings)
// and determines if the second is a substring of the first
// arguments: 
// char* str1 -- first string to be evaluated
// char* str2 -- potential substring
// result: 
// pointer to the first address in the string that they share 
// if it is a substring
char* find_substr(char*, char*);

// print_board() takes an array of arrays/pointer to a pointer of characters
// and an integer
// and prints its visual representation to console
// arguments: 
// char** board -- array of arrays holding the characters
// int n -- size of each array
void print_board(char**, int);

// move_piece() takes an array of arrays/pointer to a pointer of characters,
// an integer, and a character
// and moves the piece on the board in the direction specified by the character
// arguments: 
// char** -- array of arrays holding the characters
// int size -- size of each array
// char cd -- cardination direction for the piece to be moved
// result: 
// updated board with new piece location
void move_piece(char**, int, char);

// struct node represents a node on a stack
// val is the value of the node
// next is the link to the next node
typedef struct nodes{
    uint32_t val;
    void* next;
} node;

// struct stack represents a stack
// top is the first/most recent node on the stack
// numnodes is the number of nodes on the stack
typedef struct stacks{
    node* top;
    int numnodes;
} stack; 

// push() takes a stack pointer and a uint32_t
// and adds a node to the top of the stack with the integer value
// arguments: 
// stack* stack -- address of stack to be altered
// uint32_t x -- value of new node
// result: 
// altered stack with new node on top
void push(stack*, uint32_t);

// print_stack() takes a stack pointer
// and prints the stack
// arguments: 
// stack* stack -- address of stack to be printed
// result: 
// stack printed to console in order of top to bottom
void print_stack(stack*);

// pop() takes a stack pointer
// and removes the top value of that stack
// arguments: 
// stack* stack -- stack to be popped
// result: 
// value of the popped node
int pop(stack*);

// free_stack takes a stack pointer
// and deallocates it from memory along with all its nodes
// arguments: 
// stack* stack -- stack to be freed
// result: 
// all memory taken by stack and its nodes deallocated
void free_stack(stack*);

#endif