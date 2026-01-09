#include <stdio.h>

#include "base.h"
#include "stages.h"
#include "instructions.h"
#include "assembler.h"

// Given an assembly program, write it to memory and then run it.
void run_case(state* st, asm_line* lines, size_t len) {
  size_t out_len;
  asm_prog p = { .lines = lines, .length = len };
  uint32_t* prog = assemble(p, 0, &out_len);
  write_program(st, 0, prog, out_len);
  run_program(st, out_len);
  free(prog);
}

// Run an assembly program and ensure that the given register has the expected
// value afterwards. This function also checks that no other register values
// have changed. Returns 1 if the test passes and 0 if it fails.
int register_test(asm_line* lines, size_t len, uint8_t reg, int32_t expected) {
  int64_t initial[32];
  state st;
  for (int i = 0; i < 32; i++) {
    if (i % 2 == 0) {
      st.regs[i] = i;
    } else {
      st.regs[i] = -i;
    }
    initial[i] = st.regs[i];
  }
  st.hi_reg = 0;
  st.lo_reg = 0;
  st.pc = 0;
  st.memory = malloc(100 * sizeof(uint8_t));

  run_case(&st, lines, len);
  int res = 1;
  for (int i = 0; i < 32; i++) {
    if (i == reg) {
      if (st.regs[reg] != expected) {
        printf("Expected %d, got %d\n", expected, st.regs[reg]);
        res = 0;
      }
    } else {
      if (st.regs[i] != initial[i]) {
        res = 0;
      }
    }
  }

  free(st.memory);

  return res;
}

// Test that after running the given assembly program, the memory at the given
// address has the expected value. Returns 1 if the test passes and 0 if it
// fails.
int memory_test(asm_line* lines, size_t len, uint32_t addr, int32_t expected) {
  state st;
  for (int i = 0; i < 32; i++) {
    if (i % 2 == 0) {
      st.regs[i] = i;
    } else {
      st.regs[i] = -i;
    }
  }
  st.hi_reg = 0;
  st.lo_reg = 0;
  st.pc = 0;
  st.memory = malloc(100 * sizeof(uint8_t));

  run_case(&st, lines, len);

  int32_t mem_val = get_memory(&st, addr);
  int res = (mem_val == expected);
  if (!res) {
    printf("Expected %d, got %d\n", expected, mem_val);
  }

  free(st.memory);

  return res;

}

// Report a test result. The first input is expected to be the output of either
// register_test or memory_test. The message is a short identifier for the test
// that was run. Failures and total are modified to reflect the total number of
// tests that have been run and the total number of test failures.
void run_test(int succeeded, char* msg, int* failures, int* total) {
  if (!succeeded) {
    printf("Failed %s test\n", msg);
    (*failures)++;
  }
  (*total)++;
}

int main() {

  int failures = 0;
  int total = 0;

  asm_line beq_lines[] = {
    BEQ(2, 2, "branch"),
    ADDI(6, 0, 1),
    LABEL("branch"),
    ADDI(6, 6, 1)
  };

  asm_line jump_lines[] = {
    ADDI(5, 0, 1),   // r5 = 1
    J("target"),     // jump to "target"
    ADDI(5, 5, 100), // (should be skipped)
    LABEL("target"),
    ADDI(5, 5, 2)    // r5 = r5 + 2 = 3
  };

  asm_line jal_lines[] = {
    JAL("target"),  // jump and link → $ra = PC+4
    ADDI(4, 0, 4),  // skipped to start because of jump, but should end here with only 31 being altered
    J("end"),
    LABEL("target"),
    ADDI(4, 0, 5),  // $4 = 5 (to show we landed here)
    JR(31),         // return to caller using $ra
    LABEL("end"),           
  };

  asm_line add_lines[] = {
    ADD(4, 2, 3)  // r4 = r2 + r3 = -1
  };

  asm_line sub_lines[] = {
    SUB(4, 2, 3) // r4 = r2 - r3 = 5
  };

  asm_line sw_lines[] = {
    ADDI(2, 0, 42), // r2 = 42
    SW(2, 0, 20)    // mem[20] = r2
  };

  asm_line lw_lines[] = {
    ADDI(2, 0, 55),  // r2 = 55
    SW(2, 0, 12),    // mem[12] = 55
    LW(2, 0, 12)     // r4 = mem[12]
  };

  asm_line sw_overwrite_lines[] = {
    ADDI(2, 0, 1),  // r2 = 1
    SW(2, 0, 20),   // mem[20] = 1
    ADDI(2, 0, 2),  // r2 = 2
    SW(2, 0, 20)    // mem[20] = 2
  };

  asm_line div_lines_lo[] = {
    DIV(4, 2),   // LO = 4 / 2 = 2, HI = 4 % 2 = 0
    MFLO(4),     // $4 = LO = 2        
  };

  asm_line div_lines_hi[] = {
    DIV(4, 8),   // LO = 4 / 8 = 0, HI = 4 % 8 = 4
    MFHI(5)      // $5 = HI = 4
  };

  asm_line mult_lines_hi[] = {
    MULT(4, 8), // LO = 4 * 8 = 32
    MFLO(5)     // $5 = LO = 32
  };

  printf("\n\n------RUNNING TEST: jal------\n");
  run_test(register_test(jal_lines, 7, 31, 4), "jal", &failures, &total);
  printf("\n\n------RUNNING TEST: mult------\n");
  run_test(register_test(mult_lines_hi, 2, 5, 32), "mult", &failures, &total);
  printf("\n\n------RUNNING TEST: div lo------\n");
  run_test(register_test(div_lines_lo, 2, 4, 2), "div_lo", &failures, &total);
  printf("\n\n------RUNNING TEST: div hi------\n");
  run_test(register_test(div_lines_hi, 2, 5, 4), "div_hi", &failures, &total);
  printf("\n\n------RUNNING TEST: overwite------\n");
  run_test(memory_test(sw_overwrite_lines, 4, 20, 2), "sw overwrite", &failures, &total);
  printf("\n\n------RUNNING TEST: lw------\n");
  run_test(register_test(lw_lines, 3, 2, 55), "lw", &failures, &total);
  printf("\n\n------RUNNING TEST: sw------\n");
  run_test(memory_test(sw_lines, 2, 20, 42), "sw", &failures, &total);
  printf("\n\n------RUNNING TEST: add------\n");
  run_test(register_test(add_lines, 1, 4, -1), "add", &failures, &total);
  printf("\n\n------RUNNING TEST: sub------\n");
  run_test(register_test(sub_lines, 1, 4, 5), "sub", &failures, &total);
  printf("\n\n------RUNNING TEST: jump------\n");
  run_test(register_test(jump_lines, 5, 5, 3), "jump", &failures, &total);
  printf("\n\n------RUNNING TEST: beq------\n");
  run_test(register_test(beq_lines, 4, 6, 7), "beq", &failures, &total);

  printf("\n\nFailures: %d / %d\n", failures, total);
}

const char* alu_op_str(alu_op op) {
    switch(op) {
        case ADD: return "ADD"; 
        case AND: return "AND"; 
        case DIV: return "DIV";
        case EQ: return "EQ"; 
        case LT: return "LT"; 
        case MUL: return "MUL";
        case NEQ: return "NEQ"; 
        case OR: return "OR"; 
        case SLL: return "SLL";
        case SRA: return "SRA"; 
        case SRL: return "SRL"; 
        case SUB: return "SUB";
        case XOR: return "XOR";
        default: return "UNKNOWN_ALU_OP";
    }
}

const char* ctrl_op_str(control_op op) {
    switch(op) {
        case BRANCH: return "BRANCH"; 
        case HILO: return "HILO"; 
        case JUMP: return "JUMP";
        case JAL: return "JAL"; 
        case LW: return "LW"; 
        case SW: return "SW";
        case WB: return "WB";
        default: return "UNKNOWN_CTRL_OP";
    }
}

const char* res_op_str(resop op) {
    switch(op) {
        case LOAD: return "LOAD"; 
        case STORE: return "STORE"; 
        case WRITEBACK: return "WRITEBACK";
        case HILO_WB: return "HILO_WB"; 
        case NONE: return "NONE";
        default: return "UNKNOWN_RES_OP";
    }
}
