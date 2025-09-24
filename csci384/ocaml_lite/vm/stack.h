/**
 * @file   stack.h
 * @author Greg Anderson
 * @date   2025-07-22
 * @brief  Defines a stack of OVM objects.
 *
 * This file defines the stack data type underlying the OVM. The functions for 
 * interacting with a stack are mostly quite small, so we define them in the 
 * header to allow the compiler to inline them aggressively.
 */

#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "types.h"

/** @brief A stack of OVM objects.
 *
 * The OVM stack is implemented using a dynamically sized array for efficiency.
 */
typedef struct {
    object* values;   ///< The current elements of the stack.
    size_t size;      ///< The number of elements on the stack.
    size_t capacity;  ///< The size of the values array.
} stack;

/** @brief Create a new, empty stack with the given capacity.
 *
 * @param cap The initial capacity of the new stack.
 * @return A reference to a new stack object.
 */
static inline stack* stack_create(size_t cap) {
    stack* st = (stack*) malloc(sizeof(stack));
    st->values = (object*) malloc(cap * sizeof(object));
    st->size = 0;
    st->capacity = cap; return st;
}

/** @brief Add a new value to the top of the stack.
 *
 * @param st The stack to push the new value onto.
 * @param v The value to push.
 */
static inline void stack_push(stack* st, object v) {
    // If we're out of space, we double the capacity of the stack.
    if (st->size >= st->capacity) {
        st->capacity *= 2;
        st->values = (object*) realloc(st->values, st->capacity * sizeof(object));
    }
    st->values[st->size] = v;
    st->size++;
}

/** @brief Add a new integer value to the top of the stack.
 *
 * This function pushes an integer onto the stack and aditionally tags the 
 * value by inserting a 1 in the least significant bit. The tag helps the 
 * garbage collector operate efficiently and correctly.
 *
 * @param st The stack to push the new value onto.
 * @param i The integer to push onto the stack.
 */
static inline void stack_push_int(stack* st, int64_t i) {
    // Tag the integer in the least significant bit before pushing.
    object o;
    o.i = i << 1;
    o.i |= 1;
    stack_push(st, o);
}

/** @brief Remove and return the top value from the stack.
 *
 * @param st The stack to pop a value from.
 * @return The object from the top of the stack.
 */
static inline object stack_pop(stack* st) {
    if (st->size == 0) {
        fprintf(stderr, "Tried to pop from empty stack\n");
        exit(EXIT_FAILURE);
    }
    st->size--;
    return st->values[st->size];
}

/** @brief Remove and return an integer from the top of the stack.
 *
 * Pops a value from the top of the stack and un-tags it to recover the actual
 * integer value.
 *
 * @param st The stack to pop a value from.
 * @return An integer popped from the top of the stack.
 */
static inline int64_t stack_pop_int(stack* st) {
    object v = stack_pop(st);
    // Untag the integer before returning.
    return v.i >> 1;
}

/** @brief Return an element by index from the bottom of the stack.
 *
 * @param st The stack to retrieve a value from.
 * @param index The location on the stack to retrieve.
 * @return The value on the stack at the given index.
 */
static inline object stack_peek(stack* st, size_t index) {
    return st->values[index];
}

/** @brief Print the values of a stack.
 *
 * This function prints all values from the stack. Note that it will print the 
 * values as integers regardless of their actual type because the stack has no 
 * knowledge of the type of objects (outside of the pointer/not-a-pointer tag 
 * bit).
 *
 * @param st The stack to print.
 */
static inline void stack_print(stack* st) {
    for (size_t i = 0; i < st->size; i++) {
        int64_t v = st->values[i].i;
        if (v & 1) {  // Untag integer values
            v >>= 1;
        }
        printf("%ld ", v);
    }
    printf("\n");
}

/** @brief Remove all values from the stack.
 *
 * @param st The stack to clear.
 */
static inline void stack_clear(stack* st) {
    st->size = 0;
}

/** @brief Free a stack.
 *
 * The frees all memory associated with `st` including the stack structure
 * itself. Attempting to access any fields of `st` after deleting it may result
 * in errors.
 *
 * @param st The stack object to delete.
 */
static inline void stack_delete(stack* st) {
    free(st->values);
    free(st);
}

/** @brief Get the number of elements of a stack.
 *
 * @param st The stack to get the size of.
 */
static inline size_t stack_size(stack* st) {
    return st->size;
}
