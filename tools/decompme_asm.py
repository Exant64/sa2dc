#!/usr/bin/env python3
# generates decomp.me target asm syntax friendly file

import sys

if len(sys.argv) != 3:
    print("USAGE: decompme_asm inputfile outputfile \n")
    sys.exit(1)

with open(sys.argv[1], "r") as file:
    lines = file.readlines()
    collect_unused_labels = []
    labels = []
    for line in lines:
        line = line.split(";")[0]
        if ":" in line:
            labels.append(line.strip().split(":")[0])
        if "bsr" in line:
            collect_unused_labels.append(line.strip().split("bsr")[1].strip())
        if "bra" in line:
            collect_unused_labels.append(line.strip().split("bra")[1].strip())
    with open(sys.argv[2], "w") as out_file:
        for line in lines:
            line = line.split(";")[0]
            line = line.replace("EXPORT", "global")
            line = line.replace(".SECTION P", ".SECTION .text")
            line = line.replace(".SECTION D", ".SECTION .data")
            line = line.replace(".SECTION C", ".SECTION .rodata")
            line = line.replace("data.l", "long")
            line = line.replace("data.w", "word")
            line = line.replace("data.b", "byte")
            out_file.write(line + "\n")
        for label in collect_unused_labels:
            if label not in labels:
                out_file.write(label + ":")