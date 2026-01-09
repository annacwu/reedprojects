#include <stdio.h>

#include "stages.h"
#include "stage_test.h"

// Get the next instruction
uint32_t fetch(state* st) {
    return get_memory(st, st->pc);
}

// Decode the instruction

// helper function to decode an r-format instruction
instruction r_format(state* st, uint32_t instr) {
    instruction res;
    // 5 bit mask
    uint32_t m = 0b00000011111;
    // load data from source registers
    res.left = st->regs[(instr >> 21) & m];
    res.right = st->regs[(instr >> 16) & m]; 
    res.dest = (instr >> 11) & m;
    res.extra = (instr >> 6) & m; // this will mostly only be used in shift
    // 6 bit mask for funct code
    uint32_t funct_m = 0b111111;
    uint32_t funct = instr & funct_m;
    switch(funct) {
    case 0b100000: // add
        res.op = ADD;
        res.ctrl = WB;
        break;
    case 0b100100: // and
        res.op = AND;
        res.ctrl = WB;
        break;
    case 0b011010: // div
        if (res.dest == 0b00000) {
            res.op = DIV;
            res.ctrl = HILO;
        } else {
            fprintf(stderr, "DIV requires rd=0, but got rd=%d\n", res.dest);
            exit(EXIT_FAILURE);
        }
        break;
    case 0b001000: // jr
       if (res.right == 0b00000 && res.dest == 0b00000) {
            res.extra = res.left;
            res.op = OR; // will do left (rs) OR right, which will just give left which has the address
            res.ctrl = JUMP;
        } else {
            fprintf(stderr, "JR requires rt=0 and rd=0, but got rd=%d, rt=%d\n", res.dest, res.right);
            exit(EXIT_FAILURE);
        } 
        break;
    case 0b010000: // mfhi
        if (res.left == 0b00000 && res.right == 0b00000) {
            res.left = st->hi_reg; // get value from hi register to write back to dest 
            res.op = OR; // keep value the same bc right is 0
            res.ctrl = WB; 
        } else {
            fprintf(stderr, "MFHI requires rs=0 and rt=0, but got rs=%d, rt=%d\n", res.left, res.right);
            exit(EXIT_FAILURE);
        }  
        break;
    case 0b010010: // mflo
        if (res.left == 0b00000 && res.right == 0b00000) {
            res.left = st->lo_reg; // get value from lo register to write back to dest
            res.op = OR; // keep value the same bc right is 0
            res.ctrl = WB; 
        } else {
            fprintf(stderr, "MFLO requires rs=0 and rt=0, but got rs=%d, rt=%d\n", res.left, res.right);
            exit(EXIT_FAILURE);
        }  
        break;
    case 0b011000: // mult
        if (res.dest == 0b00000) {
            res.op = MUL;
            res.ctrl = HILO;
        } else {
            fprintf(stderr, "MULT requires rd=0, but got rd=%d\n", res.dest);
            exit(EXIT_FAILURE);
        }
        break;
    case 0b100101: // or
        res.op = OR;
        res.ctrl = WB;
        break;
    case 0b000000: // sll
        if (res.left == 0b00000) {
            res.op = SLL;
            res.ctrl = WB; 
        } else {
            fprintf(stderr, "SLL requires rs=0, but got rs=%d\n", res.left);
            exit(EXIT_FAILURE);
        }  
        break; 
    case 0b000100: // sllv
        res.op = SLL;
        res.ctrl = WB;
        break;
    case 0b101010: // slt
        res.op = LT;
        res.ctrl = WB;
        break;
    case 0b000011: // sra
        if (res.left == 0b00000) {
            res.op = SRA;
            res.ctrl = WB; 
        } else {
            fprintf(stderr, "SRA requires rs=0, but got rs=%d\n", res.left);
            exit(EXIT_FAILURE);
        }  
        break;  
    case 0b000111: // srav
        res.op = SRA;
        res.ctrl = WB;
        break;
    case 0b000010: // srl
        if (res.left == 0b00000) {
            res.op = SRL;
            res.ctrl = WB; 
        } else {
            fprintf(stderr, "SRL requires rs=0, but got rs=%d\n", res.left);
            exit(EXIT_FAILURE);
        }  
        break; 
    case 0b000110: // srlv
        res.extra = 
        res.op = SRL;
        res.ctrl = WB;
        break;
    case 0b100010: // sub
        res.op = SUB;
        res.ctrl = WB;
        break;
    case 0b100110: // xor
        res.op = XOR;
        res.ctrl = WB;
        break;
    default: 
        fprintf(stderr, "Invalid funct: %d\n", funct);
        exit(EXIT_FAILURE);
    }
    
    return res;
}

// helper to decode an i format instruction
instruction i_format(state* st, uint32_t instr, uint8_t op) {
    instruction res;
    // 5 bit mask
    uint32_t m = 0b00000011111;
    res.left = st->regs[(instr >> 21) & m]; // load information from register
    res.dest = (instr >> 16) & m;
    // 16 bit mask for immediate
    uint32_t im = 0b1111111111111111;
    res.right = instr & im; // keep immediate as direct encoding
    
    switch(op) {
        case 0b001000: // addi
            res.op = ADD;
            res.ctrl = WB;
            break;
        case 0b001100: // andi
            res.op = AND;
            res.ctrl = WB;
            break;
        case 0b000100: // beq
            res.op = EQ;
            res.ctrl = BRANCH;
            break;
        case 0b000101: // bne
            res.op = NEQ;
            res.ctrl = BRANCH;
            break;
        case 0b001111: // lui
           if (res.left == 0b00000) {
                res.op = SLL;
                res.ctrl = WB; 
            } else {
                fprintf(stderr, "LUI requires rs=0, but got rs=%d\n", res.left);
                exit(EXIT_FAILURE);
            }  
            break;  
        case 0b100011: // lw
            res.op = ADD;
            res.ctrl = LW;
            break;
        case 0b001101: // ori
            res.op = OR;
            res.ctrl = WB;
            break;
        case 0b001010: // slti
            res.op = LT;
            res.ctrl = WB;
            break;
        case 0b101011: // sw
            res.op = ADD;
            res.ctrl = SW;
            break;
        case 0b001110: // xori
            res.op = XOR;
            res.ctrl = WB;
            break;
        default: 
            fprintf(stderr, "Invalid opcode: %u\n", op);
            exit(EXIT_FAILURE); 
    }

    return res;
}

// helper to decode a j format instruction
instruction j_format(uint32_t instr, uint8_t op) {
    instruction res;
    // get least significant 26 bits (address)
    res.extra = instr & 0b11111111111111111111111111;
    res.right = 2; // hardcoded shift amount for computing address shift in alu

    switch (op)
    {
    case 0b00010:
        res.op = SLL;
        res.ctrl = JUMP;
        break;
    case 0b000011:
        res.op = SLL;
        res.dest = 31; // $31 is the reserved $ra register
        res.ctrl = JAL;
        break;
    }

    return res;
}

instruction decode(state* st, uint32_t instr) {
    // get op code
    uint32_t m = 0b111111;
    uint8_t op = (instr >> 26) & m;
    switch (op)
    {
    case 0b000000:
        return r_format(st, instr);
    case 0b000010: 
        return j_format(instr, op);
    case 0b000011:
        return j_format(instr, op);
    default: // checks for invalid op code in i_format
        return i_format(st, instr, op);
    }
}


// Execute the arithmetic and logical parts of the instruction
result execute(state* st, instruction instr) {
    result res;
    // just for ease
    int32_t left = instr.left;
    int32_t right = instr.right; 
    switch (instr.ctrl)
    {
    case BRANCH: 
        res.value = alu(left, right, instr.op, &res.hi_val); // compare values (beq or bne) with alu
        res.op = NONE;
        if (res.value){
            // assuming this is like using the xtnd hardware bc alu is doing the comparison
            // notably I think the + is wrong, but I couldn't figure out how to handle it with 
            // the fact that the other jumps just update the pc directly to the address in hi_val
            // in run_one_instruction()
            res.hi_val = st->pc + (right << 2); 
            res.jump = 1;
        } else {
            res.jump = 0;
        }
        break;
    case HILO: 
        res.value = alu(left, right, instr.op, &res.hi_val);
        res.dest = instr.dest;
        res.op = HILO_WB;
        res.jump = 0;
        break;
    case JUMP:
        res.hi_val = alu(instr.extra, right, instr.op, &res.hi_val); // shift address left by 2
        res.op = NONE;
        res.jump = 1;
        break;
    case JAL:
        res.value = st->pc + 4; // going to put pc+4 in 31, assuming we can use +4 hardware for this not alu
        res.hi_val = alu(instr.extra, right, instr.op, &res.hi_val); // shift address left by 2
        res.dest = instr.dest; 
        res.op = WRITEBACK;
        res.jump = 1;
        break;
    case LW: 
        res.value = alu(left, right, instr.op, &res.hi_val); // storing computed address in value
        res.dest = instr.dest;
        res.op = LOAD;
        res.jump = 0;
        break;
    case SW:
        res.value = alu(left, right, instr.op, &res.hi_val); // storing computed address in value
        res.dest = instr.dest; // dest is now the register that has the data to be stored
        res.op = STORE;
        res.jump = 0;
        break;
    case WB:
        res.value = alu(left, right, instr.op, &res.hi_val);
        res.dest = instr.dest;
        res.op = WRITEBACK;
        res.jump = 0;
        break;
    }
    return res;
}

// Perform any required memory updates.
result memory(state* st, result ex_res) {
    result res = ex_res;
    switch (ex_res.op)
    {
    case LOAD:
        res.value = get_memory(st, ex_res.value);
        res.op = WRITEBACK; 
        break;
    case STORE:
        set_memory(st, ex_res.value, st->regs[ex_res.dest]); // dest is rt, value is the memory address
        res.op = NONE;
        break;
    default: 
        return res;
    }
    return res;
}

// Update any register values that need to be changed.
void writeback(state* st, result res) {
    if (res.op == WRITEBACK) {
        st->regs[res.dest] = res.value;
        printf("WRITEBACK: reg %d = %d\n", res.dest, res.value);
    } else if (res.op == HILO_WB) {
        st->hi_reg = res.hi_val;
        printf("WRITEBACK: hi_reg = %d\n", res.hi_val);
        st->lo_reg = res.value;
        printf("WRITEBACK: lo_reg = %d\n", res.value);
    }
}

void run_one_instruction(state* st) {
    uint32_t next = fetch(st);
    printf("FETCH: PC = %zu, instruction = 0x%08X\n", st->pc, next);

    instruction instr = decode(st, next);
    printf("DECODE: op=%s, ctrl=%s, left=%d, right=%d, dest=%u, extra=%d\n",
           alu_op_str(instr.op), ctrl_op_str(instr.ctrl), instr.left, instr.right, instr.dest, instr.extra);

    result ex_res = execute(st, instr);
    printf("EXECUTE: value=%d, hi_val=%d, dest=%d, op=%s, jump=%u\n",
           ex_res.value, ex_res.hi_val, ex_res.dest, res_op_str(ex_res.op), ex_res.jump);

    result mem_res = memory(st, ex_res);
    printf("MEMORY: value=%d, dest=%d, op=%s\n",
           mem_res.value, mem_res.dest, res_op_str(mem_res.op));

    writeback(st, mem_res);

    if (ex_res.jump == 1){ 
        st->pc = ex_res.hi_val;
        printf("PC incremented to %zu\n", st->pc);
    } else {
        st->pc += 4;
        printf("PC incremented to %zu\n", st->pc);
    }
}

void run_program(state* st, size_t prog_len) {
  while (st->pc < 4 * prog_len) {
    run_one_instruction(st);
  }
}
