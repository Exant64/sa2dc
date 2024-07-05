#!/usr/bin/env python3
import sys

with open(sys.argv[1], "r") as symtxt:
    lines = symtxt.readlines()
    for line in lines:
        spl = line.split(" = ")
        print(f"\t.export _{spl[0]}")
        print(f"_{spl[0]}: .equ h'{spl[1].rstrip(';')}")
        