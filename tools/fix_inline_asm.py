#!/usr/bin/env python3
import sys

out_lines = []

with open(sys.argv[1], "r") as file:
    lines = [line.rstrip('\n') for line in file]
    length = len(lines)

    # this is needed to remove the inline asm stuff's exports from the top of the file since those labels are deleted
    blacklist_export = set()
    blacklist_import = set()
    exports = set()
    imports = set()

    for i in range(len(lines)):
        line = lines[i].strip()
        if line.startswith(".IMPORT"):
            imports.add(line.split('.IMPORT')[1].strip())

        if line.startswith(".INCLUDE"):
            func_name = lines[i-2].split(" ")[0]
            includeFile = line.split('.INCLUDE')[1].strip().split("\"")[1]
            blacklist_export.add(func_name[:-1]) # -1 to remove : from the label
            with open(includeFile, "r") as included:
                for include_file_line in included.readlines():
                    if include_file_line.strip().startswith(".EXPORT"):
                        exportName = include_file_line.split('.EXPORT')[1].strip()
                        exports.add(exportName)
            continue

        if line.startswith(".EXPORT"):
            exportName = line.split('.EXPORT')[1].strip()
            exports.add(exportName)

    block_lines = -1

    # make sure rodata is put in the file after the existing rodata
    # that is generated from the c code itself
    import_queue = []

    for i in range(length):
        if i < block_lines:
            continue
        if lines[i].strip().startswith(".EXPORT"):
            # dont include exports for removed labels
            if lines[i].strip().split(".EXPORT")[1].strip() in blacklist_export:
                continue
        
        if lines[i].strip().startswith(".IMPORT"):
            # dont include imports for stuff exported in this file
            if lines[i].strip().split(".IMPORT")[1].strip() in exports:
                continue

        if lines[i].strip().startswith(".INCLUDE"):
            # remove last two function definition lines
            # ex. _func_9380:                      ; function: func_9380
            #                                      ; frame size=0
            out_lines = out_lines[:len(out_lines)-2]
            includeFile = lines[i].split('.INCLUDE')[1].strip().split("\"")[1]
            if "rodata" in includeFile:
                import_queue.append(includeFile)
            else:
                with open(includeFile, "r") as included:
                    for line in included.readlines():
                        if line.strip().startswith(".IMPORT"):
                            importName = line.split('.IMPORT')[1].strip()
                            # dont import anything that's exported in the same file                        
                            if importName in exports:
                                continue
                        out_lines.append(line)
            continue
        
        if lines[i].strip() == ".ALIGN      4" and lines[i+1].strip() == "RTS" and lines[i+2].strip() == "NOP":
            #if lines[i+3].strip() != ".END":
                #out_lines.append("\t.ALIGN\t32\n")
            block_lines = i+3
            continue

        out_lines.append(lines[i] + "\n")

       
        #else:
        

with open(sys.argv[1], "w") as file:
    if len(import_queue) > 0:
        # remove .end
        out_lines = out_lines[:len(out_lines)-1]
        # this MIGHT break stuff in the future, not sure
        # currently it fixed an alignment issue in al_field.c
        # out_lines.append("\t\t.ALIGN 4\n")
        for includeFile in import_queue:
            with open(includeFile, "r") as included:
                for line in included.readlines():
                    if line.strip().startswith(".IMPORT"):
                        importName = line.split('.IMPORT')[1].strip()
                        # dont import anything that's exported in the same file                        
                        if importName in exports:
                            continue
                    out_lines.append(line)

    file.writelines(out_lines)
