#include <stdio.h>
#include "Homework3.h"
#include <stdint.h>

int main() {
    // TESTS for power2_floor()
    printf("\nTests for power2_floor():\n");

    // test 1: 4.5
    // expected output: 4.0
    printf("test 1 -- 4.5: %f\t expected: 4.0\n", power2_floor(4.5));

    // test 2: 2
    // expected output: 2.0
    printf("test 2 -- 2: %f\t expected: 2.0\n", power2_floor(2));

    // test 3: 5.5
    // expected output: 4.0
    printf("test 3 -- 5.5: %f\t expected: 4.0\n", power2_floor(5.5));

    // test 4: 143
    // expected output: 128.0
    printf("test 4 -- 143: %f\t expected: 128.0\n", power2_floor(143));

    // TESTS for set_exp()
    printf("\nTests for set_exp():\n");

    // test 1: 2,2
    // expected output: 4.0
    printf("test 1 -- 2,2: %f\t expected: 4.0\n", set_exp(2,2));
    
    // test 2: 16,2
    // expected output: 4.0
    printf("test 2 -- 16,2: %f\t expected: 4.0\n", set_exp(16,2));

    // test 3: 3,8
    // expected output: 384.0
    printf("test 3 -- 3,8: %f\t expected: 384.0\n", set_exp(3,8));

    // TESTS for print_fixed_pt()
    printf("\nTests for print_fixed_pt():\n");

    // test 1: 0b0000001110000000
    // expected output: 3.5
    printf("test 1 -- 0b0000001110000000: ");
    print_fixed_pt(0b0000001110000000);
    printf("\t expected: 3.5\n");

    // test 2: 0b0000001111000000
    // expected output: 3.75
    printf("test 2 -- 0b0000001111000000: ");
    print_fixed_pt(0b0000001111000000);
    printf("\t expected: 3.75\n");

    // test 3: 0b0001111111110000
    // expected output: 31.9375
    printf("test 3 -- 0b0001111111110000: ");
    print_fixed_pt(0b0001111111110000);
    printf("\t expected: 31.9375\n");

    // TESTS for print_sfloat_dec()
    printf("\nTests for print_sfloat_dec():\n");

    // test 1: 1,0
    // expected output: 2.0
    sfloat16 test = {1,0};
    printf("test 1 -- 1,0: ");
    print_sfloat_dec(test);
    printf("\t expected: 2.0\n");

    // test 2: 2,0b1000000000000000
    // expected output: 6.0
    sfloat16 test1 = {2,0b1000000000000000};
    printf("test 2 -- 2,0b1000000000000000: ");
    print_sfloat_dec(test1);
    printf("\t expected: 6.0\n");

    // test 3: -1,0xC000
    // expected output: 0.875
    sfloat16 test2 = {-1,0xC000};
    printf("test 3 -- -1,0xC000: ");
    print_sfloat_dec(test2);
    printf("\t expected: 0.875\n");

    // TESTS for add_sfloat()
    printf("\nTests for add_sfloat():\n");

    // test 1: 2,0 + 1,0
    // expected output: 6.0
    sfloat16 atest = {2, 0};
    sfloat16 atest1 = {1, 0};
    printf("test 1 -- 2,0 + 1,0: ");
    print_sfloat_dec(add_sfloat(atest, atest1));
    printf("\t expected: 6.0\n");

    // test 2: 4,0 + 2,0b010000000000000
    // expected output: 21.0
    sfloat16 atest2 = {4, 0};
    sfloat16 atest3 = {2, 0b0100000000000000};
    printf("test 2 -- 4,0 + 2,0b010000000000000: ");
    print_sfloat_dec(add_sfloat(atest2, atest3));
    printf("\t expected: 21.0\n");

    // test 2: 1, 0b1000000000000000 + 2, 0
    // expected output: 7.0
    sfloat16 atest4 = {1, 0b1000000000000000};
    sfloat16 atest5 = {2, 0};
    printf("test 3 -- 1, 0b1000000000000000 + 2, 0: ");
    print_sfloat_dec(add_sfloat(atest5, atest4));
    printf("\t expected: 7.0\n");

    return 0;
}
