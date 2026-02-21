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

        our_linesep = '\n'

        if "MERGE_LIST" in line:
            file.write("#pragma inline_asm(merge_%d)" % i + our_linesep)
            file.write("void merge_%d() {" % i + our_linesep)
            file.write("\t%s" % line + our_linesep)
            file.write("}" + our_linesep )
            continue

        if "INLINE_ASM" in line:
            params = line.strip().split("(")[1].split(")")[0].split(",")
            if len(params) == 3:
                file.write("#pragma inline_asm(%s(size=%s))" % (params[0], params[1]) + our_linesep)
                file.write("void %s() {" % params[0] + our_linesep)
                file.write("\t.INCLUDE %s" % params[2] + our_linesep)
                file.write("}" + our_linesep )
            else:
                file.write("#pragma inline_asm(%s)" % (params[0]) + our_linesep)
                file.write("void %s() {" % params[0] + our_linesep)
                file.write("\t.INCLUDE %s" % params[1] + our_linesep)
                file.write("}" + our_linesep )
            continue
        
        file.write(line)

    