## Homework 3
To compile this code, run the following command: 
```gcc Homework3.c HW3test.c -o test```

### 1a. union float_bits
- **WHERE**: code for the union float_bits is in Homework3.h. 
- **HOW**: used by typing union float_bits x; where x is the name of the variable.
- **Notes**: appears to be functioning properly.

### 1b. power2_floor()
- **WHERE**: code for power2_floor() is in Homework3.c and Homework3.h.
- **HOW**: run power2_floor by inputting a float. For example, power2_floor(143.5) should give 128.0.
- **Notes**: appears to be functioning properly.

### 1c. set_exp()
- **WHERE**: code for set_exp() is in Homework3.c and Homework3.h.
- **HOW**: run set_exp() by inputting a float and an integer that represents the value the float's exponent should be changed to. For example, set_exp(2,2) should return 4 because 2 is 1.0 x 2^1, so it would then be 1.0 x 2^2.
- **Notes**: appears to be functioning properly. Arithmetic operators were used for the conversion of the integer to its biased representation because Olive said that was allowed. 

### 2. print_fixed_pt()
- **WHERE**: code for print_fixed_point() is in Homework3.c and Homework3.h.
- **HOW**: run print_fixed_point() by inputting a 16 bit integer. It will be evaluated as though the radix point is halfway through, with 8 bits on each side. For example, print_fixed_point(0b0000001110000000) should give 3.5.
- **Notes**: appears to be functioning properly. Makes use of the altered ithbit function that takes 16 bit integers from lab 2 and an altered version of my power function from lab 0, as well as a C specific syntax for changing an integer to a float (0.0f). 

### 3a. struct sfloat16
- **WHERE**: code for struct sfloat16 is in Homework3.h.
- **HOW**: used typedef, so it is referred to solely as sfloat16 in all code. 
- **Notes**: appears to be functioning properly.

### 3b. print_sfloat_dec()
- **WHERE**: code for print_sfloat_dec() is in Homework3.c and Homework3.h.
- **HOW**: run print_sfloat_dec() by inputting an sfloat16 x, where its exponent and significand are defined. For example, sfloat16 = {1,0} inputted into print_sfloat_dec() should return 2.0.
- **Notes**: appears to be functioning properly. Makes use of the altered ithbit function that takes 16 bit integers from lab 2 and an altered version of my power function from lab 0, as well as a C specific syntax for changing an integer to a float (0.0f). 

### 3c. add_sfloat()
- **WHERE**: code for add_sfloat() is in Homework3.c and Homework3.h.
- **HOW**: run add_sfloat() by inputting two sfloat16s to be computed for their sum. For example, an sfloat16 x = {2, 0} and sfloat16 y = {1, 0} for add_sfloat(x, y) should give 6.0.
- **Notes**: appears to be functioning properly. Makes use of the replace_bit function from lab 2, altered to take 32 bit integers. 

### 4. Floating Point Increments
- **WHERE**: the answers for question four are the following: 

4a. For 2.0, there are no bits in the significand, as it is encoded thorugh the implicit 1 and the exponent 2^1. For the first floating point number greater than 2.0, the rest is the same but there is a bit in the significand. The same logic can be applied to 4.0, as it is encoded through the implicit 1 and 2^2 with no bits in the significand, and its next number does have a bit in the significand.

4b. There are 2^23 options available, as anything between 2 and 4, including 2 but excluding 4, would be encoded through an exponent of 2^1, where the only bits being changed are the 23 significand bits. Because each bit can either be 0 or 1, that means that each of the 2 options can be encoded 23 times with 2^23 different combinations, all of which encode a different number.

4c. For 2.0, there are no bits in the significand, as it is encoded thorugh the implicit 1 and the exponent 2^1. For the first floating point number greater than 2.0, the rest is the same but there is a bit in the significand. The same logic can be applied to 4.0, as it is encoded through the implicit 1 and 2^2 with no bits in the significand, and its next number does have a bit in the significand. (same as 4a, applies to sfloat16s in the same manner as long as they are not negative or the biased version of the exponent is not a factor).

4d. 127 x 2^16. There are 127 possible negative exponents to be encoded with an 8 bit exponent for the sfloat16, and there is no possible way to encode 0 with an sfloat16 because the implicit 1 is always present. Therefore, because the significand is 16 bits, it can be encoded through 2^16 total combinations with each of the 127 negative exponents (from -1 to -127). Hence, 127 x 2^16. 

4e. (2^23 x 127) + 1.  All positive fractional values (between 0 and 1) are encoded through the negative exponents. There are 127 negative exponents, and 23 bits in the significand that can all be encoded in 2 different combinations (0 and 1), so its 2^23 x 127. Then, to encode 0, the exponent has to be encoded with all 0s to replace the implicit one, and the significant also has to be all 0s. This is the only way to encode 0, so to include it in the count of numbers on the interval [0, 1), it would be (2^23 x 127) + 1. 