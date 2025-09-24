/**
 * @file   main.c
 * @author Greg Anderson
 * @date   2025-07-22
 * @brief  The main code for the OVM.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "stack.h"
#include "readfile.h"
#include "alloc.h"

// Shared global OVM state, declared in machine_state.c
extern stack* st;
extern object* current_env;

/** @brief Read a constant from the code.
 *
 * @param cst The byte stream to read a constant from.
 * @param to_read The number of bytes to read from the stream.
 */
int64_t read_constant(uint8_t *cst, size_t to_read) {
    int64_t ret = 0;
    for (size_t i = 0; i < to_read; i++) {
        ret <<= 8;
        ret |= cst[i];
    }
    return ret;
}

/** @brief Copy an environment.
 * 
 * Creates a shallow copy of a given environment. That is, each element is
 * copied as a machine word, but we do not follow pointers and recursively copy
 * referenced objects. This function expects that the parameter copy is an
 * already-allocated object array with size sufficient to hold expeceted_size
 * objects.
 *
 * @param copy The location to store the copy in.
 * @param env The environment to copy.
 * @param env_size The number of elements to copy.
 * @param expected_size The total size of the new environment.
 */
void copy_env(object* copy, object* env, size_t env_size, size_t expected_size) {
#ifdef DEBUG
    printf("Copying environment from %p to %p\n", env, copy);
#endif
    for (size_t i = 0; i < env_size; i++) {
        // Copy each element from the old environment.
#ifdef DEBUG
        printf("Copying env value %d: %p\n", i, env[i]);
#endif
        copy[i] = env[i];
    }
    for (size_t i = env_size; i < expected_size; i++) {
        // Fill the rest of the elements with 1 so that the garbage collector
        // sees these as non-pointers and doesn't try to dereference them.
        copy[i].i = 1;
    }
}

/** @brief Allocate space for a new closure object.
 *
 * @param env_size The size of the environment of the new closure.
 * @param local1 A local object which should not be deleted by the garbage collector.
 * @param local2 A local object which should not be deleted by the garbage collector.
 * @return A pointer to the newly-allocated closure.
 */
closure* allocate_closure(size_t env_size, object* local1, object* local2) {
    // This fake object is used to tell the garbage collector that we don't need
    // to retain a third object. It has its value set to 1 so the collector sees
    // it as a non-pointer value.
    object fake;
    OBJ_INT(fake) = 1;
    closure* ret = ovm_alloc((1 + env_size) * sizeof(object), CLOSURE, local1, local2, &fake);
    return ret;
}

/** @brief Create a (shallow) copy of a closure.
 *
 * This function copies all the elements of a closure but does not recursively
 * deep copy the environment. This is sufficient since OCaml-lite has immutable
 * data, so multiple references to the same object are safe. We do need to copy
 * the environment array because otherwise two invocations of a closure could
 * overwrite the same argument index.
 *
 * @param src The closure to copy.
 * @param local1 An object that should not be deleted by the garbage collector.
 * @param local2 An object that should not be deleted by the garbage collector.
 * @return A new closure object which is a copy of src.
 */
closure* copy_closure(closure* src, object* local1, object* local2) {
    closure* ret = allocate_closure(cl_expected_size(src), local1, local2);
    set_code_ptr(ret, cl_code_ptr(src));
    set_env_size(ret, cl_env_size(src));
    set_expected_size(ret, cl_expected_size(src));
    copy_env(cl_env(ret), cl_env(src), cl_env_size(src), cl_expected_size(src));
    return ret;
}

/** @brief Push a stack frame onto the stack before a function call.
 *
 * When calling a function, we need to push the current stack pointer, the
 * current environment, and the current program counter (as a return location).
 *
 * @param pc The program counter.
 * @param sp The current stack pointer.
 */
void push_stack_frame(size_t pc, size_t sp) {
    object z;
    OBJ_SP(z) = sp;              // Push the current stack pointer.
    stack_push_int(st, OBJ_INT(z));
    OBJ_ENV(z) = current_env;    // Push the current environment.
    stack_push(st, z);
#ifdef DEBUG
    printf("Pushing a stack frame, PC=%lx\n", pc);
#endif
    OBJ_SP(z) = pc;              // Push the current instruction index
    stack_push_int(st, OBJ_INT(z));
}

/** @brief Execute a CLOSE or CLOSREC instruction.
 *
 * This function creates a new closure and inserts captured variables from the
 * OVM stack into the closure environment.
 *
 * @param code The code that is being executed.
 * @param pc A pointer to the program counter.
 * @param recursive A boolean indicating whether this closure is recursive.
 */
void handle_close(Bytes code, size_t* pc, int recursive) {
    uint8_t total_size = read_constant(code.array + *pc + 1, 1);
    object fake;
    OBJ_INT(fake) = 1;
    closure* f = allocate_closure(total_size, &fake, &fake);
#ifdef DEBUG
    printf("Closure is at %p\n", f);
#endif
    uint8_t env_size = read_constant(code.array + *pc + 2, 1);
    uint8_t code_ptr = read_constant(code.array + *pc + 3, 1);
    *pc += 3;
    set_expected_size(f, total_size);
    size_t start = recursive ? env_size + 1 : env_size;
    set_env_size(f, start);
    set_code_ptr(f, code_ptr);
    for (int i = 0; i < env_size; i++) {
        object t = stack_pop(st);
        set_env(f, env_size - i - 1, t);
    }
    for (size_t i = start; i < cl_expected_size(f); i++) {
        // Fill the rest of the environment with fake objects for the garbage
        // collector.
        set_env(f, i, fake);
    }
    object obj;
    OBJ_CLOS(obj) = f;
    if (recursive) {
        set_env(f, env_size, obj);
    }
    stack_push(st, obj);
}

/** @brief Handle a call instruction
 *
 * This function is a unified implementation of CALL, CALLPOP, CALLN, and
 * CALLNP.
 *
 * @param code The code being executed.
 * @param fn_ptrs The function pointer array for the bytecode file.
 * @param pc The program counter.
 * @param sp The stack pointer.
 * @param args The number of arguments the function is being applied to.
 * @param pops The number of POP1 instructions to execute.
 */
void handle_call(Bytes code, size_t* fn_ptrs, size_t* pc,
                 size_t* sp, uint8_t args, uint8_t pops) {
    object cl_obj = stack_pop(st);
    closure* cl = OBJ_CLOS(cl_obj);  // Pop the closure
    object ncl;
#ifdef DEBUG
    printf("Popping a closure: stack size is now %ld\n", st->size);
#endif
    object* old_env;
    object fake;
    OBJ_INT(fake) = 1;
    if (cl_env_size(cl) + args == cl_expected_size(cl)) {
#ifdef DEBUG
        printf("Function fully applied\n");
#endif
        old_env = current_env;
        object oe;
        OBJ_ENV(oe) = old_env;
        current_env = ovm_alloc(cl_expected_size(cl) * sizeof(object),
                                ENV, &oe, &cl_obj, &fake);
        cl = OBJ_CLOS(cl_obj);   // cl_obj may be moved by the ovm_alloc call
        old_env = OBJ_ENV(oe);
        copy_env(current_env, cl_env(cl), cl_env_size(cl),
                 cl_expected_size(cl));
        for (int i = 0; i < args; i++) {
            object t = stack_pop(st);
            current_env[cl_env_size(cl) + i] = t;
        }
        for (int i = 0; i < pops; i++) {
            // Execute the implicit POP1 instructions
            stack_pop(st);
#ifdef DEBUG
            printf("Popping for cleanup: stack size is now %ld\n", st->size);
#endif
        }
        if (*pc < code.length - 1 && code.array[*pc + 1] != 0x42) {
            // We only push a stack frame if this is not a tail call.
            object* tmp = current_env;
            current_env = old_env;
            push_stack_frame(*pc, *sp);
            current_env = tmp;
        }
        *sp = stack_size(st);
#ifdef DEBUG
        printf("Jumping to function: %lx\n", fn_ptrs[cl_code_ptr(cl)]);
#endif
        *pc = fn_ptrs[cl_code_ptr(cl)] - 1;
    } else {
#ifdef DEBUG
        printf("Function partially applied\n");
#endif
        OBJ_CLOS(ncl) = copy_closure(cl, &cl_obj, &fake);
        for (int i = 0; i < args; i++) {
            object t = stack_pop(st);
            set_env(OBJ_CLOS(ncl), cl_env_size(OBJ_CLOS(ncl)) + i, t);
        }
        for (int i = 0; i < pops; i++) {
            // Execute the implicit POP1 instructions
            stack_pop(st);
#ifdef DEBUG
            printf("Popping for cleanup: stack size is now %ld\n", st->size);
#endif
        }
        set_env_size(OBJ_CLOS(ncl), cl_env_size(OBJ_CLOS(ncl)) + args);
        stack_push(st, ncl);
    }
}

static inline void constant(Bytes code, size_t* pc) {
    int64_t v = read_constant(code.array + *pc + 1, 8);
    stack_push_int(st, v);
    *pc += 8;
}

static inline void sconst(Bytes code, size_t* pc) {
    object fake;
    OBJ_INT(fake) = 1;
    int64_t len = strlen((char*) code.array + *pc + 1);
    char* str = ovm_alloc(len + 1, STRING, &fake, &fake, &fake);
    strcpy(str, (char*) code.array + *pc + 1);
    str[len] = '\0';
    object obj;
    OBJ_STR(obj) = str;
    stack_push(st, obj);
    *pc += len + 1;
}

static inline void add() {
    int64_t x = stack_pop_int(st);
    int64_t y = stack_pop_int(st);
    stack_push_int(st, x + y);
}

static inline void sub() {
    int64_t x = stack_pop_int(st);
    int64_t y = stack_pop_int(st);
    stack_push_int(st, y - x);
}

static inline void mul() {
    int64_t x = stack_pop_int(st);
    int64_t y = stack_pop_int(st);
    stack_push_int(st, x * y);
}

static inline void divide() {
    int64_t y = stack_pop_int(st);
    if (y == 0) {
        fprintf(stderr, "Divide by zero\n");
        exit(EXIT_FAILURE);
    }
    int64_t x = stack_pop_int(st);
    stack_push_int(st, x / y);
}

static inline void neg() {
    int64_t x = stack_pop_int(st);
    stack_push_int(st, -x);
}

static inline void eq() {
    object y = stack_pop(st);
    object x = stack_pop(st);
    int64_t z;
    if (OBJ_INT(y) & 1) {  // If this is an integer, use integer comparison.
        z = OBJ_INT(x) == OBJ_INT(y);
    } else {  // Otherwise, we assume this is a string.
        z = strcmp(OBJ_STR(x), OBJ_STR(y)) == 0;
    }
    stack_push_int(st, z);
}

static inline void neq() {
    object y = stack_pop(st);
    object x = stack_pop(st);
    int64_t z;
    if (OBJ_INT(y) & 1) {  // If this is an integer, use integer comparison.
        z = OBJ_INT(x) != OBJ_INT(y);
    } else {  // Otherwise, we assume this is a string.
        z = strcmp(OBJ_STR(x), OBJ_STR(y)) != 0;
    }
    stack_push_int(st, z);
}

static inline void leq() {
    int64_t y = stack_pop_int(st);
    int64_t x = stack_pop_int(st);
    stack_push_int(st, x <= y);
}

static inline void lt() {
    int64_t y = stack_pop_int(st);
    int64_t x = stack_pop_int(st);
    stack_push_int(st, x < y);
}

static inline void and() {
    int64_t y = stack_pop_int(st);
    int64_t x = stack_pop_int(st);
    stack_push_int(st, x && y);
}

static inline void or() {
    int64_t y = stack_pop_int(st);
    int64_t x = stack_pop_int(st);
    stack_push_int(st, x || y);
}

static inline void not() {
    int64_t x = stack_pop_int(st);
    stack_push_int(st, !x);
}

static inline void mod() {
    int64_t y = stack_pop_int(st);
    if (y == 0) {
        fprintf(stderr, "Mod by zero\n");
        exit(EXIT_FAILURE);
    }
    int64_t x = stack_pop_int(st);
    stack_push_int(st, x % y);
}

static inline void concat() {
    object y = stack_pop(st);
    object x = stack_pop(st);
    object fake;
    OBJ_INT(fake) = 1;
    object obj;
    OBJ_STR(obj) = ovm_alloc(strlen(OBJ_STR(x)) + strlen(OBJ_STR(y)) + 1,
                             STRING, &x, &y, &fake);
    sprintf(OBJ_STR(obj), "%s%s", OBJ_STR(x), OBJ_STR(y));
    stack_push(st, obj);
}

static inline void charat() {
    object y = stack_pop(st);
    object x = stack_pop(st);
    int64_t len = (int64_t) strlen(OBJ_STR(x));
    if (OBJ_INT(y) >= len) {
        fprintf(stderr, "Attempting to access past the end of a string: index %ld of %ld",
                OBJ_INT(y), len);
    }
    object fake;
    OBJ_INT(fake) = 1;
    object obj;
    OBJ_STR(obj) = ovm_alloc(2, STRING, &x, &y, &fake);
    OBJ_STR(obj)[0] = OBJ_STR(x)[OBJ_INT(y)];
    OBJ_STR(obj)[1] = '\0';
    stack_push(st, obj);
}

static inline void br(size_t* pc) {
    int64_t y = stack_pop_int(st);
    int64_t x = stack_pop_int(st);
    if (x) {
        *pc += y;
    }
}

static inline void jmp(size_t* pc) {
    int64_t x = stack_pop_int(st);
    *pc += x;
}

static inline void ret(size_t* pc, size_t* sp) {
    object x = stack_pop(st);        // Get the return value of the function
    object y;
    OBJ_INT(y) = stack_pop_int(st);  // Restore the program counter
    *pc = OBJ_SP(y);
#ifdef DEBUG
    printf("Returning to %lx\n", *pc);
#endif
    y = stack_pop(st);        // Restore the environment.
    current_env = OBJ_ENV(y);
    y.i = stack_pop_int(st);  // Restore the stack pointer
    *sp = OBJ_SP(y);
    stack_push(st, x);  // Put the return value back on the stack.
}

static inline void callpop(Bytes code, size_t* pc, size_t* sp, size_t* fn_ptrs) {
    int64_t z = read_constant(code.array + *pc + 1, 1);
    *pc += 1;
    handle_call(code, fn_ptrs, pc, sp, 1, z);
}

static inline void calln(Bytes code, size_t* pc, size_t* sp, size_t* fn_ptrs) {
    int64_t x = read_constant(code.array + *pc + 1, 1);
    *pc += 1;
    handle_call(code, fn_ptrs, pc, sp, x, 0);
}

static inline void callnp(Bytes code, size_t* pc, size_t* sp, size_t* fn_ptrs) {
    int64_t x = read_constant(code.array + *pc + 1, 1);
    int64_t z = read_constant(code.array + *pc + 2, 1);
    *pc += 2;
    handle_call(code, fn_ptrs, pc, sp, x, z);
}

static inline void over(Bytes code, size_t* pc, size_t* sp) {
    int64_t x = read_constant(code.array + *pc + 1, 1);
    stack_push(st, stack_peek(st, *sp + x));
    *pc += 1;
}

static inline void pop1() {
    object x = stack_pop(st);
    stack_pop(st);
    stack_push(st, x);
}

static inline void env(Bytes code, size_t* pc) {
    int64_t x = read_constant(code.array + *pc + 1, 1);
    *pc += 1;
    object y = current_env[x];
    stack_push(st, y);
}

static inline void cons(Bytes code, size_t* pc) {
    int64_t x = read_constant(code.array + *pc + 1, 1);
    int64_t y = read_constant(code.array + *pc + 2, 1);
    *pc += 2;
    object fake;
    OBJ_INT(fake) = 1;
    object* obj = ovm_alloc((y + 1) * sizeof(object), ENV, &fake, &fake, &fake);
    object tag;
    OBJ_INT(tag) = (x << 1) | 1;
    obj[0] = tag;
    for (int i = 0; i < y; i++) {
        obj[y - i] = stack_pop(st);
    }
    object data;
    OBJ_ENV(data) = obj;
    stack_push(st, data);
}

static inline void gettag() {
    object x = stack_pop(st);
    stack_push(st, x);
    stack_push(st, OBJ_ENV(x)[0]);
#ifdef DEBUG
    printf("Got tag of %p: it was %d\n", OBJ_ENV(x), OBJ_ENV(x)[0]);
#endif
}

static inline void decons(Bytes code, size_t* pc) {
    int64_t x = read_constant(code.array + *pc + 1, 1);
    *pc += 1;
    object y = stack_pop(st);
    for (int i = 0; i < x; i++) {
        stack_push(st, OBJ_ENV(y)[i+1]);
    }
}

static inline void print() {
    object x = stack_pop(st);
    printf("%s\n", OBJ_STR(x));
    stack_push_int(st, 0);
}

static inline void stoi() {
    object x = stack_pop(st);
    int64_t i = atoi(OBJ_STR(x));
    stack_push_int(st, i);
}

static inline void itos() {
    int64_t x = stack_pop_int(st);
    object fake;
    OBJ_INT(fake) = 1;
    object z;
    OBJ_STR(z) = ovm_alloc(20, STRING, &fake, &fake, &fake);
    sprintf(OBJ_STR(z), "%ld", x);
    stack_push(st, z);
}

static inline void error() {
    object x = stack_pop(st);
    fprintf(stderr, "%s\n", OBJ_STR(x));
    exit(EXIT_FAILURE);
}

static inline void strlength() {
    object x = stack_pop(st);
    stack_push_int(st, strlen(OBJ_STR(x)));
}

/** @brief The main OVM entry point.
 *
 * @param code The code to run.
 * @param fn_ptrs The function pointer array from the bytecode file being run.
 */
void interpret(Bytes code, size_t* fn_ptrs) {
    // The program counter. fn_ptrs[0] always points to main.
    size_t pc = fn_ptrs[0];
    size_t sp = 0;   // Points to the bottom of the current stack frame
    while (pc < code.length) {
#ifdef DEBUG
        printf("PC: %lx, instr: %x, stack size: %ld, stack_top: 0x%lx\n", pc, code.array[pc], st->size, st->values[st->size - 1]);
#endif
        // Most of this function is a big switch statement checking the opcode
        // at the current point in the program and interacting with the stack
        // appropriately.
        switch (code.array[pc]) {
        case 0x07: constant(code, &pc); break;
        case 0x0F: sconst(code, &pc); break;
        case 0x20: add(); break;
        case 0x21: sub(); break;
        case 0x22: mul(); break;
        case 0x23: divide(); break;
        case 0x24: neg(); break;
        case 0x25: eq(); break;
        case 0x26: neq(); break;
        case 0x27: leq(); break;
        case 0x28: lt(); break;
        case 0x29: and(); break;
        case 0x2A: or(); break;
        case 0x2B: not(); break;
        case 0x2C: mod(); break;
        case 0x2D: concat(); break;
        case 0x2E: charat(); break;
        case 0x30: br(&pc); break;
        case 0x31: jmp(&pc); break;
        case 0x40: handle_close(code, &pc, 0); break;
        case 0x41: handle_call(code, fn_ptrs, &pc, &sp, 1, 0); break;
        case 0x42: ret(&pc, &sp); break;
        case 0x43: handle_close(code, &pc, 1); break;
        case 0x44: callpop(code, &pc, &sp, fn_ptrs); break;
        case 0x45: calln(code, &pc, &sp, fn_ptrs); break;
        case 0x46: callnp(code, &pc, &sp, fn_ptrs); break;
        case 0x50: over(code, &pc, &sp); break;
        case 0x51: pop1(); break;
        case 0x52: env(code, &pc); break;
        case 0x60: cons(code, &pc); break;
        case 0x61: gettag(); break;
        case 0x62: decons(code, &pc); break;
        case 0xF0: print(); break;
        case 0xF1: stoi(); break;
        case 0xF2: itos(); break;
        case 0xF3: error(); break;
        case 0xF4: strlength(); break;
        default:
            fprintf(stderr, "Unrecognized opcode: %x\n", code.array[pc]);
            exit(EXIT_FAILURE);
        }
        pc++;
    }
#ifdef DEBUG
    printf("Final PC: %lx", pc);
#endif
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ovm <bytecode-file>\n");
        exit(EXIT_FAILURE);
    }
    size_t* fn_ptrs;
    size_t num_fns;
    Bytes code = read_bytecode_file(argv[1], &fn_ptrs, &num_fns);
    st = stack_create(1024);
    ovm_allocator_init();
    object tmp;
    tmp.i = 1;
    current_env = ovm_alloc(0, ENV, &tmp, &tmp, &tmp);
    interpret(code, fn_ptrs);
    stack_clear(st);
    ovm_allocator_free();
    stack_delete(st);
    free(fn_ptrs);
    free(code.array);
}
