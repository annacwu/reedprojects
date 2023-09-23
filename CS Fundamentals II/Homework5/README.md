## Homework 5
To compile this code, run the following command: 
```gcc Homework5.c HW5test.c -o test```

For the MIPS code, put the .asm file into QtSpim. See ### 2 MIPS Assembly Basics for further instruction.

### 1a. maximum()
- **WHERE**: code for maximum() is in Homework5.c and Homework5.h.
- **HOW**: run maximum() by inputting an array pointer, the size of the items in the array, and the length of the array. For example, array[5] = {100, 2, 58, 738, 4} and maximum(&array, sizeof(uint32_t), 5) should give a pointer to the address of 738.
- **Notes**: appears to be functioning properly. Tested by casting to a uint32_t and dereferencing the memory address. 

### 1b. CmpInt()
- **WHERE**: code for CmpInt() is in Homework5.c and Homework5.h.
- **HOW**: run CmpInt() by inputting two void pointers to integers. For example, if int x = 7 and int y = 6, CmptInt(&x, &y) should return 1.
- **Notes**: appears to be functioning properly.

### 1c. CmpFloat()
- **WHERE**: code for CmpFloat() is in Homework5.c and Homework5.h.
- **HOW**: run CmpFloat() by inputting two void pointers to floats. For example, if int x = 7.8 and int y = 3.4, CmptFloat(&x, &y) should return 1.
- **Notes**: appears to be functioning properly.

### 1d. maximum_v2()
- **WHERE**: code for maximum_v2() is in Homework4.c and Homework4.h.
- **HOW**: run maximum_v2() by inputting an array pointer, the size of the items in the array, the length of the array, and a function. For example, array[5] = {100, 2, 58, 738, 4} and maximum(&array, sizeof(uint32_t), 5, CmpInt) should give a pointer to the address of 738.
- **Notes**: appears to be functioning properly. Only 1 line difference from maximum.

### 2. MIPS Assembly Basics
- **WHERE**: code for the MIPS assembly problem is in HW5MIPS.asm.
- **HOW**: run HW5MIPS.asm by loading it into QtSpim and assigning a value to the register $a0. For example, if $a0 holds the value 4, by the end of running the code, there should be a 16 stored in $v0.
- **Notes**: In the asm file, I have a commented out line with li $a0, 4. This is what I used to test the program. Additionally, running it in QtSpim will give an error because technically I do not terminate the main program properly with a syscall due to the fact that that overwrites $v0 and it's harder to see the result. The program still runs to the necesssary point despite the error. 

### 3a. sCPU struct
- **WHERE**: code for the sCPU struct is in Homework5.c and Homework5.h.
- **HOW**:  it was created with a typedef, so it is used by typing sCPU as a type in the program.
- **Notes**: appears to be functioning properly.

### 3b. CPU_init()
- **WHERE**: code for CPU_init() is in Homework5.c and Homework5.h.
- **HOW**: run CPU_init() by initializing a new sCPU pointer variable. For example, sCPU* cpu_ptr = CPU_init() will create a new dynamically allocated empty sCPU.
- **Notes**: appears to be functioning properly.

### 3c. CPU_destroy()
- **WHERE**: code for CPU_destroy() is in Homework5.c and Homework5.h.
- **HOW**: run CPU destroy by passing it a pointer to an sCPU. For example, CPU_destroy(cpu_ptr) will deallocate cpu_ptr.
- **Notes**: appears to be functioning properly.

### 4-6. CPU_step()
- **WHERE**: code for CPU_step() is in Homework5.c and Homework5.h.
- **HOW**: run CPU_step() by inputting an opcode (0 prints r0, 1 puts, 2 adds, 2 stores, 4 loads, 5 multiplies), as well as various values in args 1, 2, and 3, which correspond to different registers. For example, CPU_step(2,0,0,1, cpu_ptr) computer r0 = r0 + r1. Execute a variety of these commands in a row in the test file to manipulate the sCPU. 
- **Notes**: appears to be functioning properly.

### 7a. Computing 1 + 3 + 5 + 7 + 9
- **WHERE**: code for executing 1 + 3 + 5 + 7 + 9 is in HW5test.c.
- **HOW**: Using opcodes 0, 1, and 2 for CPU_step. 
- **Notes**: The code is in the test file, marked by a comment that says 7a. 

### 7b. Computing x++
- **WHERE**: code for executing x++ is in HW5test.c.
- **HOW**: Using opcodes 0, 1, 2, and 4 for CPU_step.
- **Notes**: The code is in the test file, marked by a comment that says 7b.

### 7c. Array with first 5 powers of 2
- **WHERE**: code for executing an array with first 5 powers of 2 is in HW5test.c.
- **HOW**: using opcodes 0, 1, 3, 4, and 5 for CPU_step.
- **Notes**: The code is in the test file, marked by a comment that says 7c.
