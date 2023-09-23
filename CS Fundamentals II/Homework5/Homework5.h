#ifndef HOMEWORK5_H
#define HOMEWORK5_H
#include <stdint.h>

// maximum() takes a void pointer to an array and two integers
// and returns a pointer to the max value of the array
// arguments: 
// void* arr -- pointer to the array
// int size -- size of items in the array
// int num -- number of items in the array
// result: 
// pointer to max value in the array
void* maximum(void*, int, int);

// CmpInt() takes a two void pointers
// and returns either -1, 0, or 1
// arguments: 
// void* a_ptr -- pointer to the first value
// void* b_ptr -- pointer to the second value
// result: 
// -1 if a < b, 0 if a = b, 1 if a > b
int CmpInt(void*, void*);

// CmpFloat() takes a two void pointers
// and returns either -1, 0, or 1
// arguments: 
// void* a_ptr -- pointer to the first value
// void* b_ptr -- pointer to the second value
// result: 
// -1 if a < b, 0 if a = b, 1 if a > b
int CmpFloat(void*, void*);

// maximum() takes a void pointer to an array, two integers, and a function
// and returns a pointer to the max value of the array
// arguments: 
// void* arr -- pointer to the array
// int size -- size of items in the array
// int num -- number of items in the array
// int (*cmpfn) (void*, void*) -- comparison function (as seen above)
// result: 
// pointer to max value in the array
void* maximum_v2(void*, int, int, int (*cmpfn) (void*, void*));

// struct sCPU represents a CPU
// reg is an array that represents the register
// ram is an array that represents the RAM
typedef struct sCPUs {
    int32_t reg[32];
    int32_t ram[1024];
} sCPU;

// CPU_init() takes no arguments
// and returns a pointer to an sCPU
// no arguments
// result: 
// pointer to an empty sCPU
sCPU* CPU_init();

// CPU_destroy takes a pointer to an sCPU
// and deallocates the memory
// arguments: 
// sCPU* cpu -- sCPU to be deallocated
// result: 
// cpu free'd from the heap
void CPU_destroy(sCPU*);

// CPU_step takes 4 uint*_ts and one sCPU*
// and manipulates the sCPU based on the provided opcodes
// arguments: 
// uint8_t opcode -- opcode for instruction to be ran
// uint8_t arg1 -- depends on opcode, likely refers to a register
// uint8_t arg2 -- depends on opcode, likely refers to a register
// uint8_t arg3 -- depends on opcode, likely refers to a register
// sCPU* cpu -- sCPU to be manipulated
// result: 
// values manipulated in RAM and register of the sCPU
void CPU_step (uint8_t, uint8_t,  uint8_t,  uint8_t, sCPU*);

#endif