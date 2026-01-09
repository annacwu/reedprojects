#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "assembler.h"
#include "instructions.h"

typedef struct {
  char* label;
  size_t offset;
} label_idx;

size_t lookup_label(label_idx* labels, size_t length, label lbl) {
  for (size_t i = 0; i < length; i++) {
    if (strcmp(lbl, labels[i].label) == 0) {
      return labels[i].offset;
    }
  }
  fprintf(stderr, "Failed to localte label: %s\n", lbl);
  exit(EXIT_FAILURE);
}

uint32_t* assemble(asm_prog program, uint32_t start, size_t* output_length) {
  size_t num_labels = 0;
  for (size_t i = 0; i < program.length; i++) {
    if (program.lines[i].tag == LABEL) {
      num_labels++;
    }
  }
  label_idx* labels = malloc(num_labels * sizeof(label_idx));
  size_t idx = 0;
  for (size_t i = 0; i < program.length; i++) {
    if (program.lines[i].tag == LABEL) {
      labels[idx].label = program.lines[i].data.lbl;
      labels[idx].offset = i - idx;
      idx++;
    }
  }
  *output_length = program.length - num_labels;
  uint32_t* output = malloc(*output_length * sizeof(uint32_t));
  idx = 0;
  for (size_t i = 0; i < program.length; i++) {
    if (program.lines[i].tag == INSTR) {
      asm_instr ins = program.lines[i].data.instr;
      switch (ins.op) {
      case _ADD:
        output[idx] = add(ins.reg1, ins.reg2, ins.reg3);
        break;
      case _ADDI:
        output[idx] = addi(ins.reg1, ins.reg2, ins.imm);
        break;
      case _AND:
        output[idx] = and(ins.reg1, ins.reg2, ins.reg3);
        break;
      case _ANDI:
        output[idx] = andi(ins.reg1, ins.reg2, ins.imm);
        break;
      case _BEQ:
        output[idx] = beq(ins.reg1, ins.reg2, lookup_label(labels, num_labels, ins.lbl) - idx);
        break;
      case _BNE:
        output[idx] = bne(ins.reg1, ins.reg2, lookup_label(labels, num_labels, ins.lbl) - idx);
        break;
      case _DIV:
        output[idx] = sdiv(ins.reg1, ins.reg2);
        break;
      case _J:
        output[idx] = j(lookup_label(labels, num_labels, ins.lbl) + start);
        break;
      case _JAL:
        output[idx] = jal(lookup_label(labels, num_labels, ins.lbl) + start);
        break;
      case _JR:
        output[idx] = jr(ins.reg1);
        break;
      case _LUI:
        output[idx] = lui(ins.reg1, ins.imm);
        break;
      case _LW:
        output[idx] = lw(ins.reg1, ins.reg2, ins.imm);
        break;
      case _MFHI:
        output[idx] = mfhi(ins.reg1);
        break;
      case _MFLO:
        output[idx] = mflo(ins.reg1);
        break;
      case _MULT:
        output[idx] = mult(ins.reg1, ins.reg2);
        break;
      case _OR:
        output[idx] = or(ins.reg1, ins.reg2, ins.reg3);
        break;
      case _ORI:
        output[idx] = ori(ins.reg1, ins.reg2, ins.imm);
        break;
      case _SLL:
        output[idx] = sll(ins.reg1, ins.reg2, ins.shamt);
        break;
      case _SLLV:
        output[idx] = sllv(ins.reg1, ins.reg2, ins.reg3);
        break;
      case _SLT:
        output[idx] = slt(ins.reg1, ins.reg2, ins.reg3);
        break;
      case _SLTI:
        output[idx] = slti(ins.reg1, ins.reg2, ins.imm);
        break;
      case _SRA:
        output[idx] = sra(ins.reg1, ins.reg2, ins.shamt);
        break;
      case _SRAV:
        output[idx] = srav(ins.reg1, ins.reg2, ins.reg3);
        break;
      case _SRL:
        output[idx] = srl(ins.reg1, ins.reg2, ins.shamt);
        break;
      case _SRLV:
        output[idx] = srlv(ins.reg1, ins.reg2, ins.reg3);
        break;
      case _SUB:
        output[idx] = sub(ins.reg1, ins.reg2, ins.reg3);
        break;
      case _SW:
        output[idx] = sw(ins.reg1, ins.reg2, ins.imm);
        break;
      case _XOR:
        output[idx] = xor(ins.reg1, ins.reg2, ins.reg3);
        break;
      case _XORI:
        output[idx] = xori(ins.reg1, ins.reg2, ins.imm);
        break;
      }
      idx++;
    }
  }
  free(labels);
  return output;
}

void free_prog(asm_prog p) {
  for (size_t i = 0; i < p.length; i++) {
    if (p.lines[i].tag == LABEL) {
      free(p.lines[i].data.lbl);
    }
  }
  free(p.lines);
}
