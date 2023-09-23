## Homework 6
To compile this code, run the following command: 
```gcc Homework6.c HW6test.c -o test```

For the MIPS code, put the .asm file into QtSpim. See SOEMSHDLFHSLKDF for further instruction.

### 1a. iCPU struct
- **WHERE**: code for the iCPU struct is in Homework6.c and Homework6.h.
- **HOW**: it was created with a typedef, so it is used by typing iCPU as a type in the program.
- **Notes**: appears to be functioning properly.

### 1b. iCPU_init() and iCPU_destroy()
- **WHERE**: code for iCPU_init() and iCPU_destroy() is in Homework6.c and Homework6.h.
- **HOW**: run iCPU_init() by initializing a new iCPU pointer variable. For example, iCPU* cpu_ptr = CPU_init() will create a new dynamically allocated empty iCPU. Run CPU destroy by passing it a pointer to an iCPU. For example, CPU_destroy(cpu_ptr) will deallocate cpu_ptr.
- **Notes**: appears to be functioning properly.

### 1c. iCPU_step()
- **WHERE**: code for iCPU_step() is in Homework6.c and Homework6.h.
- **HOW**: run iCPU_step() by inputting an opcode (0 prints r0, 1 puts, 2 adds, 2 stores, 4 loads, 5 multiplies, as well as additional codes added in #3), as well as various values in args 1, 2, and 3, which correspond to different registers. For example, CPU_step(2,0,0,1, cpu_ptr) computes r0 = r0 + r1. Execute a variety of these commands in a row in the test file to manipulate the iCPU. 
- **Notes**: appears to be functioning properly.

### 2a. read_instruction()
- **WHERE**: code for read_instruction() is in Homework6.c and Homework6.h.
- **HOW**: run read_instruction() by by inputting a uint32_t and a pointer to an iCPU. For example, read_instruction(0b0, cpu_ptr) should read the instruction that prints r0. 
- **Notes**: appears to be functioning properly. 

### 2b. write_instruction()
- **WHERE**: code for write_instruction() is in Homework6.c and Homework6.h.
- **HOW**: run write_instruction() by inputting 4 uint8_ts that serve as the opcode and 3 arguments, as well as an integer and an iCPU pointer. For example, write_instruction(0,0,0,0,4, mycpu) indicates that at the 4th line of the program, the program should print r0. 
- **Notes**: appears to be functioning properly. 

### 2c. clock_cycle()
- **WHERE**: code for clock_cycle() is in Homework6.c and Homework6.h.
- **HOW**: run clock_cycle() by inputting a pointer to a non-empty iCPU. 
- **Notes**: appears to be functioning properly. 

### 2d. run_program()
- **WHERE**: code for run_program() is in Homework6.c and Homework6.h.
- **HOW**: run run_program() by inputting a pointer to a non-empty iCPU. If it is empty, nothing will happen.
- **Notes**: appears to be functioning properly. 

### 2e. iCPU program
- **WHERE**: code for question 2e is in HW6.test
- **HOW**: marked by a comment that says // Question 2e.
- **Notes**: appears to be functioning properly. Though not in the original instructions, I added a terminate after I programmed it to end the program for test legibility. 

### 3. iCPU_step(): control unit instructions
- **WHERE**: code for iCPU_step(): control unit instructions is in Homework6.c and Homework6.h.
- **HOW**: run iCPU_step(): control unit instructions by inputting the new opcodes. Opcode 6 is the jump instruction, 7 is the branch equal instruction, 8 is the branch not equal instruction, and 9 is the terminate instruction. The rest is similar to how the other iCPU_step instrutions work. 
- **Notes**: appears to be functioning properly. Each of the additional codes (6-9) are marked with comments in the iCPU_step() function in Homework6.c.

### 4. Executing Machine Instructions
- **WHERE**: code for question 4 is in HW6.test
- **HOW**: marked by a comment that says // Question 4.
- **Notes**: appears to be functioning properly. Perhaps if I had used bne instead of beq, I may not have had to check it with 101 instead of 100 to get it run the proper number of times, but I'm not sure. Regardless, the code outputs the proper result. 

### 5. MIPS array problem
- **WHERE**: code for the MIPS array problem is in Homework6.asm, marked by a comment that says Question 5.
- **HOW**: To run the program, you have to comment out Question 6 on the file. Additionally, it assumes there is are pointers to two different arrays in $a0 and $a2, as well as the lengths of those arrays in $a1 and $a3. So, to do anything interesting, you have to create and code those in. 
- **Notes**: Technically the loop is after main, though because this isn't a function call program and rather more like a single function, this shouldn't cause an issue in the program execution. 

### 6. MIPS function call problem
- **WHERE**: code for MIPS function call problem is in Homework6.asm, marked by a comment that says Question 6.
- **HOW**: To run the program, you have to comment out Question 5 on the file or the two declarations of main will cause errors. 
- **Notes**: Attempted to follow all of the stack space allocation from lecture. The program is just terminated with a syscall. 

### 7. MIPS qual problem
- **WHERE**: code for the MIPS qual problem is in bitsNotSet.asm.
- **HOW**: run the MIPS qual problem by inputting an immediate into $a0 with the first line of main, li $a0, 3. Change 3 to be whatever desired number. 
- **Notes**: appears to be functioning properly. The code is ended with jr $ra rather than a syscall, but you can look at the $v0 register in QtSpim to see if the right output is there, which it seems to be. 