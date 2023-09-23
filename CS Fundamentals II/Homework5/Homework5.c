#include "Homework5.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// returns a pointer to the max value of any array
// via generic memory pointer arithmetic and memcmp
void* maximum(void* arr, int size, int num){
    void* max = (char*) arr;
    for (int i = 0; i < num; i++){
        void* elemAddr = (char*) arr + (i * size);
        if (memcmp(max, elemAddr, size) < 0){
            max = elemAddr;
        } 
    }
    return max;
}

// returns an integer based on the comparison of two generic values
// via casting as integers and conditional statements
int CmpInt(void* a_ptr, void* b_ptr){
    int* ia = (int*) a_ptr;
    int* ib = (int*) b_ptr;
    if (*ia - *ib < 0){
        return -1;
    } else if (*ia - *ib == 0){
        return 0;
    } else {
        return 1;
    }
}

// returns an integer based on the comparison of two generic values
// via casting as floats and conditional statements
int CmpFloat(void* a, void* b){
    float* ia = (float*) a;
    float* ib = (float*) b;
    if (*ia - *ib < 0){
        return -1;
    } else if (*ia - *ib == 0){
        return 0;
    } else {
        return 1;
    }
}

// returns a pointer to the max value of any array
// via generic memory pointer arithmetic and an argument comparison function
void* maximum_v2(void* arr, int size, int num, int (*cmpfn) (void*, void*)){
    void* max = (char*) arr;
    for (int i = 0; i < num; i++){
        void* elemAddr = (char*) arr + (i * size);
        if (cmpfn(max, elemAddr) < 0){
            max = elemAddr;
        } 
    }
    return max;
}

// returns a pointer to an sCPU with 0's in the ram and reg
// via loopos, malloc, and pointer syntax
sCPU* CPU_init(){
    sCPU* newCPU = (sCPU*) malloc(sizeof(sCPU));
    for (int i = 0; i < 1024; i++){
        newCPU -> ram[i] = 0;
    }
    for (int i = 0; i < 32; i++){
        newCPU -> reg[i] = 0;
    }
    return newCPU;
}

// deallocates an sCPU from memory
// via free command
void CPU_destroy(sCPU* cpu){
    free(cpu);
}

// manipulates the values in the ram and reg of an sCPU
// via inputted opcodes, pointer syntax, and conditional statements
void CPU_step(uint8_t opcode, uint8_t arg1,  uint8_t arg2,  uint8_t arg3, sCPU* cpu){
    if (opcode == 0){
        printf("%d\n", cpu -> reg[0]);
    } else if (opcode == 1){
        uint32_t x = 0b0;
        uint32_t a = arg2 << 8;
        uint32_t b = arg3;
        x = x | a;
        x = x | b;
        cpu -> reg[arg1] = x;
    } else if (opcode == 2){
        int sum = cpu -> reg[arg2] + cpu -> reg[arg3];
        cpu -> reg[arg1] = sum;
    } else if (opcode == 5){
        int prod = cpu -> reg[arg2] * cpu -> reg[arg3];
        cpu -> reg[arg1] = prod;
    } else if (opcode == 3){
        cpu -> ram[cpu -> reg[arg2]] = cpu -> reg[arg1];
    } else if (opcode == 4){
        cpu -> reg[arg1] = cpu -> ram[cpu -> reg[arg2]];
    }
}

