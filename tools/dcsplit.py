#!/usr/bin/env python3

# the disassembly part of this was made from the doldecomp disassembly script as a base

from sh4dis import sh4
import struct
import sys
import os
from pathlib import Path
import yaml

if len(sys.argv) < 2:
    print("USAGE: dcdis config.yaml")
    sys.exit(1)

with open(sys.argv[1], 'r') as file:
    dcsplit_config = yaml.safe_load(file)

with open(dcsplit_config['target_path'], 'rb') as file:
    filecontent = bytearray(file.read())

labelled_addresses = set()
label_names = {}

def add_label(addr, name):
    labelled_addresses.add(addr)
    if name != None and not addr in label_names:
        label_names[addr] = name

# checks if the specified address could be a pointer
def label_candidate(address):
    if address in blacklistLabels:
        return False
    
    if address >= key and address < key + len(filecontent):
        return True
    
    return False

# this handles the sections between functions that hold the data that the functions were using
def add_label_funcData(addr, short):
    labelled_addresses.add(addr)

    # if a short, add to list of the 2 byte labels (data.w)
    if short == True:
        data_short_addresses.add(addr)
        return
    
    # if not a short, it has to be 4 bytes (data.l)
    data_long_addresses.add(addr)

    # check if it happens to be a pointer to something valid in the binary
    integer = struct.unpack("I", filecontent[addr - key : addr - key + 4])[0]
    if label_candidate(integer) == True:
        labelled_addresses.add(integer)

data_short_addresses = set()
data_long_addresses = set() 
labelled_bss_addresses = set()

if 'blacklist_code' in dcsplit_config:
    for elem in dcsplit_config['blacklist_code']:
        add_label_funcData(elem, True)

key = dcsplit_config['key'] 

if 'symbol_addrs_path' in dcsplit_config:
    with open(dcsplit_config['symbol_addrs_path'], 'r') as symbols_file:
        symbols_lines = symbols_file.readlines()
        for line in symbols_lines:
            spl = line.split("=")
            symbol_name = spl[0].strip()
            symbol_addr = spl[1].strip()[:-1]
            add_label(int(symbol_addr, 16), "_" + symbol_name)

writeBss = 0

# since the disassembler always jumps 2 bytes (length of an SH4 instruction)
# do not attempt to disassemble any addresses that are the "middle" of a 4 byte data
def check_invalid_data_address(address):
    return (address - 2) in labelled_addresses and (address - 2) in data_long_addresses

add_label(key, dcsplit_config['entry'])

def addr_to_label(addr):
    return "#" + addr_to_label_noh(addr)

def addr_to_label_noh(addr):
    if addr not in labelled_addresses:
        return "h'%08X" % addr

    if addr in label_names:
        return label_names[addr]
    
    return "_lbl_%08X" % addr    

def get_short_at(offset):
    return struct.unpack("H", filecontent[offset : (offset + 2)])

def get_instruction_at(address, offset):
    code = get_short_at(offset)
    return sh4.decode(code[0], address)

# Calls callback for every instruction in the specified code section
def disasm_iter(offset, address, size, callback):
    if size == 0:
        return
    start = address
    end = address + size
    while address < end:
        
        code = struct.unpack("H", filecontent[(offset + (address-start)) : (offset + (address-start)) + 2])
        insn = sh4.decode(code[0], address)
        #print(sh4.disasm(code[0], address))
        if insn is not None:
            callback(start, end, address, offset + address - start, insn, code[0])
        address += 2
        #if address < end:
        #    o = offset + address - start
        #    callback(address, offset + address - start, None, filecontent[o : o + 4])
        #    address += 4

# helps converting the operands of an instruction to valid asm
def oper2str(insn, oper):
    if oper[0] == sh4.OPER_TYPE.ADDRESS:
        if insn.op == sh4.OP.MOV or insn.op == sh4.OP.MOVA or insn.op == sh4.OP.BT or insn.op == sh4.OP.BRA or insn.op == sh4.OP.BF or insn.op == sh4.OP.BSR:
            return addr_to_label_noh(oper[1])
        else:
            return addr_to_label(oper[1])
    if oper[0] in [sh4.OPER_TYPE.GPREG, sh4.OPER_TYPE.FPUREG, sh4.OPER_TYPE.CTRLREG, sh4.OPER_TYPE.SYSREG]:
        return oper[1].name.upper()
    if oper[0] == sh4.OPER_TYPE.BANKREG:
        return oper[1].name[:-1].upper()
    if oper[0] == sh4.OPER_TYPE.DEREF_REG:
        return '@%s' % oper[1].name.upper()
    if oper[0] == sh4.OPER_TYPE.DEREF_REG_POST_INCR:
        return '@%s+' % oper[1].name.upper()
    if oper[0] == sh4.OPER_TYPE.DEREF_REG_PRE_DECR:
        return '@-%s' % oper[1].name.upper()
    if oper[0] == sh4.OPER_TYPE.DEREF_REG_IMM:
        return '@(%d,%s)' % (oper[2], oper[1].name.upper())
    if oper[0] == sh4.OPER_TYPE.DEREF_REG_REG:
        return '@(%s,%s)' % (oper[1].name.upper(), oper[2].name.upper())
    if oper[0] == sh4.OPER_TYPE.IMMEDIATE:
        if insn.op == sh4.OP.MOV or insn.op == sh4.OP.ADD or insn.op == sh4.OP.CMP:
            return '#%d' % (oper[1])
        else:
            return '#%d' % (oper[1] & 0xFF)

# converts an instruction to text
def decoded2str(decoded):
    result = decoded.op.name.lower()
    if decoded.length_suffix == sh4.SUFFIX.L:
        result += '.l'
    if decoded.length_suffix == sh4.SUFFIX.B:
        result += '.b'
    if decoded.length_suffix == sh4.SUFFIX.W:
        result += '.w'
    if decoded.delay_slot:
        if result == "bt" or result == "bf":
            result += '/s'
        else:
            result += '.s'
    if decoded.cond != sh4.COND.NONE:
        result += ['', '/eq', '/ge', '/gt', '/hi', '/hs', '/pl', '/pz', '/str'][decoded.cond.value]
    if decoded.operands:
        result += ' '
        result += oper2str(decoded,decoded.operands[0])
        if len(decoded.operands) > 1:
            result += ','
            result += oper2str(decoded,decoded.operands[1])
            if len(decoded.operands) > 2: # eg: fmac fr0,fr0,fr0
                result += ','
                result += oper2str(decoded,decoded.operands[2])
    return result

def disasm(arg, pc = 0):
    if type(arg) == int:
        decoded = decode(arg, pc)
    else:
        decoded = arg

    #print(decoded)

    return decoded2str(decoded)

def insn_to_text(insn, raw, pc):
    # in regular DC code, these instructions never really occur
    # and usually only pop up when data is misinterpreted as instructions
    if insn.op in [sh4.OP.TRAPA, sh4.OP.FSRRA, sh4.OP.FSCA]:
        return "error"
    
    operand_count = len(insn.operands)
    if operand_count == 0:
        return disasm(insn, pc)

    # if pointing to a "blacklisted" address it definitely cannot be a real instruction
    if insn.operands[0][0] == sh4.OPER_TYPE.ADDRESS and insn.operands[0][1] in blacklistLabels:
        return "error"
    
    # i guess these are also usually invalid instructions 
    # i honestly don't really remember why, been a while since i wrote this lol
    if insn.operands[0][0] == sh4.OPER_TYPE.BANKREG:
        return "error"
    if operand_count > 1 and insn.operands[1][0] == sh4.OPER_TYPE.BANKREG:
        return "error"
    
    return disasm(insn, pc)            

asm_file = None
def open_asm_file_output(filename):
    global asm_file
    asm_file = open(filename, "w")

def write_asm_file_output(str):
    asm_file.write(str + os.linesep)

def close_asm_file_output():
    asm_file.close()

def bytes_iter(offset, address, size):
    if size == 0:
        return
    start = address
    end = address + size
    while address < end:
        if address in labelled_addresses:
            if address in label_names:
                write_asm_file_output("\n\t.EXPORT %s" % addr_to_label_noh(address))
                write_asm_file_output("%s:" % addr_to_label_noh(address))
            else:
                write_asm_file_output("\n\t.EXPORT %s" % ("_lbl_%08X" % address))
                write_asm_file_output("_lbl_%08X:" % address)
        #code = struct.unpack("B", filecontent[(offset + (address-start)) : (offset + (address-start)) + 1])[0]
        code = filecontent[offset + address - start]
        write_asm_file_output("\t.data.b h'%04x" % int(code))
        address += 1

# todo: make it merge the bytes between labels
def bss_iter(offset, address, size):
    if size == 0:
        return

    #sorted_bss_labels = filter(lambda x: x - key > len(filecontent), list(sorted(labelled_addresses)))
    #print(sorted_bss_labels)
    start = address
    end = address + size
    while address < end:
        if address in labelled_addresses:
            if address in label_names:
                write_asm_file_output("\n\t.EXPORT %s" % addr_to_label_noh(address))
                write_asm_file_output("%s:" % addr_to_label_noh(address))
            else:
                write_asm_file_output("\n\t.EXPORT %s" % ("_lbl_%08X" % address))
                write_asm_file_output("_lbl_%08X:" % address)
        write_asm_file_output("\t.res.b 1")
        address += 1

def disassemble_callback(seg_start, seg_end, address, offset, insn, bytes):
    raw = struct.unpack("H", filecontent[offset : offset + 2])[0]
    integer = struct.unpack("I", filecontent[offset : offset + 4])[0]
    
    if check_invalid_data_address(address):
        return
    
    # todo: figure out how this case occurs, i probably put it here for a good reason
    if address in data_short_addresses and address in data_long_addresses:
        data_short_addresses.remove(address)

    # Output label (if any)
    if address in labelled_addresses:
        if address in label_names:
            write_asm_file_output("\n\t.EXPORT %s" % label_names[address])
            write_asm_file_output("%s:" % label_names[address])
        else:
            write_asm_file_output("\n\t.EXPORT %s" % ("_lbl_%08X" % address))
            write_asm_file_output("_lbl_%08X:" % address)
        if address in data_short_addresses:
            write_asm_file_output("\t.data.w h'%04x" % (raw))
            return
        elif address in data_long_addresses:
            if ((integer >= key and integer < key + len(filecontent)) or integer in label_names) and (integer < seg_start or integer >= seg_end):
                write_asm_file_output("\t.IMPORT %s" % (addr_to_label_noh(integer)))    

            write_asm_file_output("\t.data.l %s" % (addr_to_label_noh(integer)))
            return

    asm = None

    # if we had an instruction try and turn it into string
    if insn != None:
        asm = insn_to_text(insn, raw, address)

    # todo: make this use some Optional shit or something
    # if we failed to parse the instruction, turn it into data (short)
    if asm == "error":
        asm = ".data.w h'%04X" % raw

    write_asm_file_output('\t%s ;%x' % (asm,address))

blacklistLabels = set()
usedLabels = set()

# todo: clean this up to... not be needed at all lol
# invalidate any invalid labels that cropped up
def blacklist_label_callback(seg_start, seg_end, address, offset, insn, bytes):
    if check_invalid_data_address(address):
        return
    
    # Output label (if any)
    if address in labelled_addresses:
        usedLabels.add(address)

max_end = 0
max_data_end = 0
is_short_data = False
is_data = set()
import_data = set()
visited = set()
functions_done = set()

def find_func_end(address, seg_end):
#    print("%x" % (address), visited)
   # print([("%x") % (x) for x in sorted(functions_done)])
    if address > seg_end:
        return
    if address in visited:
        return
    visited.add(address)
    while True:
        
        insn = get_instruction_at(address + key, address)
        
        global max_end
        global max_data_end

        result = insn.op.name.lower()
        if result in {"rts", "jmp"}:
            if address + 2 > max_end:
                max_end = address+2  # delayed, so put the end on the instruction after it
            return

        if insn.op == sh4.OP.MOV or insn.op == sh4.OP.MOVA:
            if insn.operands[0][0] == sh4.OPER_TYPE.ADDRESS:
                is_data.add(insn.operands[0][1] - key)
                import_data.add(insn.operands[0][1] - key)
                if insn.operands[0][1] - key > max_data_end:
                    max_data_end = insn.operands[0][1] - key
                    is_short_data = insn.length_suffix != sh4.SUFFIX.L and insn.op != sh4.OP.MOVA

        if result in {"bf", "bra", "bsr", "bsrf", "bt"}:
            if insn.operands[0][0] == sh4.OPER_TYPE.ADDRESS:
                import_data.add(insn.operands[0][1] - key)
                for op in insn.operands:
                    if op[0] == sh4.OPER_TYPE.ADDRESS:
                        if (op[1] - key) not in functions_done:
                            find_func_end(op[1] - key, seg_end)
                if result == "bra":
                    if address + 2 > max_end:
                        max_end = address+2  # delayed, so put the end on the instruction after it
                    return

        address += 2
    
def func_search(addr, seg_end):
    # i'm guessing this global thing is a terrible idea, but i don't know python for shit
    global max_end
    global max_data_end
    global visited
    global is_data

    # init
    max_end = 0
    max_data_end = 0
    visited = set()
    is_data = set()
    import_data = set()

    functions_done.add(addr)

    # basically, this finds the last/"farthest" rts or jmp in the function
    find_func_end(addr, seg_end)

    # now there are a lot of weird cases that are possible with the data sections
    # so what im doing is, first we note down if and where the function uses data
    # and after the function end (max_end), if there is nop padding
    # then the data section is definitely there at the end of our function

    # if it is, find the end of that
    # if there isnt nop but we detected data used by the code, then the data section is in another function later on
    # and after our function code, most likely there directly is another function

    # usual cases:
    # 1. no data at all
    # 2. data, but its inbetween the function and not after the function
    #    this is caught by keeping track of the furthest data address
    # 3. data in a later function, this can be caught by checking if the address
    #    right after the end of the function is a data label or not
    
    # if there's no data used by the function or there was only data inbetween the function, but not after it
    if max_data_end == 0 or max_data_end < max_end:
        return max_end + 2 # then we just return the end of the code

    # if its not a nop, neither a data from our func 
    # (it can be directly data without nops before it, if the alignment lines up perfectly)
    maybe_nop = get_instruction_at(max_end + 2 + key, max_end + 2)
    if maybe_nop.op != sh4.OP.NOP and max_end + 2 not in is_data:
        return max_end + 2

    # in this case, after our function's code our data is there
    # we jump over the last data we found
    # and then skip past the nops
    data_section = max_data_end + (2 if is_short_data else 4)
    maybe_nop = get_instruction_at(data_section + key, data_section)
    while(maybe_nop.op == sh4.OP.NOP):
        data_section = data_section + 2
        maybe_nop = get_instruction_at(data_section + key, data_section)

    if get_short_at(data_section) == 0 and (seg_end - data_section) < 32:
        while get_short_at(data_section) == 0:
            data_section += 2

    return data_section

# Get labels
def get_label_callback(seg_start, seg_end, address, offset, insn, bytes):
    if insn == None:
        return
    
    if address in data_long_addresses or address in data_short_addresses:
        return

    # if branch instruction
    if insn.op in {sh4.OP.BF, sh4.OP.BRA, sh4.OP.BRAF, sh4.OP.BSR, sh4.OP.BSRF, sh4.OP.BT}:
        for op in insn.operands:
            if op[0] == sh4.OPER_TYPE.ADDRESS:                
                add_label(op[1], None)
    if insn.op == sh4.OP.MOV or insn.op == sh4.OP.MOVA:
        if insn.operands[0][0] == sh4.OPER_TYPE.ADDRESS:
            if insn.operands[0][1] == 0x0C51DEA4:
                print("!!!! %X" % address)
            if insn.length_suffix == sh4.SUFFIX.L or insn.op == sh4.OP.MOVA:
                add_label_funcData(insn.operands[0][1], False)
            else:
                add_label_funcData(insn.operands[0][1], True)

def get_subseg_name(subseg):
    return ("code_%x" % subseg[0]) if len(subseg) < 3 else subseg[2]

# collect locations to disassemble that could contain labels

# contains start and end of segments that contain code
code_intervals = []

segs = dcsplit_config['segments']
for i in range(len(segs)):
    seg = segs[i]

    if seg['type'] != 'asm' and seg['type'] != 'code':
        continue

    start = seg['start']

    # get the start of the next "area"
    if (i == len(segs) - 1):
        # if it's the last defined area then we have to go until the end of the file
        end = len(filecontent)
    else:
        # if not then go until the next defined segment
        end = segs[i+1]['start']

    if seg['type'] == 'code':
        subsegs = seg['subsegments']
        for j in range(len(subsegs)):
            subseg = subsegs[j]
            if subseg[1] != 'asm' and subseg[1] != 'libasm': 
                continue

            subseg_start = subseg[0]

            if j == len(subsegs) - 1:
                subseg_end = end
            else:
                subseg_end = subsegs[j+1][0]

            code_intervals.append([subseg_start, subseg_end, get_subseg_name(subseg)])
            
        continue

    code_intervals.append([start, end, seg['name']])

def code_interval_step(callback):
    for interval in code_intervals:
        start = interval[0]
        end = interval[1]
        disasm_iter(start, start + key, end - start, callback)

# step 0: make output "asm" directory if it doesn't exist yet
if not os.path.exists("asm"):
    os.mkdir("asm")

# step 1: collect labels
code_interval_step(get_label_callback)

# step 2: remove potentially invalid labels (todo: this might be doable way simpler)
code_interval_step(blacklist_label_callback)

blacklistLabels = { x for x in labelled_addresses if x not in usedLabels }
labelled_addresses = { x for x in labelled_addresses if (x - 2) not in data_long_addresses }

# step 3: generate asm (we need to get the proper labels in step 1 and 2 so that we can output those into the asm code)

def gen_asm(object_name, seg_type, start, end):
    # the section types that the segment types correspond to
    section_map = {
        "asm": "P",
        "data": "D,DATA,ALIGN=4",
        "rodata": "C,DATA,ALIGN=4",
        "bss" : "B",
        "libasm": "PSG",
        "libdata": "DSG",
        "librodata": "CSG"
    }

    # open file
    open_asm_file_output(object_name + ".src")

    write_asm_file_output("\t.SECTION %s" % section_map[seg_type])
    if(seg_type == 'asm' or seg_type == 'libasm'):
        disasm_iter(start, start + key, end - start, disassemble_callback)
    elif seg_type == 'bss':
        bss_iter(start, start + key, end - start)
    else:
        bytes_iter(start, start + key, end - start)

    close_asm_file_output()

# contains start and end of all segments, for linker script creation
all_intervals = []
# tells the linker script generator where to use regular asm, and where to use inline asm c files
use_c_files = set()
c_file_already_exists = set()
has_rodata = set()
has_data = set()

def add_to_c_file(path, func_name, asm):
    if path in c_file_already_exists:
        return
    
    c_file = open(path, "a")

    c_file.write("INLINE_ASM(%s, \"%s\");" % (func_name, asm) + os.linesep * 2)

    c_file.close()

segs = dcsplit_config['segments']
for i in range(len(segs)):
    seg = segs[i]

    start = seg['start']
    dir = (seg['dir'] + "/") if 'dir' in seg else ''
    
    if dir != '':
        os.makedirs(dcsplit_config['asm_path'] + "/" + dir, exist_ok=True) 

    # get the start of the next "area"
    if (i == len(segs) - 1):
        # if it's the last defined area then we have to go until the end of the file
        end = len(filecontent)
    else:
        # if not then go until the next defined segment
        end = segs[i+1]['start']

    if seg['type'] == 'code':
        # if a rodata or data section for a file 
        # is split into multiple segments, this is how we keep track of how many there are
        rodata_data_index = 0

        subsegs = seg['subsegments']
        for j in range(len(subsegs)):
            subseg = subsegs[j]
            subseg_start = subseg[0]
            subseg_name = get_subseg_name(subseg)

            if len(subseg) > 3:
                subseg_end = subseg[3]
            elif j == len(subsegs) - 1:
                subseg_end = end
            else:
                subseg_end = subsegs[j+1][0]

            os.makedirs(dcsplit_config['src_path'] + "/" + dir, exist_ok=True) 
            os.makedirs(dcsplit_config['nonmatchings_path'] + "/" + dir + subseg_name + "/", exist_ok=True) 

            c_file_path = dcsplit_config['src_path'] + "/" + dir + subseg_name + ".c"

            if subseg[1] != 'asm' and subseg[1] != 'libasm':
                name = dir + subseg_name

                suffix = ''
                if rodata_data_index > 0:
                    last_seg = subsegs[j-1]
                    if last_seg[1] == subseg[1] and last_seg[2] == subseg[2]:
                        suffix = f"_{rodata_data_index}"
                    else:
                        rodata_data_index = 0

                nonmatch = dcsplit_config['nonmatchings_path'] + "/" + dir + subseg_name + "/" + subseg[1] + suffix 
                
                has_code_equivalent = any((dir + subseg_name) == e[2] for e in all_intervals)
                if has_code_equivalent:
                    if 'rodata' in subseg[1]:
                        add_to_c_file(c_file_path, "_rodata" + suffix, nonmatch + ".src")
                    elif 'data' in subseg[1]:
                        add_to_c_file(c_file_path, "_data" + suffix, nonmatch + ".src")

                    rodata_data_index += 1
                else:
                    all_intervals.append([start, end, name])
                    nonmatch = dcsplit_config['asm_path'] + "/" + name

                gen_asm (
                    nonmatch,
                    subseg[1], 
                    subseg_start, 
                    subseg_end
                ) 
                continue

            # build c file
            use_c_files.add(dir + subseg_name)

            # don't create the c files if they already exist, the repo should have them one and done under usual circumstances
            
            if not os.path.exists(c_file_path) or 'overwrite_c_always' in dcsplit_config:
                os.makedirs(os.path.dirname(c_file_path), exist_ok=True) 
                c_file = open(c_file_path, "w")
            else:
                c_file_already_exists.add(c_file_path)
                c_file = None

            def write_c_file(str):
                if c_file == None:
                    return
                c_file.write(str + os.linesep)

            if 'generated_c_preamble' in dcsplit_config:
                write_c_file(dcsplit_config['generated_c_preamble'] + os.linesep)

            all_intervals.append([start, end, dir + subseg_name])    

            func = subseg_start
            while func < subseg_end:    
                end_func = func_search(func, subseg_end)
                #print("%x %x %x" % (func, end_func, subseg_end))
                func_va = (func + key)
                func_name = ("_func_%08X" % func_va) if func_va not in label_names else label_names[func_va]

                write_c_file("INLINE_ASM(%s, \"%s\");" % (func_name, dcsplit_config['nonmatchings_path'] + "/" + dir + subseg_name + "/" + func_name + ".src") + os.linesep)

                if end_func > subseg_end:
                    end_func = subseg_end

                gen_asm(
                    dcsplit_config['nonmatchings_path'] + "/" + dir + subseg_name + "/" + func_name, 
                    subseg[1], 
                    func, 
                    end_func
                )       
                func = end_func
            if c_file != None:
                c_file.close() 
        continue

    all_intervals.append([start, end, dir + seg['name']])    
    gen_asm(dcsplit_config['asm_path'] + "/" + dir + seg['name'], seg['type'], start, end)

def create_lnk_sub():
    lnk = open(dcsplit_config['linker_path'], "w")
    lnk.write("align_section" + os.linesep)
    lnk.write("elf" + os.linesep)
    lnk.write(("output %s" % ".\\" + dcsplit_config['elf_path'].replace("/", "\\")) + os.linesep)
    lnk.write("nolibrary" + os.linesep)
    
    if 'additional_linker' in dcsplit_config:
        for file in dcsplit_config['additional_linker']:
            lnk.write(file + os.linesep)

    sorted_files = sorted(all_intervals, key=lambda x: x[0])
    
    for file in sorted_files:
        build_path = ".\\%s\\%s\\" % (dcsplit_config['build_path'], dcsplit_config['asm_path'])
        if file[2] in use_c_files:
            build_path = ".\\%s\\%s\\" % (dcsplit_config['build_path'], dcsplit_config['src_path'])
        if file[2] in has_rodata:
            lnk.write("input " + build_path + file[2].replace("/", "\\") + "_rodata.obj" + os.linesep)    
        if file[2] in has_data:
            lnk.write("input " + build_path + file[2].replace("/", "\\") + "_data.obj" + os.linesep)    
        lnk.write("input " + build_path + file[2].replace("/", "\\") + ".obj" + os.linesep)

    lnk.write(("print %s" % ".\\" + dcsplit_config['map_path'].replace("/", "\\")) + os.linesep)
    lnk.write("entry " + dcsplit_config['entry'] + os.linesep)
    lnk.write("form a" + os.linesep)
    lnk.write("debug" + os.linesep)
    lnk.write(("start %s(%08X)" % (dcsplit_config['section_order'], dcsplit_config['key'])) + os.linesep)
    lnk.write("exit" + os.linesep)
    lnk.close()

create_lnk_sub()

if writeBss == 1:
    sectionLabels = sorted(labelled_bss_addresses)

    bssAddress = sectionLabels[0]
    bssEnd = sectionLabels[len(sectionLabels)-1]
    j = 0
    print("%x %x" % (bssAddress, bssEnd))
    print("\t.SECTION B,DATA")
    print("\t.ORG h'%08x" % bssAddress)
    while bssAddress < bssEnd:
        if j < len(sectionLabels):
            gapSize = sectionLabels[j] - bssAddress
            if gapSize != 0:
                print("\t.res.b h'%X" % gapSize)
            
            print("\t.GLOBAL lbl_%08X" % sectionLabels[j])
            print("lbl_%08X:" % sectionLabels[j])
            j += 1
        else:
            gapSize = bssEnd - bssAddress
            if gapSize != 0:
                print("\t.res.b h'%X" % gapSize)
        bssAddress += gapSize
    print("\t.res.b 1")
