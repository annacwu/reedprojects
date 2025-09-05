## Homework 2
To compile this code, run the following command: 
```gcc Homework2.c HW2test.c -o test```

### 1. mod32() and mod16()
- **WHERE**: code for both mod32() and mod16() is in Homework2.h and Homework2.c
- **HOW**: run both mod32() and mod16() by inputting an integer x. For example, mod32(33) should give 1 and mod16(8) should give 8. 
- **Notes**: appears to be functioning properly. 

### 2. cond()
- **WHERE**: code for cond() is in Homework2.h and Homework2.c
- **HOW**: run both cond() by inputting three integers, x, y, and z. For example, cond(0, 2, 3) should give 3.
- **Notes**: appears to be functioning properly. I went to tutoring and was helped by Molly on Tuesday to arrive at this solution. 

### 3. Number Representation: 
- **WHERE**: the answers for question three are the following: 

(a) Unsigned integers with 13 bits
max: 8191
min: 0

(b) Signed integers with 13 bits.
max: 4095
min: -4096

(c) Unsigned integers with 32 bits
max: 4,294,967,295 or 2^32 - 1
min: 0

(d) Unsigned integers with 64 bits
max: 2^64 - 1
min: 0

(e) Unsigned fixed points with 32 bits
max: 65,535.99998474121
min: 0.00001525878 or (1/2)^16

(f) Unsigned fixed points with 64 bits
max: 4,294,967,296 or 2^32
min: (1/2)^32 or 2.3283064e-10

### 4a. swap_bits()
- **WHERE**: code for swap_bits() is in Homework2.h and Homework2.c
- **HOW**: run both swap_bits() by inputting a number (either in binary representation or decimal representation), as well as two integers less than 16. For example, swap_bits(0b101, 1, 2) [also known as swap_bits(3, 1, 2)] should give 0000000000000011.
- **Notes**: appears to be functioning properly. I realized later on after it already worked that I could've used my ith_bit function for the beginning part, but I already coded it so I left it as is. Additionally, to test this function I use print_binary, which I copied over from lab into the Homework2.c file. I could've imported the lab header to use it, but this was simpler for me. I use it for 4b and 4c as well.

### 4b. reverse()
- **WHERE**: code for reverse() is in Homework2.h and Homework2.c
- **HOW**: run both reverse() by inputting an integer that can be represented by 16 bits, as well as two integers less than 16. For example, reverse(12,0,3) should give 3.
- **Notes**: appears to be functioning properly. I had a weird test case where reverse(12,0,3) only worked if the loop ran for reps - 1, whereas reverse(12,1,2) only worked if the loop ran for reps. I understand why that was the case for 12,1,2, but not entirely sure for 12,0,3 because all the other cases worked regardless of the condition. Either way, the code functions for all the cases I could think of. 


### 4c. is_bin_pal()
- **WHERE**: code for is_bin_pal() is in Homework2.h and Homework2.c
- **HOW**: run both is_bin_pal() by inputting an integer that can be represented by 16 bits (either in binary or decimal representation -- it is easier to see the effect in binary). For example, is_bin_pal(0b101000) should return 1. 
- **Notes**: appears to be functioning properly. I use a function call to ith_bit, also a function I copied over from lab2 that I've placed in Homework2.c.

### 4d. Hexadecimal Palindromes

If a number is a hexidecimal palindrome, that does not imply that it is also a binary palindrome. For example, the hexidecimal 0x000BEEB0 is a palindrome, but it equates to 10111110111010110000 in binary, which is not a palindrome. 

Similarly, if a number is a binary palindrome, that does not imply that it is also a hexidecimal palindrome. For example, the binary number 0b1000000000000001 is a palindrome, but it equates to 0x00008001, which is not a palindrome. 
