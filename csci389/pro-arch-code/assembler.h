#pragma once

#include <stdlib.h>

#define ZERO 0
#define AT 1
#define V0 2
#define V1 3
#define A0 4
#define A1 5
#define A2 6
#define A3 7
#define T0 8
#define T1 9
#define T2 10
#define T3 11
#define T4 12
#define T5 13
#define T6 14
#define T7 15
#define S0 16
#define S1 17
#define S2 18
#define S3 19
#define S4 20
#define S5 21
#define S6 22
#define S7 23
#define T8 24
#define T9 25
#define K0 26
#define K1 27
#define GP 28
#define SP 29
#define FP 30
#define RA 31

#define LABEL(l) { .tag = LABEL, .data = { .lbl = l } }
#define ADD(d, s, t) { .tag = INSTR, .data = { .instr = { .op = _ADD, .reg1 = d, .reg2 = s, .reg3 = t } } }
#define ADDI(d, s, i) { .tag = INSTR, .data = { .instr = { .op = _ADDI, .reg1 = d, .reg2 = s, .imm = i } } }
#define AND(d, s, t) { .tag = INSTR, .data = { .instr = { .op = _AND, .reg1 = d, .reg2 = s, .reg3 = t } } }
#define ANDI(d, s, i) { .tag = INSTR, .data = { .instr = { .op = _ANDI, .reg1 = d, .reg2 = s, .imm = i } } }
#define BEQ(s, t, l) { .tag = INSTR, .data = { .instr = { .op = _BEQ, .reg1 = s, .reg2 = t, .lbl = l } } }
#define BNE(s, t, l) { .tag = INSTR, .data = { .instr = { .op = _BNE, .reg1 = s, .reg2 = t, .lbl = l } } }
#define DIV(s, t) { .tag = INSTR, .data = { .instr = { .op = _DIV, .reg1 = s, .reg2 = t } } }
#define J(l) { .tag = INSTR, .data = { .instr = { .op = _J, .lbl = l } } }
#define JAL(l) { .tag = INSTR, .data = { .instr = { .op = _JAL, .lbl = l } } }
#define JR(l) { .tag = INSTR, .data = { .instr = { .op = _JR, .reg1 = l } } }
#define LUI(d, i) { .tag = INSTR, .data = { .instr = { .op = _LUI, .reg1 = d, .imm = i } } }
#define LW(d, s, o) { .tag = INSTR, .data = { .instr = { .op = _LW, .reg1 = d, .reg2 = s, .imm = o } } }
#define MFHI(d) { .tag = INSTR, .data = { .instr = { .op = _MFHI, .reg1 = d } } }
#define MFLO(d) { .tag = INSTR, .data = { .instr = { .op = _MFLO, .reg1 = d } } }
#define MULT(s, t) { .tag = INSTR, .data = { .instr = { .op = _MULT, .reg1 = s, .reg2 = t } } }
#define OR(d, s, t) { .tag = INSTR, .data = { .instr = { .op = _OR, .reg1 = d, .reg2 = s, .reg3 = t } } }
#define ORI(d, s, i) { .tag = INSTR, .data = { .instr = { .op = _ORI, .reg1 = d, .reg2 = s, .imm = i } } }
#define SLL(d, s, a) { .tag = INSTR, .data = { .instr = { .op = _SLL, .reg1 = d, .reg2 = s, .shamt = a } } }
#define SLLV(d, s, t) { .tag = INSTR, .data = { .instr = { .op = _SLLV, .reg1 = d, .reg2 = s, .reg3 = t } } }
#define SLT(d, s, t) { .tag = INSTR, .data = { .instr = { .op = _SLT, .reg1 = d, .reg2 = s, .reg3 = t } } }
#define SLTI(d, s, i) { .tag = INSTR, .data = { .instr = { .op = _SLTI, .reg1 = d, .reg2 = s, .imm = i } } }
#define SRA(d, s, a) { .tag = INSTR, .data = { .instr = { .op = _SRA, .reg1 = d, .reg2 = s, .shamt = a } } }
#define SRAV(d, s, t) { .tag = INSTR, .data = { .instr = { .op = _SRAV, .reg1 = d, .reg2 = s, .reg3 = t } } }
#define SRL(d, s, a) { .tag = INSTR, .data = { .instr = { .op = _SRL, .reg1 = d, .reg2 = s, .shamt = a } } }
#define SRLV(d, s, t) { .tag = INSTR, .data = { .instr = { .op = _SRLV, .reg1 = d, .reg2 = s, .reg3 = t } } }
#define SUB(d, s, t) { .tag = INSTR, .data = { .instr = { .op = _SUB, .reg1 = d, .reg2 = s, .reg3 = t } } }
#define SW(d, s, o) { .tag = INSTR, .data = { .instr = { .op = _SW, .reg1 = d, .reg2 = s, .imm = o } } }
#define XOR(d, s, t) { .tag = INSTR, .data = { .instr = { .op = _XOR, .reg1 = d, .reg2 = s, .reg3 = t } } }
#define XORI(d, s, i) { .tag = INSTR, .data = { .instr = { .op = _XORI, .reg1 = d, .reg2 = s, .imm = i } } }

typedef enum {
  _ADD, _ADDI, _AND, _ANDI, _BEQ, _BNE, _DIV, _J, _JAL, _JR, _LUI, _LW, _MFHI,
  _MFLO, _MULT, _OR, _ORI, _SLL, _SLLV, _SLT, _SLTI, _SRA, _SRAV, _SRL, _SRLV,
  _SUB, _SW, _XOR, _XORI
} asm_op;

typedef char* label;

typedef struct {
  asm_op op;
  uint8_t reg1;
  uint8_t reg2;
  uint8_t reg3;
  uint8_t shamt;
  int16_t imm;
  label lbl;
} asm_instr;

typedef struct {
  enum { LABEL, INSTR } tag;
  union { label lbl; asm_instr instr; } data;
} asm_line;

typedef struct {
  asm_line* lines;
  size_t length;
} asm_prog;

uint32_t* assemble(asm_prog, uint32_t, size_t*);

void free_prog(asm_prog);
