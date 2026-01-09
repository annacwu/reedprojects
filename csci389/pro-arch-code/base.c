#include "base.h"

// Simulate the arithmetic logic unit
// The `hi` argument is used by instructions which interact with the HI and LO
// registers. In these cases, the return value should be stored in LO and the
// value to be stored in HI will be placed at the address pointed to by `hi`.
int32_t alu(int32_t left, int32_t right, alu_op op, int32_t* hi) {
  int64_t res;
  switch (op) {
  case ADD:
    return left + right;
  case AND:
    return left & right;
  case DIV:
    *hi = left % right;
    return left / right;
  case EQ:
    return left == right;
  case LT:
    return left < right;
  case MUL:
    res = left * (int64_t) right;
    *hi = res >> 32;
    return res & 0xFFFFFFFF;
  case NEQ:
    return left != right;
  case OR:
    return left | right;
  case SLL:
    return left << right;
  case SRA:
    if (left < 0) {
      return left >> right | 0xFFFFFFFF << (32 - right);
    } else {
      return (uint32_t) left >> (uint32_t) right;
    }
  case SRL:
    return (int32_t) ((uint32_t) left >> right);
  case SUB:
    return left - right;
  case XOR:
    return left ^ right;
  }
}

// Simulated memory lookup
int32_t get_memory(state* st, int32_t addr) {
  uint32_t val = 0;
  val |= st->memory[addr  ] << 24;
  val |= st->memory[addr+1] << 16;
  val |= st->memory[addr+2] <<  8;
  val |= st->memory[addr+3]      ;
  return val;
}

// Simulated memory modification
void set_memory(state* st, int32_t addr, int32_t value) {
  st->memory[addr  ] =  value >> 24         ;
  st->memory[addr+1] = (value >> 16) & 0xFF;
  st->memory[addr+2] = (value >>  8) & 0xFF;
  st->memory[addr+3] = (value      ) & 0xFF;
}

void write_program(state* st, uint32_t addr, uint32_t* prog, size_t len) {
  for (size_t i = 0; i < len; i++) {
    set_memory(st, addr + 4 *  i, prog[i]);
  }
}
