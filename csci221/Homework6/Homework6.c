#include "Homework6.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// returns a pointer to an iCPU with 0's in the ram, reg, prog, and pc
// via loopos, malloc, and pointer syntax
iCPU* iCPU_init(){
    iCPU* newCPU = (iCPU*) malloc(sizeof(iCPU));
    for (int i = 0; i < 1024; i++){
        newCPU -> ram[i] = 0;
    }
    for (int i = 0; i < 32; i++){
        newCPU -> reg[i] = 0;
    }
    newCPU -> pc = 0;
    for (int i = 0; i < 32; i++){
        newCPU -> prog[i] = 0;
    }
    return newCPU;
}

// deallocates an iCPU from memory
// via free command
void iCPU_destroy(iCPU* cpu){
    free(cpu);
}

// manipulates the values in the ram, reg, and pc of an iCPU
// via inputted opcodes, pointer syntax, and conditional statements
void iCPU_step(uint8_t opcode, uint8_t arg1,  uint8_t arg2,  uint8_t arg3, iCPU* cpu){
    if (opcode == 0){ // print r0
        printf("%d\n", cpu -> reg[0]);
        cpu -> pc += 1;
    } else if (opcode == 1){ // add
        uint32_t x = 0b0;
        uint32_t a = arg2 << 8;
        uint32_t b = arg3;
        x = x | a;
        x = x | b;
        cpu -> reg[arg1] = x;
        cpu -> pc += 1;
    } else if (opcode == 2){ 
        int sum = cpu -> reg[arg2] + cpu -> reg[arg3];
        cpu -> reg[arg1] = sum;
        cpu -> pc += 1;
    } else if (opcode == 5){ // mul
        int prod = cpu -> reg[arg2] * cpu -> reg[arg3];
        cpu -> reg[arg1] = prod;
        cpu -> pc += 1;
    } else if (opcode == 3){ 
        cpu -> ram[cpu -> reg[arg2]] = cpu -> reg[arg1];
        cpu -> pc += 1;
    } else if (opcode == 4){
        cpu -> reg[arg1] = cpu -> ram[cpu -> reg[arg2]];
        cpu -> pc += 1;
    } else if (opcode == 6){ // jump
        cpu -> pc = arg3;
    } else if (opcode == 7){ // beq
        if (cpu-> reg[arg1] == cpu -> reg[arg2]){
            cpu -> pc = arg3;
        } else{
            cpu -> pc += 1;
        }
    } else if (opcode == 8){ // bne
        if (cpu-> reg[arg1] == cpu -> reg[arg2]){
            cpu -> pc += 1;
        } else{
            cpu -> pc = arg3;
        }
    } else if (opcode == 9){ // terminate
        cpu -> pc = 32;
    }
}

// reads an inputted iCPU instruction
// via casting and dereferencing a uint32_t
void read_instruction(uint32_t x, iCPU* c){
    uint8_t* casted_x = (uint8_t*) &x;

    uint8_t opcode = *(casted_x + 3);
    uint8_t arg1 = *(casted_x + 2);
    uint8_t arg2 = *(casted_x + 1);
    uint8_t arg3 = *(casted_x);

    iCPU_step(opcode, arg1, arg2, arg3, c);
}

// writes an iCPU instruction
// via shifting and storing the instruction in the iCPU prog
void write_instruction (uint8_t opcode, uint8_t arg1, uint8_t arg2, uint8_t arg3, int lineNum, iCPU* cpu){
    uint32_t instruction = 0;
    instruction = opcode;
    instruction = instruction << 8;
    instruction += arg1;
    instruction = instruction << 8;
    instruction += arg2;
    instruction = instruction << 8;
    instruction += arg3;
    cpu -> prog[lineNum] = instruction;
}

// simulates one clock cycle of a cpu
// by calling the read_instruction function
void clock_cycle(iCPU* cpu){
    read_instruction(cpu -> prog[cpu -> pc], cpu);
}

// runs the stored program in the prog of an iCPU
// via a while loop
void run_program(iCPU* cpu){
    cpu -> pc = 0;
    while (cpu -> pc < 32){
        clock_cycle(cpu);
    }
    printf("program finished\n");
}
