#!/usr/bin/env python3
import sys
import ast

out_lines = []

with open(sys.argv[1], "r") as file:
    lines = [line.rstrip('\n') for line in file]
    length = len(lines)

    # this is needed to remove the inline asm stuff's exports from the top of the file since those labels are deleted
    blacklist_export = set()
    blacklist_import = set()
    exports = set()
    imports = set()
    labels = set()

    merge_dict = {}

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
                    if ":" in include_file_line:
                        labels.add(include_file_line.split(":")[0])
                    if include_file_line.strip().startswith(".EXPORT"):
                        exportName = include_file_line.split('.EXPORT')[1].strip()
                        exports.add(exportName)
            continue

        if ":" in line:
            labels.add(line.split(":")[0])

        if line.startswith("MERGE_LIST"):
            func_name = lines[i-2].split(" ")[0]
            blacklist_export.add(func_name[:-1]) # -1 to remove : from the label
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
            imported_label = lines[i].strip().split(".IMPORT")[1].strip()
            if imported_label in exports or imported_label in labels:
                continue

        if lines[i].strip().startswith("MERGE_LIST"):
            # remove last two function definition lines
            # ex. _func_9380:                      ; function: func_9380
            #                                      ; frame size=0
            out_lines = out_lines[:len(out_lines)-2]

            # parse the merge list
            python_array = lines[i].split("MERGE_LIST(")[1].split(");")[0]
            parsed_list = ast.literal_eval(python_array)

            # create a dictionary from it
            label_dict = {}
            for lists in parsed_list:
                if "h'" in lists[0]:
                    label_dict[lists[0].upper()] = lists[1]
                else:
                    label_dict[lists[0]] = lists[1]

            # find the last data section (surrounded by ALIGN 32's) and the start of the func
            align_count = 0
            function_start_index = 0
            stop_align = 0
            align_start_index = -1
            for rename_line_index in range(len(out_lines)-1, -1, -1):
                rename_line = out_lines[rename_line_index]
                
                if "ALIGN" in rename_line:
                    align_count += 1
                    if align_count == 1:
                        stop_align = rename_line_index
                
                #if align_count == 1:
                if ":" in rename_line and align_start_index == -1:
                    align_start_index = rename_line_index              

                if "; function" in rename_line:
                    function_start_index = rename_line_index + 2
                    break
                
            # get the label name that the references use
            label_rename_name = out_lines[align_start_index].split(":")[0]
            print(label_rename_name)

            # find all label offsets and the data they have
            label_offset_dict = {}
            i = 0

            end_index = len(out_lines) - 1
            if "ALIGN" not in out_lines[end_index]:
                end_index += 1
            
            for label_discovery_index in range(align_start_index + 1, end_index, 1):
                label_discovery_line = out_lines[label_discovery_index]
                print(label_discovery_line)
                sym = label_discovery_line.split(".DATA.")[1][1:].strip()

                if i == 0:
                    label_offset_dict[label_rename_name] = sym
                else:
                    label_offset_dict[label_rename_name+"+"+str(i)] = sym

                if ".L" in label_discovery_line:
                    i += 4
                else:
                    i += 2

            if "ALIGN" in out_lines[align_start_index - 1]:
                out_lines = out_lines[:align_start_index-1]
            else:
                out_lines = out_lines[:align_start_index]
            
            for function_line_index in range(function_start_index, len(out_lines), 1):
                func_line = out_lines[function_line_index]
                if label_rename_name in func_line:
                    find = label_rename_name + func_line.split(label_rename_name)[1].split(",")[0]
                    replace = label_dict[label_offset_dict[find]]
                    out_lines[function_line_index] = func_line.replace(find, replace)
            
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
