#include "Homework4.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(){

    // TESTS for print_uint8_arr(), print_uint8_arr_v2(), and print_uint16_arr()
    printf("\nTests for print_uint8_arr(), print_uint8_arr_v2(), and print_uint16_arr():\n");

    // test 1: {5, 27, 11, 43}
    // expected output: [ 5 27 11 43 ]
    uint8_t xs[4] = {5, 27, 11, 43};
    printf("test 1 -- print_uint8_arr: ");
    print_uint8_arr(xs, 4);
    printf("\t expected: [ 5 27 11 43 ]\n");

    // test 2: {5, 27, 11, 43}
    // expected output: [ 5 27 11 43 ]
    printf("test 2 -- print_uint8_arr_v2: ");
    print_uint8_arr_v2(xs, 4);
    printf("\t expected: [ 5 27 11 43 ]\n");

    // test 3: {732, 748, 212, 1012}
    // expected output: [ 732, 748, 212, 1012 ]
    uint16_t zs[4] = {732, 748, 212, 1012};
    printf("test 3 -- print_uint16_arr: ");
    print_uint16_arr(zs, 4);
    printf("\t expected: [ 732, 748, 212, 1012 ]\n");

    // TESTS for surgical_mult4()
    printf("\nTests for surgical_mult4():\n");

    // test 1: 4.0
    // expected output: 16.0
    float f = 4.0;
    surgical_mult4(&f);
    printf("test 1 -- 4.0: %f\t expected: 16.0\n", f);

    // test 2: 1.25
    // expected output: 5.0
    float ff = 1.25;
    surgical_mult4(&ff);
    printf("test 2 -- 1.25: %f\t expected: 5.0\n", ff);

    // test 3: 4.35
    // expected output: 17.4
    float fff = 4.35;
    surgical_mult4(&fff);
    printf("test 3 -- 4.35: %f\t expected: 17.4\n", fff);

    // TESTS for find_substr()
    printf("\nTests for find_substr():\n");

    // test 1: "Sphinx of black quartz judge my vow", "ck quartz ju"
    // expected output: c (dereferenced from pointer)
    char string[] = "Sphinx of black quartz judge my vow";
    char stringg[] = "ck quartz ju";
    printf("test 1 -- 'Sphinx of black quartz judge my vow', 'ck quartz ju': %c\t expected: c (dereferenced from pointer)\n", *(find_substr(string, stringg)));

    // test 2: "Sphinx of black quartz judge my vow", "ck quartz jx"
    // expected output:  [\0] (dereferenced from pointer)
    char string1[] = "Sphinx of black quartz judge my vow";
    char stringg2[] = "ck quartz jx";
    printf("test 2 -- 'Sphinx of black quartz judge my vow', 'ck quartz jx': %c\t expected:[NUL] (dereferenced from pointer)\n", *(find_substr(string1, stringg2)));

    // test 3: "abcdef", "abcd"
    // expected output: a
    char stringy[] = "abcdef";
    char stringyy[] = "abcd";
    printf("test 3 -- 'abcdef','abcd': %c\t expected: a (dereferenced from pointer)\n", *(find_substr(stringy, stringyy)));

    // TESTS for print_board()
    printf("\nTests for print_board():\n");

    // test 1: 
    printf("test 1: \n");
    int board_size = 4;
    char** board = malloc(sizeof(char*) * board_size);
    for (int j=0; j<board_size; j++) {
        board[j] = malloc(sizeof(char) * board_size);
        for (int i=0; i<board_size; i++) {
        board[j][i] = '.';
        }
    }
    board[1][3] = '@';
    print_board(board, board_size);

    //test 2: 
    printf("test 2: \n");
    int board_size1 = 7;
    char** board1 = malloc(sizeof(char*) * board_size1);
    for (int j=0; j<board_size1; j++) {
        board1[j] = malloc(sizeof(char) * board_size1);
        for (int i=0; i<board_size1; i++) {
        board1[j][i] = '.';
        }
    }
    board1[4][1] = '@';
    print_board(board1, board_size1);

    //test 3: 
    printf("test 3: \n");
    int board_size2 = 3;
    char** board2 = malloc(sizeof(char*) * board_size2);
    for (int j=0; j<board_size2; j++) {
        board2[j] = malloc(sizeof(char) * board_size2);
        for (int i=0; i<board_size2; i++) {
        board2[j][i] = '.';
        }
    }
    board2[0][0] = '@';
    print_board(board2, board_size2);

    // all tests fulfill their expected output
    printf("-> All boards print as expected.\n");

    // TESTS for move_piece()
    printf("\nTests for move_piece() [using board from test 1 of print_board()]:\n");

    // test 1: n
    printf("test 1 -- n: \n");
    move_piece(board, board_size, 'n');
    print_board(board, board_size);

    // test 2: s,w
    printf("test 2 -- s,w: \n");
    move_piece(board, board_size, 's');
    move_piece(board, board_size, 'w');
    print_board(board, board_size);

    // test 3: e, e
    printf("test 1 -- e,e: \n");
    move_piece(board, board_size, 'e');
    move_piece(board, board_size, 'e');
    print_board(board, board_size);

    // all tests fulfill their expected output
    printf("-> All boards print as expected.\n");

    // the following is for all stack tests: 
    stack* stackywacky = (stack*) malloc(sizeof(stack));
    stackywacky -> numnodes = 0;
    uint32_t one = 3;
    uint32_t two = 10;
    uint32_t three = 7;

    // TESTS for push()
    printf("\nTests for push():\n");

    // test 1: 3
    // expected output: 3
    push(stackywacky, one);
    printf("test 1 -- 3: %d\texpected: 3\n", (stackywacky ->top) ->val);

    // test 2: 10
    // expected output: 10
    push(stackywacky, two);
    printf("test 2 -- 10: %d\texpected: 10\n", (stackywacky ->top) ->val);

    // test 3: 7
    // expected output: 7
    push(stackywacky, three);
    printf("test 3 -- 7: %d\texpected: 7\n", (stackywacky ->top) ->val);

    // TESTS for print_stack()
    printf("\nTest for print_stack():\n");
    
    // test:
    printf("test: \n");
    print_stack(stackywacky);
    printf("expected:\n7\n10\n3\n");

    // TESTS for pop()
    printf("\nTests for pop():\n");

    // test 1: 
    // expected output: 7
    printf("test 1: %d\t expected: 7\n",pop(stackywacky));

    // test 2: 
    // expected output: 10
    printf("test 2: %d\t expected: 10\n",pop(stackywacky));

    // test 3: 
    // expected output: 3
    printf("test 3: %d\t expected: 0\n",pop(stackywacky));

    // test 4: 
    // expected output: No nodes to pop.
    printf("test 4: %d\t expected: 'There are no nodes to pop', 0\n",pop(stackywacky));

    push(stackywacky, one);
    push(stackywacky, two);
    push(stackywacky, three);

    // TESTS for free_stack()
    printf("\nTest for free_stack():\n");
    free_stack(stackywacky);
    printf("--> Functions as expected.\n\n");

    return 0;
}