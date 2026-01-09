#pragma once

#include <stdlib.h>

static inline uint32_t r_format(uint8_t rd, uint8_t rs, uint8_t rt, uint8_t shamt, uint8_t funct) {
  return (rs << 21) | (rt << 16) | (rd << 11) | (shamt << 6) | funct;
}

static inline uint32_t i_format(uint8_t opcode, uint8_t rt, uint8_t rs, uint16_t imm) {
  return (opcode << 26) | (rs << 21) | (rt << 16) | imm;
}

static inline uint32_t add(uint8_t rd, uint8_t rs, uint8_t rt) {
  return r_format(rd, rs, rt, 0, 0x20);
}

static inline uint32_t addi(uint8_t rt, uint8_t rs, int16_t imm) {
  return i_format(0x08, rt, rs, imm);
}

static inline uint32_t and(uint8_t rd, uint8_t rs, uint8_t rt) {
  return r_format(rd, rs, rt, 0, 0x24);
}

static inline uint32_t andi(uint8_t rt, uint8_t rs, int16_t imm) {
  return i_format(0x0C, rt, rs, imm);
}

static inline uint32_t beq(uint8_t rt, uint8_t rs, int16_t offset) {
  return i_format(0x04, rt, rs, offset);
}

static inline uint32_t bne(uint8_t rt, uint8_t rs, int16_t offset) {
  return i_format(0x5, rt, rs, offset);
}

static inline uint32_t sdiv(uint8_t rs, uint8_t rt) {
  return r_format(0, rs, rt, 0, 0x1A);
}

static inline uint32_t j(uint32_t address) {
  return (0x02 << 26) | address;
}

static inline uint32_t jal(uint32_t address) {
  return (0x03 << 26) | address;
}

static inline uint32_t jr(uint8_t rs) {
  return r_format(0, rs, 0, 0, 0x08);
}

static inline uint32_t lui(uint8_t rt, int16_t imm) {
  return i_format(0x0F, rt, 0, imm);
}

static inline uint32_t lw(uint8_t rt, uint8_t rs, int16_t imm) {
  return i_format(0x23, rt, rs, imm);
}

static inline uint32_t mfhi(uint8_t rd) {
  return r_format(rd, 0, 0, 0, 0x10);
}

static inline uint32_t mflo(uint8_t rd) {
  return r_format(rd, 0, 0, 0, 0x12);
}

static inline uint32_t mult(uint8_t rs, uint8_t rt) {
  return r_format(0, rs, rt, 0, 0x18);
}

static inline uint32_t or(uint8_t rd, uint8_t rs, uint8_t rt) {
  return r_format(rd, rs, rt, 0, 0x25);
}

static inline uint32_t ori(uint8_t rt, uint8_t rs, int16_t imm) {
  return i_format(0x0D, rt, rs, imm);
}

static inline uint32_t sll(uint8_t rd, uint8_t rt, uint8_t shamt) {
  return r_format(rd, 0, rt, shamt, 0x00);
}

static inline uint32_t sllv(uint8_t rd, uint8_t rs, uint8_t rt) {
  return r_format(rd, rs, rt, 0, 0x04);
}

static inline uint32_t slt(uint8_t rd, uint8_t rs, uint8_t rt) {
  return r_format(rd, rs, rt, 0, 0x2A);
}

static inline uint32_t slti(uint8_t rt, uint8_t rs, int16_t imm) {
  return i_format(0x0A, rt, rs, imm);
}

static inline uint32_t sra(uint8_t rd, uint8_t rt, uint8_t shamt) {
  return r_format(rd, 0, rt, shamt, 0x03);
}

static inline uint32_t srav(uint8_t rd, uint8_t rs, uint8_t rt) {
  return r_format(rd, rs, rt, 0, 0x07);
}

static inline uint32_t srl(uint8_t rd, uint8_t rt, uint8_t shamt) {
  return r_format(rd, 0, rt, shamt, 0x02);
}

static inline uint32_t srlv(uint8_t rd, uint8_t rs, uint8_t rt) {
  return r_format(rd, rs, rt, 0, 0x06);
}

static inline uint32_t sub(uint8_t rd, uint8_t rs, uint8_t rt) {
  return r_format(rd, rs, rt, 0, 0x22);
}

static inline uint32_t sw(uint8_t rt, uint8_t rs, int16_t imm) {
  return i_format(0x2B, rt, rs, imm);
}

static inline uint32_t xor(uint8_t rd, uint8_t rs, uint8_t rt) {
  return r_format(rd, rs, rt, 0, 0x26);
}

static inline uint32_t xori(uint8_t rt, uint8_t rs, int16_t imm) {
  return i_format(0x0E, rt, rs, imm);
}
