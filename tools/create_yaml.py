with open("sa2.txt", "r") as layout:
    for line in layout.readlines():
        if "=" not in line:
            continue
    
        spl = line.split("=")
        name = spl[0].strip()
        range = spl[1].strip()
        range_spl = range.split("-")
        if name == "":
            print("- [0x%X, asm]" % (int(range_spl[0], 16) - 0x8C010000))    
        else:
            print("- [0x%X, asm, %s]" % (int(range_spl[0], 16) - 0x8C010000, name))