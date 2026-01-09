#pragma once

#include <stdlib.h>

// This struct represents the internal state of various pieces of data stored
// within our VM.
typedef struct {
  int32_t regs[32];  // CPU registers
  int32_t hi_reg;    // Special HI register
  int32_t lo_reg;    // Special LO register
  uint8_t* memory;   // Memory as an array of bytes
  size_t pc;         // Program counter
} state;

// Possible operations the ALU can perform.
typedef enum {
  ADD,  // addition
  AND,  // bitwise conjunction
  DIV,  // integer division
  EQ,   // equality check
  LT,   // less-than check
  MUL,  // multiplication
  NEQ,  // inequality check
  OR,   // bitwise disjunction
  SLL,  // left shift
  SRA,  // arithmetic right shift (inserting zeros or ones to maintain sign)
  SRL,  // logical right shift (always inserting zeros)
  SUB,  // subtraction
  XOR   // bitwise exclusive or
} alu_op;

// Possible operations for controlling jumps, memory, and register updates.
typedef enum {
  BRANCH,  // conditional branch
  HILO,    // write into the HI and LO registers
  JUMP,    // unconditional jump
  JAL,     // unconditional jump and link
  LW,      // load word from memory
  SW,      // store word to memory
  WB       // Write a value back to a register
} control_op;

// Decoded instruction
typedef struct {
  int32_t left;     // Left argument
  int32_t right;    // Right argument
  int32_t extra;    // Extra argument needed for some instructions
  uint8_t dest;     // Destination register index
  alu_op op;        // ALU operation
  control_op ctrl;  // Control operation
} instruction;

// Possible operations for memory and register updates.
typedef enum {
  LOAD,       // Load from memory
  STORE,      // Store to memory
  WRITEBACK,  // Write back to register
  HILO_WB,    // Write to the HI and LO registers
  NONE        // Do nothing
} resop;

// Result of executing an instruction
typedef struct {
  int32_t value;   // Arithmetic/logical result
  int32_t hi_val;  // Extra result needed by some instructions
  uint8_t dest;    // Destination register index
  resop op;        // Memory/register operation for later stages
  uint8_t jump;    // boolean, true if we need to modify the PC
} result;

int32_t alu(int32_t, int32_t, alu_op, int32_t*);
int32_t get_memory(state*, int32_t);
void set_memory(state*, int32_t, int32_t);
void write_program(state*, uint32_t, uint32_t*, size_t);
