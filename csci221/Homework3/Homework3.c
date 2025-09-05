#include "Homework3.h"
#include <stdint.h>
#include <stdio.h>

// computes the closest power of 2 of the input
// via bitwise operations and masking
float power2_floor(float x){
    union float_bits num;
    num.f = x;
    uint32_t mask = 0b01111111100000000000000000000000;
    num.bits &= mask;
    return num.f;
}

// computes a float with a surgically inputted exponent
// via bitwise operations and biased conversion of exponent values
float set_exp(float x, int new_exp){
    union float_bits num;
    num.f = x;

    // clearing the previous exponent
    uint32_t mask = 0b10000000011111111111111111111111;
    num.bits &= mask;
    // converting the exponent to its biased representation
    new_exp += 127;
    new_exp = new_exp << 23;
    
    num.bits |= new_exp;

    return num.f;
}

// prints the value of a 16 bit integer as though it is a fixed point
// via bitwise operations and for loops
void print_fixed_pt(uint16_t x){
    float num = 0;
    
    // isolate integer and fractional values
    uint16_t intmask = 0b1111111100000000;
    uint16_t fracmask = 0b11111111;

    uint16_t intvalue = x & intmask;
    intvalue >>= 8;
    num += intvalue;

    uint16_t fracvalue = x & fracmask; 

    for(int i = 0; i < 8; i++){
        // compute fractional values of the fixed point by multiplying the 
        // any 1 on the right side of the radix point with its corresponding
        // value of 2^-i
        float frac = ith_bit16(fracvalue, i) * (1.0f/pow(2,(8-i))); 
        num += frac;
    }

    printf("%f", num);

}

// prints the value of an sfloat16 as a float
// through for loops and function calls
void print_sfloat_dec(sfloat16 x){
    float final = 0;
    float power = pow(2, x.exp);

    final += power;

    // set to 1 for the implicit one
    float num = 1;

    for(int i = 0; i < 16; i++){
        // compute the fractional value of the significand through
        // similar process to previous function
        float frac = ith_bit16(x.sig, i) * (1.0f/pow(2,(16-i))); 
        num += frac;
    }

    // multiply final (becaue it is set to value of the power) by the 1.sig value
    final *= num; 

    printf("%f", final);

}

// computes the sum of two sfloat16s
// through loops and integer addition
sfloat16 add_sfloat(sfloat16 x, sfloat16 y){
    // using replace bit to add the implicit ones in
    uint32_t ysig = y.sig;
    ysig = replace_bit(ysig, 16, 1);
    uint32_t xsig = x.sig;
    xsig = replace_bit(xsig, 16, 1); // couldve appended with an or where the 17th bit was 1

    if (x.exp != y.exp){
        while (x.exp != y.exp) {
            y.exp += 1;
            ysig = ysig >> 1;
        }
    }

    uint32_t sum = xsig + ysig;
    sfloat16 final;
    int count = 0;
    // if the sum exceeds the greatest number than can be represented by 16 bits
    while (sum > 131071){
        sum = sum >> 1;
        count++;
    }
    // get rid of the implicit one
    sum = replace_bit(sum, 16, 0);
    final.sig = sum;
    final.exp = x.exp + count;

    return final;
}


// the following are the helper functions from the personal library
// copied here instead of imported from a header because
// many had to be altered slightly (as explained in Homework3.h)
void print_binary(int32_t x) {
    // printf("The binary representation of %d is: ", x);
    for(int i = 31; i >= 0; i--){
        if (ith_bit(x,i) == 1){
            printf("1");
        } else {
            printf("0");
        }
    }
    printf("\n");
}
int ith_bit(int32_t x, int i) {
    int32_t m = 0b1;
    m = m << i; 
    int32_t res = x & m;
    res = res >> i;
    if (res == 1){
        return 1;
    } else {
        return 0;
    }
}
int ith_bit16(int16_t x, int i) {
    int32_t m = 0b1;
    m = m << i; 
    int32_t res = x & m;
    res = res >> i;
    if (res == 1){
        return 1;
    } else {
        return 0;
    }
}
int32_t replace_bit(int32_t x, int32_t i, int32_t b){
    int32_t m = 0b1;
    m = m << i;
    if (b == 1){
        return x | m;
    } else {
        int32_t opp = ~m;
        return x & opp;  
    }
}
float pow (int base, int power) {
    if(power == 0){
        return 1.0f;
    }
    if(power < 0){
        int res = base;
        for (int i = 1; i < -power; i++){
            res = res * base;
        } 
        float result = 1.0f/res;
        return result;
    }
    int res = base;
    for (int i = 1; i < power; i++){
        res = res * base;
    } 
    return res;
}