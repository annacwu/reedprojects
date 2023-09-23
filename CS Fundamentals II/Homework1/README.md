## Homework 1
To compile this code, run the following command: 
```gcc Homework1.c HW1test.c -o test```

To compile command_line_fib.c, run the following command: 
```gcc Homework1.c command_line_fib.c -o fibtest```

### 1. choose_mover()
- **WHERE**: code for choose_mover() is in Homework1.h and Homework1.c
- **HOW**: run choose_mover() by inputting an integer x for number of boxes. For example, choose_mover(31) should give B. 
- **Notes**: appears to be functioning properly. 

### 2a. fibonacci()
- **WHERE**: code for fibonacci() is in Homework1.h and Homework1.c
- **HOW**: run fibonacci() by inputting an integer n for nth number in the sequence. For example, choose_mover(8) should return 21. 
- **Notes**: when you input n > 48, it starts to return negative numbers. I think this has to do with binary overflow and 2s complement and such, the fibonacci numbers just become too large for the int. 

### 2b. command_line_fib()
- **WHERE**: code for command_line_fib() is in command_line_fib.c and Homework1.c
- **HOW**: run command_line_fib() by typing an integer into the command line following the executable file name. For example, './fibtest 8' should give 21.
- **Notes**: should give a print statement for any nonpositive or noninteger inputs. 

### 3a. int_three
- **WHERE**: the struct int_three is defined in Homework1.h
- **HOW**: the struct was defined with a typedef, so it is always referred to as just int_three. 
- **Notes**: the a, b, c variables are defined backwards (c, b, a) so that when one intuitively creates a struct it will be readable left to right, such as {1, 0, 0} to represent the 3-bit number 100.  

### 3b. print_three_unsigned() and print_three_signed()
- **WHERE**: code for print_three_unsigned() and print_three_signed() is in Homework1.h and Homework1.c
- **HOW**: run both functions by inputting an int_three as the argument. 
- **Notes**: appears to be functioning properly. 

### 3c. tiny_add()
- **WHERE**: code for tiny_add() is in Homework1.h and Homework1.c
- **HOW**: run tiny_add() by inputting two defined int_three structs. It should return the sum of them based on regular binary arithmetic.
- **Notes**: appears to be functioning properly as per the rules of overflow discussed in lecture. 

