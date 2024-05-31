#!/usr/bin/env python3

import sys
import os

lines = []
with open(sys.argv[1], "r") as file:
    lines = file.readlines()

with open(sys.argv[2], "w") as file:
    for line in lines:
        if "INLINE_ASM" in line:
            params = line.strip().split("(")[1].split(")")[0].split(",")
            file.write("#pragma inline_asm(%s)" % params[0] + os.linesep)
            file.write("void %s() {" % params[0] + os.linesep)
            file.write("\t.INCLUDE %s" % params[1] + os.linesep)
            file.write("}" + os.linesep )
            continue
        
        file.write(line)

    