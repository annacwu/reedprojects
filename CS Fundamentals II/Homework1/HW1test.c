#include <stdio.h>
#include "Homework1.h"

int main() {
    // TESTS FOR choose_mover()
    printf("Tests for choose_mover():\n");

    // test 1: 20 boxes
    // expected output: B
    printf("test 1 -- 20 boxes: %c\t expected: A\n", choose_mover(20));

    // test 2: 31 boxes
    // expected output: B
    printf("test 2 -- 31 boxes: %c\t expected: B\n", choose_mover(31));

    // test 3: 50 boxes
    // expected output: A
    printf("test 3 -- 50 boxes: %c\t expected: A\n", choose_mover(50));

    // test 4: 60 boxes
    // expected output: B
    printf("test 2 -- 60 boxes: %c\t expected: B\n", choose_mover(60));

    // TESTS FOR fibonacci()
    printf("\nTests for fibonacci():\n");

    // test 1: 2
    // expected result: 1
    printf("test 1 -- 2: %d\t expected: 1\n", fibonacci(2));

    // test 2: 5
    // expected result: 5
    printf("test 2 -- 5: %d\t expected: 5\n", fibonacci(5));

    // test 3: 8
    // expected result: 21
    printf("test 3 -- 8: %d\t expected: 21\n", fibonacci(8));

    // test 4: 20
    // expected result: 6765
    printf("test 4 -- 20: %d\t expected: 6765\n", fibonacci(20));

    // TESTS for print_three_unsigned() and print_three_signed()
    printf("\nTests for print_three_unsigned() and print_three_signed():\n");

    // test 1: 1, 0, 0
    // expected result: 4
    int_three x = {1, 0, 0};
    print_three_unsigned(x);

    // test 2: 1, 0, 1
    // expected result: 5
    int_three y = {1, 0, 1};
    print_three_unsigned(y);

    // test 3: 1, 1, 0
    // expected result: 6
    int_three z = {1, 1, 0};
    print_three_unsigned(z);

    // test 4: 1, 0, 0
    // expected result: -4
    print_three_signed(x);

    // test 5: 1, 0, 1
    // expected result: -3
    print_three_signed(y);

    // test 6: 0, 1, 1
    // expected result: 3
    int_three w = {0, 1, 1};
    print_three_signed(w);

    // TESTS for tiny_add()
    // these tests use the int_three structs defined in the print tests above
    printf("\nTests for tiny_add():\n");

    // test 1: 100 + 101
    // expected result: 001
    int_three testres = tiny_add(x,y);
    printf("The sum is: %d %d %d\n", testres.c, testres.b, testres.a);

    // test 2: 100 + 110
    // expected result: 010
    int_three testres2 = tiny_add(x,z);
    printf("The sum is: %d %d %d\n", testres2.c, testres2.b, testres2.a);

    // test 3: 100 + 011
    // expected result: 111
    int_three testres3 = tiny_add(x, w);
    printf("The sum is: %d %d %d\n", testres3.c, testres3.b, testres3.a);

    // test 4: 101 + 011
    // expected result: 000
    int_three testres4 = tiny_add(y, w);
    printf("The sum is: %d %d %d\n", testres4.c, testres4.b, testres4.a);
    
}