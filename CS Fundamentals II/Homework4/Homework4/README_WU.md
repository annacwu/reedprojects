## Homework 4
To compile this code, run the following command: 
```gcc Homework4.c HW4test.c -o test```

### 1a. print_uint8_arr()
- **WHERE**: code for print_uint8_arr() is in Homework4.c and Homework4.h.
- **HOW**: run print_uint8_arr() by inputting an array and the length of the array. For example, xs = {1, 2, 3, 4}, print_uint8_arr(xs, 4) should give [ 1 2 3 4 ].
- **Notes**: appears to be functioning properly.

### 1b. print_uint8_arr_v2()
- **WHERE**: code for print_uint8_arr_v2() is in Homework4.c and Homework4.h.
- **HOW**: run print_uint8_arr_v2() by inputting an array and the length of the array. For example, xs = {1, 2, 3, 4}, print_uint8_arr_v2(xs, 4) should give [ 1 2 3 4 ].
- **Notes**: different from 1a because it uses the dereference operator and pointer addition.

### 1c. print_uint16_arr()
- **WHERE**: code for print_uint16_arr() is in Homework4.c and Homework4.h.
- **HOW**: run print_uint16_arr() by inputting an array and the length of the array. For example, xs = {1, 2, 3, 4}, print_uint16_arr(xs, 4) should give [ 1 2 3 4 ].
- **Notes**: same code as 1b but takes a uint16_t as an argument. 

### 1d. Code Execution

Two arrays get printed to the console: 
[ 257 514 17 1028 18 19 20 21 ]
[ 1 1 2 2 17 0 4 4 ]

These end up being printed because the casted_y_ptr interprets all of the values in the array ys as uint8_ts rather than u_int16s, which is what they initially are. So, the y pointer points to the same spot in memory as the ys array, but it traverses it byte by byte rather than two bytes at a time. Therefore, as can be seen in the diagram labeled 1d.jpeg, it reads the least significant bits stored in the memory address of the first uint16, 257. These bits are equivalent to 1, so 1 is printed to console. Then, it moves to the next half of the same bits of 257, which also equal 1, so 1 is printed again. This continues for only the first 4 numbers in the 8 number array, because the iterations stop at 8 and it has to read each of the 4 numbers twice. 

### 2. Pointer Puzzle

a. As can be seen in the diagram 2a.jpeg, a, b, and c are all void pointers that point to each other's location in memory. When it says a = &b, it means that a is a pointer to the address of b. The same follows for the rest of the pointers. 

b. The result of evaluating the code is 0. The function dereferences the void pointer as a void** pointer, as a regular void* cannot be dereferenced. So, it interprets the pointer, for example a, as a pointer to a void pointer, which when dereferenced, gives the address it points to. So, from 2a, we see that a points to the address of b. The function then does that again to b to get the address of c, then again to get a again, then a to b, then b to c. So, f(f(f(f(f(a))))) = c = &a. Thus, the code is actually c - c, or &a - %a, which equals 0. 

### 3. surgical_mult4()
- **WHERE**: code for surgical_mult4() is in Homework4.c and Homework4.h.
- **HOW**: run surgical_mult4() by inputting a pointer to a float or the address. For example, float f = 4.0, surgical_mult4(&f) should give 16.0.
- **Notes**: I didn't actually create a separate pointer to test, I just put in &f because it is essentially the same thing.

### 4. find_substr()
- **WHERE**: code for find_substr() is in Homework4.c and Homework4.h.
- **HOW**: run find_substr() by inputting two strings, the second of which is the potentially substring of the first.
- **Notes**: because it can't actually print \0 to the console when it comes across it, it just stops printing the string. This is to be expected, but I thought I would make a note of it anyway. 

### 5a. print_board()
- **WHERE**: code for print_board() is in Homework4.c and Homework4.h.
- **HOW**: run print_board() by creating the board in the main function through for loops to establish the arrays, then inputting a pointer to the arrays and their size. 
- **Notes**: Testing this didn't follow the standard test layout for me because it is much more complex. I still tried different cases and everything appeared to be functioning properly.

### 5b. move_piece()
- **WHERE**: code for move_piece() is in Homework4.c and Homework4.h.
- **HOW**: run move_piece() by inputting a pointer to the arrays (the board) and their size, as well as a cardinal diretion to move the piece. For example, 'n' would move the piece north. 
- **Notes**: Testing this didn't follow the standard test layout for me because it is much more complex. I still tried different cases and everything appeared to be functioning properly.

### 6a. struct node
- **WHERE**: code for the struct node is in Homework4.h. 
- **HOW**: it is typedef'ed, so it is used by typing node as a type in the program.
- **Notes**: appears to be functioning properly.

### 6b. struck stack
- **WHERE**: code for the struct stackis in Homework4.h. 
- **HOW**: it is typedef'ed, so it is used by typing stack as a type in the program.
- **Notes**: in the test file I malloc'd the stack upon creation, or else there were problems with free_stack with my operating system. Not sure if this was intended or not, but it works.

### 6c. push()
- **WHERE**: code for push() is in Homework4.c and Homework4.h.
- **HOW**: run push() by inputting a pointer to a stack and the value of the node to be added. 
- **Notes**: appears to be functioning properly.

### 6d. print_stack()
- **WHERE**: code for print_stack() is in Homework4.c and Homework4.h.
- **HOW**: run print_stack() by passing it a pointer to a stack type. 
- **Notes**: testing this was complicated in the scope of testing all the other stack functions as well, so I only officially included one official test. However, I used it many times over the course of problem 6 to test the other functions and it works well.

### 6e. pop()
- **WHERE**: code for pop() is in Homework4.c and Homework4.h.
- **HOW**: run pop() by inputting a pointer to a stack that should be popped.
- **Notes**: i added an error case to make sure if you try to use it to pop an empty stack there aren't a bunch of errors. 

### 6f. free_stack()
- **WHERE**: code for free_stack() is in Homework4.c and Homework4.h.
- **HOW**: run free_stack() by passing it a pointer to a stack type.
- **Notes**: appears to be functioning properly, though it is a little hard to test. I included print functions in the function so it would tell me if it was iterating and I kept it in for testing. When I try to print the freed stack after calling this function the program does nothing, so I assume it worked. There were also a lot of seg fault errors when I was making this one, so the layout of the conditionals and loops are a little funky, but ultimately functions as expected. 