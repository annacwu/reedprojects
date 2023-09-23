#ifndef HOMEWORK6_H
#define HOMEWORK6_H
#include <stdint.h>

// struct iCPU represents a CPU
// reg is an array that represents the register
// ram is an array that represents the RAM
// pc is an int that represents a program counter
// prog is an array that represents the stored program instructions
typedef struct iCPUs {
    int32_t reg[32];
    int32_t ram[1024];
    int pc;
    uint32_t prog[32];
} iCPU;

// iCPU_init() takes no arguments
// and returns a pointer to an iCPU
// no arguments
// result: 
// pointer to an empty iCPU
iCPU* iCPU_init();

// CPU_destroy takes a pointer to an iCPU
// and deallocates the memory
// arguments: 
// iCPU* cpu -- iCPU to be deallocated
// result: 
// cpu free'd from the heap
void iCPU_destroy(iCPU*);

// CPU_step takes 4 uint*_ts and one iCPU*
// and manipulates the iCPU based on the provided opcodes
// arguments: 
// uint8_t opcode -- opcode for instruction to be ran
// uint8_t arg1 -- depends on opcode, likely refers to a register
// uint8_t arg2 -- depends on opcode, likely refers to a register
// uint8_t arg3 -- depends on opcode, likely refers to a register
// iCPU* cpu -- sCPU to be manipulated
// result: 
// values manipulated in RAM and register and pc of the iCPU
void iCPU_step (uint8_t, uint8_t,  uint8_t,  uint8_t, iCPU*);

// read_instruction takes a uint32_t and one iCPU*
// and interprets the uint32_t as a program instruction
// arguments: 
// uint32_t x -- instruction to be broken into 4 chunks for interpretation
// iCPU* -- iCPU that houses the instructions
// result: 
// calls iCPU_step with the interpreted instructions
void read_instruction(uint32_t, iCPU*);

// write_instruction takes 4 uint8_ts, an int, and one iCPU*
// and turns them into one 32 bit instruction for read_instruction
// arguments: 
// uint8_t opcode -- opcode for instruction to be ran
// uint8_t arg1 -- depends on opcode, likely refers to a register
// uint8_t arg2 -- depends on opcode, likely refers to a register
// uint8_t arg3 -- depends on opcode, likely refers to a register
// int lineNum -- line of the program that it will be run on
// iCPU* cpu -- iCPU to store the instruction
// result:
// stores the instruction in the prog of the iCPU
void write_instruction (uint8_t, uint8_t, uint8_t, uint8_t, int, iCPU*);

// clock_cycle takes an *iCPU
// and calls read_instruction once using its prog and pc
// arguments: 
// iCPU* cpu -- iCPU to be executed
// result:
// certain instruction interpreted by read_instruction
void clock_cycle(iCPU*);

// run_program takes an *iCPU
// and executes its prog
// arguments: 
// iCPU* cpu -- iCPU to be executed
// result:
// all of the instructions in prog are executed
void run_program(iCPU*);

#endif