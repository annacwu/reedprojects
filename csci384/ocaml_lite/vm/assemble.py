import sys


fn_map = {}
closure_labels = {}
current_fn = None

instr_codes = {
    'CONST'   : 0x07,
    'SCONST'  : 0x0F,
    'ADD'     : 0x20,
    'SUB'     : 0x21,
    'MUL'     : 0x22,
    'DIV'     : 0x23,
    'NEG'     : 0x24,
    'EQ'      : 0x25,
    'NEQ'     : 0x26,
    'LEQ'     : 0x27,
    'LT'      : 0x28,
    'AND'     : 0x29,
    'OR'      : 0x2A,
    'NOT'     : 0x2B,
    'MOD'     : 0x2C,
    'CONCAT'  : 0x2D,
    'CHARAT'  : 0x2E,
    'BR'      : 0x30,
    'JUMP'    : 0x31,
    'CLOSE'   : 0x40,
    'CALL'    : 0x41,
    'RET'     : 0x42,
    'CLOSEREC': 0x43,
    'CALLPOP' : 0x44,
    'CALLN'   : 0x45,
    'CALLNP'  : 0x46,
    'OVER'    : 0x50,
    'POP1'    : 0x51,
    'ENV'     : 0x52,
    'CONS'    : 0x60,
    'GETTAG'  : 0x61,
    'DECONS'  : 0x62,
    'PRINT'   : 0xF0,
    'STOI'    : 0xF1,
    'ITOS'    : 0xF2,
    'ERROR'   : 0xF3,
}

with open(sys.argv[1], 'r', encoding='utf-8') as infile:
    for line in infile:
        if line[0] == '#':
            continue
        if not line[0].isspace():
            name = line[:line.index(':')]
            current_fn = name
            current_size = 0
            fn_map[name] = bytearray()
            closure_labels[name] = []
            continue
        if '"' in line:
            line = line.strip()
            ind = line.index(' ')
            words = [line[:ind]]
            s = line[ind:].strip()
            words.append(s[1:-1])
        else:
            words = list(line.strip().split())
        if len(words) == 0 or words[0][0] == '#':
            continue
        words[0] = words[0].upper()
        fn_map[current_fn].append(instr_codes[words[0]])
        if words[0] == 'CONST':
            fn_map[current_fn].extend(int(words[1]).to_bytes(8, signed=True))
        elif words[0] == 'SCONST':
            fn_map[current_fn].extend(words[1].encode('utf-8'))
            fn_map[current_fn].append(0x00)
        elif words[0] == 'CLOSE' or words[0] == 'CLOSEREC':
            fn_map[current_fn].append(int(words[1]))
            fn_map[current_fn].append(int(words[2]))
            closure_labels[current_fn].append((len(fn_map[current_fn]), words[3]))
            fn_map[current_fn].append(0xFF)
        elif words[0] == 'OVER' or words[0] == 'ENV' or words[0] == 'DECONS' \
                or words[0] == 'CALLPOP' or words[0] == 'CALLN':
            fn_map[current_fn].append(int(words[1]))
        elif words[0] == 'CONS' or words[0] == 'CALLNP':
            fn_map[current_fn].append(int(words[1]))
            fn_map[current_fn].append(int(words[2]))
    infile.seek(0)

fn_positions = {}
total_size = 16 + 8 * len(fn_map)

fn_names = ['main'] + list(filter(lambda x: x != 'main', fn_map.keys()))

for name in fn_names:
    if name == 'main':
        continue
    fn_positions[name] = total_size
    total_size += len(fn_map[name])
fn_positions['main'] = total_size

for name in fn_names:
    for (pos, label) in closure_labels[name]:
        fn_map[name][pos] = fn_names.index(label)

with open(sys.argv[2], 'wb') as outfile:
    outfile.write("ReedPLDI".encode('utf-8'))
    outfile.write(len(fn_map).to_bytes(8))
    for name in fn_names:
        outfile.write(fn_positions[name].to_bytes(8))
    for name in fn_names:
        if name != 'main':
            outfile.write(fn_map[name])
    outfile.write(fn_map['main'])
