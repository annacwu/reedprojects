#include <stdio.h>
#include "Homework2.h"
#include <stdint.h>

int main(){
    // TESTS for mod32() and mod16()
    printf("\nTests for mod32():\n");

    // test 1: 33
    // expected output: 1
    printf("test 1 -- 33: %d\t expected: 1\n", mod32(33));

    // test 2: 32
    // expected output: 0
    printf("test 2 -- 32: %d\t expected: 0\n", mod32(32));

    // test 3: 17
    // expected output: 17
    printf("test 3 -- 17: %d\t expected: 17\n", mod32(17));

    printf("Tests for mod16():\n");

    // test 1: 17
    // expected output: 1
    printf("test 1 -- 17: %d\t expected: 1\n", mod16(17));

    // test 2: 8
    // expected output: 8
    printf("test 2 -- 8: %d\t expected: 8\n", mod16(8));

    // test 3: 16
    // expected output: 0
    printf("test 3 -- 16: %d\t expected: 0\n", mod16(16));

    // TESTS for cond()
    printf("\nTests for cond():\n");

    // test 1: 1,2,3
    // expected output: 2
    printf("test 1 -- 1,2,3: %d\t expected: 2\n", cond(1,2,3));

    // test 2: 0,2,3
    // expected output: 3
    printf("test 2 -- 0,2,3: %d\t expected: 3\n", cond(0,2,3));

    // test 3: 10,54,79
    // expected output: 54
    printf("test 3 -- 10,54,79: %d\t expected: 54\n", cond(10,54,79));

    // TESTS for swap_bits()
    printf("\nTests for swap_bits():\n");

    // test 1: 0b101, 1, 2
    // expected output: 0000000000000011
    printf("test 1 -- 0b101, 1, 2: ");
    print_binary(swap_bits(0b101, 1, 2));
    printf("\t expected: 0000000000000011\n");

    // test 2: 0b1001, 2, 3
    // expected output: 0000000000000101
    printf("test 2 -- 0b1001, 2, 3: ");
    print_binary(swap_bits(0b1001, 2, 3));
    printf("\t expected: 0000000000000101\n");

    // test 3: 0b001, 1, 2
    // expected output: 0000000000000001
    printf("test 3 -- 0b001, 1, 2: ");
    print_binary(swap_bits(0b001, 1, 2));
    printf("\t expected: 0000000000000001\n");

    // test 4: 0b1001, 3, 4
    // expected output: 0000000000010001
    printf("test 4 -- 0b1001, 3, 4: ");
    print_binary(swap_bits(0b1001, 3, 4));
    printf("\t expected: 0000000000010001\n");

    // TESTS for reverse()
    printf("\nTests for reverse():\n");

    // test 1: 12,0,3
    // expected output: 3
    printf("test 1 -- 12,0,3: %d\t expected: 3\n",reverse(12,0,3));

    // test 2: 12,1,2
    // expected output: 10
    printf("test 2 -- 12,1,2: %d\t expected: 10\n",reverse(12,1,2));

    // test 3: 5,0,4
    // expected output: 20
    printf("test 3 -- 5,0,4: %d\t expected: 20\n",reverse(5,0,4));

    // test 4: 1,0,15
    // expected output: 32768
    printf("test 4 -- 1,0,15: %d\t expected: 32768\n",reverse(1,0,15));

    // TESTS for is_bin_pal()
    printf("\nTests for is_bin_pal():\n");

    // test 1: 0b101
    // expected output: 1
    printf("test 1 -- 0b101: %d\t expected: 1\n", is_bin_pal(0b101));

    // test 2: 0b101000
    // expected output: 1
    printf("test 2 -- 0b101000: %d\t expected: 1\n", is_bin_pal(0b101000));

    // test 3: 0b1011
    // expected output: 0
    printf("test 3 -- 0b1011: %d\t expected: 0\n", is_bin_pal(0b1011));

    // test 4: 0b10110000
    // expected output: 0
    printf("test 4 -- 0b10110000: %d\t expected: 0\n", is_bin_pal(0b10110000));

    // ADDITIONAL POTENTIAL PALINDROME TESTS:
    // printf("this should print 1: %d\n", is_bin_pal(0b1111));
    // printf("this should print 1: %d\n", is_bin_pal(0b1111000));
    // printf("this should print 1: %d\n", is_bin_pal(0b1000000000000001));

    return 0;
}