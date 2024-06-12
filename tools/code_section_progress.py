#!/usr/bin/env python3
# gets the progress of the code section based on hitachi map file
# it only recognizes completely decompiled files atm (so files without inline_asm)
# (since per-function decompiling is not completely possible atm anyways)

import sys

source_file = {}

in_program_section = False
line_start = 0

total_size = 0
decompiled_size = 0

curr_size = 0

objects = {}

with open(sys.argv[1], "r") as map_file:
    lines = map_file.readlines()
    for i in range(len(lines)):
        line = lines[i]
        if "input .\\build\\src" in line:
            spl = line.split("build\\")[1].replace(".obj", ".c").strip()
            source_file[spl.split("\\")[-1].split(".")[0]] = spl.replace("\\", "/")

        if "P                                 H'" in line:
            line_start = i
            in_program_section = True

        if "* TOTAL ADDRESS *" in line:
            break

        if "H SERIES LINKAGE EDITOR" in line:
            in_program_section = False

        if in_program_section:
            if line.strip() == "": continue
            if i % 2 == line_start % 2:
                size_of_obj = int(line.split(" ")[-1].strip().split("H'")[1], 16)
                if size_of_obj == 0: continue
                name_of_obj = lines[i+1].strip().split(" ")[0]
                if name_of_obj not in source_file:
                    continue
                total_size += size_of_obj

                fully_matched = True
                with open(source_file[name_of_obj], "r") as c_file:
                    for line in c_file:
                        if "ASM" in line:
                            fully_matched = False
                            continue

                if fully_matched:
                    decompiled_size += size_of_obj
                objects[size_of_obj] = name_of_obj
                print("%s %x %s" % (name_of_obj, size_of_obj, fully_matched))
            
for k in sorted(objects):
    print(objects[k] + " %x" % k)
print(str((decompiled_size / total_size) * 100) + "%")