import sys
import os


instr_codes = {
    0x07: 'CONST'   ,
    0x0F: 'SCONST'  ,
    0x20: 'ADD'     ,
    0x21: 'SUB'     ,
    0x22: 'MUL'     ,
    0x23: 'DIV'     ,
    0x24: 'NEG'     ,
    0x25: 'EQ'      ,
    0x26: 'NEQ'     ,
    0x27: 'LEQ'     ,
    0x28: 'LT'      ,
    0x29: 'AND'     ,
    0x2A: 'OR'      ,
    0x2B: 'NOT'     ,
    0x2C: 'MOD'     ,
    0x2D: 'CONCAT'  ,
    0x2E: 'CHARAT'  ,
    0x30: 'BR'      ,
    0x31: 'JUMP'    ,
    0x40: 'CLOSE'   ,
    0x41: 'CALL'    ,
    0x42: 'RET'     ,
    0x43: 'CLOSEREC',
    0x44: 'CALLPOP' ,
    0x45: 'CALLN'   ,
    0x46: 'CALLNP'  ,
    0x50: 'OVER'    ,
    0x51: 'POP1'    ,
    0x52: 'ENV'     ,
    0x60: 'CONS'    ,
    0x61: 'GETTAG'  ,
    0x62: 'DECONS'  ,
    0xF0: 'PRINT'   ,
    0xF1: 'STOI'    ,
    0xF2: 'ITOS'    ,
    0xF3: 'ERROR'   ,
}

file_size = os.path.getsize(sys.argv[1])
with open(sys.argv[1], 'rb') as infile:
    magic_no = infile.read(8)
    if magic_no != "ReedPLDI".encode('utf-8'):
        print("Bad magic number")
        sys.exit()
    num_fns = int.from_bytes(infile.read(8))
    fn_ptrs = []
    for i in range(num_fns):
        fn_ptrs.append(int.from_bytes(infile.read(8)))
    for fn_index, ptr in enumerate(fn_ptrs):
        if fn_index == 0:
            print("main:")
        else:
            print(f"{fn_index}:")
        infile.seek(ptr)
        byte_offset = ptr
        while byte := infile.read(1):
            if fn_index < num_fns - 1:
                next_break = fn_ptrs[fn_index + 1]
            elif fn_index == num_fns - 1:
                next_break = fn_ptrs[0]
            else:
                next_break = int(1e8)
            code = int.from_bytes(byte)
            print(f"\t{instr_codes[code]}", end='')
            if code == 0x07:
                val = infile.read(8)
                byte_offset += 8
                print(f" {int.from_bytes(val)}")
            elif code == 0x0F:
                b = infile.read(1)
                byte_offset += 1
                s = ""
                while b != b"\x00":
                    s += b.decode('utf-8')
                    b = infile.read(1)
                    byte_offset += 1
                print(f' "{s}"')
            elif code == 0x40 or code == 0x43:
                s = int.from_bytes(infile.read(1))
                n = int.from_bytes(infile.read(1))
                f = int.from_bytes(infile.read(1))
                byte_offset += 3
                print(f" {s} {n} {f}")
            elif code == 0x50 or code == 0x52 or code == 0x62 or \
                    code == 0x44 or code == 0x45:
                n = int.from_bytes(infile.read(1))
                byte_offset += 1
                print(f" {n}")
            elif code == 0x60 or code == 0x46:
                t = int.from_bytes(infile.read(1))
                n = int.from_bytes(infile.read(1))
                byte_offset += 2
                print(f" {t} {n}")
            else:
                print()
            byte_offset += 1
            if byte_offset in fn_ptrs or byte_offset >= file_size:
                break
