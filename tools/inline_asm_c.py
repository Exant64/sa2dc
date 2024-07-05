#!/usr/bin/env python3

import sys
import os

lines = []
with open(sys.argv[1], "r") as file:
    lines = file.readlines()

with open(sys.argv[2], "w") as file:
    for i in range(len(lines)):
        line = lines[i]

        if line.lstrip().startswith("//"): continue

        if "MERGE_LIST" in line:
            file.write("#pragma inline_asm(merge_%d)" % i + os.linesep)
            file.write("void merge_%d() {" % i + os.linesep)
            file.write("\t%s" % line + os.linesep)
            file.write("}" + os.linesep )
            continue

        if "INLINE_ASM" in line:
            params = line.strip().split("(")[1].split(")")[0].split(",")
            file.write("#pragma inline_asm(%s)" % params[0] + os.linesep)
            file.write("void %s() {" % params[0] + os.linesep)
            file.write("\t.INCLUDE %s" % params[1] + os.linesep)
            file.write("}" + os.linesep )
            continue
        
        file.write(line)

    