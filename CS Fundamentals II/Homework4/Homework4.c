#include "Homework4.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// prints an array of uint8_ts
// via loops and array syntax
void print_uint8_arr (uint8_t* arr, int len){
    printf("[ ");
    for (int i = 0; i < len; i++){
        printf("%d ", arr[i]);
    }
    printf("]");
}

// prints an array of uint8_ts
// via loops and pointer arithmetic
void print_uint8_arr_v2 (uint8_t* arr, int len){
    printf("[ ");
    for (int i = 0; i < len; i++){
        // dereferences the place in memory i spots from the pointer start
        printf("%d ", *(arr + i));
    }
    printf("]");
}

// prints an array of uint16_ts
// via loops and pointer arithmetic
void print_uint16_arr (uint16_t* arr, int len){
    printf("[ ");
    for (int i = 0; i < len; i++){
        printf("%d ", *(arr + i));
    }
    printf("]");
}

// alters the value of a float to be 4 times greater
// via pointer and integer addition
void surgical_mult4(float* fp){
    uint8_t* cast_fp = (uint8_t*) fp;
    // move the pointer to the last section of bits
    // that houses the exponent values
    cast_fp += 3;
    // add one to the 2^1 place by dereferencing
    *cast_fp += 1;
}

// determines if a whole string is a substring of another string
// via loops and array/pointer syntax
char* find_substr(char* str1, char* str2){
    int match = 0;
    char* point = 0;

    int i = 0;
    while (str1[i] != '\0'){
        int j = 0;
        if (str2[j] == str1[i]){
            point = &str1[i];
            match = 1;
            int k = i;
            while (str2[j] != '\0'){
                if (str1[k] != str2[j]){
                    match = 0;
                }
            k++;
            j++;
            }
        }
        i++;
    }

    if (match == 1){
        return point;
    } else{
        return &str1[i];
    }
}

// prints a board representation
// via for loops and array syntax
void print_board(char** board, int n){
    for (int i = 0; i < n; i++){
        printf("[ ");
        for (int j = 0; j < n; j++){
            printf("%c ", board[i][j]);
        }
        printf("]\n");
    }
   printf("\n");
}

// moves a piece on the board
// via conditional statements and loops
void move_piece(char** board, int size, char cd){
    if (cd == 'n'){
        // pointer to whole row of a board
        char* temp = board[0];
        for (int i = 0; i < size-1; i++){
            board[i] = board[i+1];
        }
        board[size - 1] = temp;
    }

    if (cd == 's'){
        char* temp = board[size-1];
        for (int i = size-1; i > 0; i--){
            board[i] = board[i-1];
        }
        board[0] = temp;
    }

    if (cd == 'e'){
        for (int i = 0; i < size; i++){
            // variable holding a specific character on the board
            char temp = board[i][size - 1];
            for (int j = size - 1; j > 0; j--){
                board[i][j] = board[i][j - 1];
            }
            board[i][0] = temp;
        }
    }

    if (cd == 'w'){
        for (int i = 0; i < size; i++){
            char temp = board[i][0];
            for (int j = 0; j < size-1; j++){
                board[i][j] = board[i][j + 1];
            }
            board[i][size - 1] = temp;
        }
    }
}

// adds a value to the top of a stack
// via pointer syntax
void push(stack* stack, uint32_t x){
    node* newptr = (node*) malloc(sizeof(node));
    newptr -> val = x;
    if (stack -> top == NULL){
        newptr -> next = NULL;
    } else {
        newptr -> next = stack -> top;
    }
    stack -> top = newptr;
    stack -> numnodes += 1;
}

// prints a stack
// via pointer syntax
void print_stack(stack* stack){
    node* curr = stack -> top;
    for (int i = 0; i < stack -> numnodes; i++){
        printf("%d\n", curr -> val);
        curr = curr -> next;
    }
}

// pops a value from the top of a stack
// via pointer syntax
int pop(stack* stack){
    // error handling conditional
    if (stack -> numnodes != 0){
        node* popping = stack -> top;
        int value = popping -> val;
        stack -> numnodes -= 1;
        if (stack -> numnodes > 0){
            stack -> top = popping -> next;
            free(popping);
            return value;
        } else{
            free(popping);
            return 0;
    }
    } else {
        printf("There are no nodes to pop.\n");
        return 0;
    }   

}

// deallocates stack and all its nodes from memory
// via pointer syntax
void free_stack(stack* stack){
    int nodes = stack -> numnodes;
    node* curr = stack -> top;
    node* temp = curr -> next;
    for (int i = 0; i < nodes - 1; i++){
        printf("Currently freeing: %d\n", curr -> val);
        free(curr);
        curr = temp;
        temp = temp -> next;
    }
    printf("Currently freeing: %d\n", curr -> val);
    free(curr);
    printf("Now freeing the stack.\n");
    free(stack);
}