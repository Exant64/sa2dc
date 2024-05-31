#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// i tried to decomp it as accurately as possible
// i realized in the middle of it that it was a mistake cuz the code is just stupid lol
// but i didn't wanna throw out all the work so i ended up finishing it, not sure what to do now
// it does work though afaik

// there's one place where i took a bit of creative liberty, the way they removed the output file incase it failed
// is they had a "del " string that they copypasted the outputfilename to the end of it and then called
// system() on that string if it failed
// except they didnt even make enough space for the filename so it overwrote some error strings
// that + i didn't wanna use system() so im using remove()

uint8_t args_filler;
long unsigned int args_start_address;
long unsigned int args_end_address;

bool args_filler_enabled;
bool args_start_address_enabled;
bool args_end_address_enabled;

uint32_t parseHex(const char* str) {
    char* pEnd;
    // im assuming its strtol, not sure what they could've used, the thing they used is inlined, but definitely looks
    // standard library-esc

    uint32_t num = strtol(str, &pEnd, 16);

    if (*pEnd) {
        fprintf(stderr, "ERROR: invalid hexadecimal parameter\n");
        exit(1);
    }

    return num;
}

FILE *inputFile, *outputFile;

uint8_t ReadByte() {
    return (uint8_t)fgetc(inputFile);
}

int ReadInt16() {
    uint8_t v0 = fgetc(inputFile);
    return ((uint8_t)fgetc(inputFile) << 8) + v0;
}

int ReadInt32() {
    uint8_t v3 = fgetc(inputFile);
    uint8_t v2 = fgetc(inputFile);
    uint8_t v0 = fgetc(inputFile);
    return ((v2 + ((v0 + ((uint8_t)fgetc(inputFile) << 8)) << 8)) << 8) + v3;
}

typedef struct program_segment {
    int p_offset;
    int p_vaddr;
    int p_filesz;
} program_segment;

int phnum;
int phoff;
program_segment** pSegments;

void write_filler(uint32_t startaddr, int count) {
    for (int i = 0; i < count; i++) {
        uint32_t vaddr = startaddr + i;
        if (vaddr >= args_start_address && args_end_address > vaddr) {
            fputc(args_filler, outputFile);
        }
    }
}

void copy_bytes(uint32_t startaddr, int count) {
    for (int i = 0; i < count; i++) {
        uint32_t vaddr = startaddr + i;
        char byte = ReadByte();
        if (vaddr >= args_start_address && args_end_address > vaddr) {
            fputc(byte, outputFile);
        }
    }
}

void ReadElfHeader(void) {
    if (ReadByte() != '\x7F' || ReadByte() != 'E' || ReadByte() != 'L' || ReadByte() != 'F') {
        fprintf(stderr, "ERROR: input file is not an ELF file\n");
        exit(1);
    }

    ReadByte();
    if (ReadByte() != 1) {
        fprintf(stderr, "ERROR: input file is not little endian\n");
        exit(1);
    }

    ReadByte();
    ReadByte();
    for (int i = 0; i < 8; i++) {
        ReadByte();
    }

    // ET_EXEC
    if (ReadInt16() != 2) {
        fprintf(stderr, "ERROR: input file is not absolute\n");
        exit(1);
    }

    ReadInt16();
    ReadInt32();
    ReadInt32();
    phoff = ReadInt32();
    ReadInt32();
    ReadInt32();
    ReadInt16();

    if (ReadInt16() != 32) {
        fprintf(stderr, "ERROR: input file is not absolute\n");
        exit(1);
    }

    phnum = ReadInt16();
}

int CompareFunction(const void* p1, const void* p2) {
    const program_segment* a1 = *(const program_segment**)p1;
    const program_segment* a2 = *(const program_segment**)p2;

    int diff = a1->p_vaddr - a2->p_vaddr;
    if (!diff)
        diff = a1->p_filesz - a2->p_filesz;

    if (diff > 0)
        return 1;

    if (diff < 0)
        return -1;

    return 0;
}

// the first segment's address is the elf's "start"
static inline long unsigned int get_module_start_address() {
    return pSegments[0]->p_vaddr;
}

// and the last segment's address + size is the elf's "end"
static inline long unsigned int get_module_end_address() {
    program_segment* seg = pSegments[phnum - 1];
    return seg->p_filesz + seg->p_vaddr;
}

static inline bool check_extension(const char* str, const char* ext) {
    int len = strlen(str);

    return len >= 5 && str[len - 4] == '.' && tolower(str[len - 3]) == ext[0] && tolower(str[len - 2]) == ext[1] &&
           tolower(str[len - 1]) == ext[2];
}

int main(int argc, const char** argv) {
    int argi;

    fprintf(stderr, "ELF2BIN: ELF -> binary converter Ver. 0.83\n");
    fprintf(stderr, "Copyright (c) 1998, Hitachi, Ltd.  All rights reserved.\n");

    args_filler_enabled = false;
    args_start_address_enabled = false;
    args_end_address_enabled = false;

    if (argc == 1) {
        fprintf(stderr, "USAGE: elf2bin [-b xx] [-s xxxxxxxx] [-e xxxxxxxx] <input> [<output>]\n");
        return 1;
    }

    for (argi = 1; argi < argc; argi += 2) {
        const char* argument = argv[argi];
        const char* value = argv[argi + 1];

        // if it doesn't start with '-' it's not valid
        if (argument[0] != '-')
            break;

        switch (tolower(argument[1])) {
            // filler byte
            case 'b':
                if (argument[2]) {
                    fprintf(stderr, "ERROR: invalid switch %s\n", argument);
                    return 1;
                }

                if (args_filler_enabled) {
                    fprintf(stderr, "ERROR: -b specified twice\n");
                    return 1;
                }

                if (argi - argc == -1) {
                    fprintf(stderr, "ERROR: missing hexadecimal value\n");
                    return 1;
                }

                uint32_t number = parseHex(value);
                if (number > 0xFF) {
                    fprintf(stderr, "ERROR: filler data out of range\n");
                    return 1;
                }
                args_filler = number;
                fprintf(stderr, "filler: %02X\n", args_filler);

                args_filler_enabled = true;
                break;

            // specified end address
            case 'e':
                if (argument[2]) {
                    fprintf(stderr, "ERROR: invalid switch %s\n", argument);
                    return 1;
                }

                if (args_end_address_enabled) {
                    fprintf(stderr, "ERROR: -s specified twice\n");
                    return 1;
                }

                if (argi - argc == -1) {
                    fprintf(stderr, "ERROR: missing hexadecimal value\n");
                    return 1;
                }

                args_end_address = parseHex(value);
                args_end_address_enabled = true;
                break;

            // specified start address
            case 's':
                if (argument[2]) {
                    fprintf(stderr, "ERROR: invalid switch %s\n", argument);
                    return 1;
                }

                if (args_start_address_enabled) {
                    fprintf(stderr, "ERROR: -s specified twice\n");
                    return 1;
                }

                if (argi - argc == -1) {
                    fprintf(stderr, "ERROR: missing hexadecimal value\n");
                    return 1;
                }

                args_start_address = parseHex(value);
                args_start_address_enabled = true;
                break;
            default:
                fprintf(stderr, "ERROR: invalid switch %s\n", argument);
                break;
        }
    }

    if (argi == argc) {
        fprintf(stderr, "ERROR: input file not specified\n");
        return 1;
    }

    char** inputOutputPair = &argv[argi];
    char* fileName = inputOutputPair[0];
    char* outFileName;
    int v8 = strlen(fileName) + 1;
    int v30 = v8 - 1;

    if (check_extension(fileName, "abs") || check_extension(fileName, "elf")) {
        if (argi - argc == -1) {
            outFileName = malloc(v8);
            if (!outFileName) {
                fprintf(stderr, "FATAL: insufficient memory\n");
                exit(1);
            }

            strcpy(outFileName, fileName);
            memcpy(&outFileName[v30 - 3], "bin", 3);

            ++argi;
        } else {
            argi += 2;
            if (argc > argi) {
                fprintf(stderr, "ERROR: extra argument specified\n");
                exit(1);
            }

            outFileName = inputOutputPair[1];
        }
    } else {
        outFileName = malloc(v30 + 5);
        if (!outFileName) {
            fprintf(stderr, "FATAL: insufficient memory\n");
            exit(1);
        }

        int i = 0;
        while (fileName[i] != '.' || fileName[i] != '\0')
            i++;
        fileName[i] = '\0';

        if (argi - argc == -1) {
            // this should just really be a switch lol
            // if im understanding correctly this just gets the input argument
            // ...which is what all the previous code does too so idk why this one does it like this?
            if (args_start_address_enabled + args_end_address_enabled + args_filler_enabled == 0)
                strcpy(outFileName, argv[1]);
            if (args_start_address_enabled + args_end_address_enabled + args_filler_enabled == 1)
                strcpy(outFileName, argv[3]);
            if (args_start_address_enabled + args_end_address_enabled + args_filler_enabled == 2)
                strcpy(outFileName, argv[5]);
            if (args_start_address_enabled + args_end_address_enabled + args_filler_enabled == 3)
                strcpy(outFileName, argv[7]);
            strcat(outFileName, ".bin");
        } else {
            if (argi + 1 <= argc) {
                fprintf(stderr, "ERROR: input error\n");
                exit(1);
            }

            outFileName = inputOutputPair[1];
            argi += 2;
        }
    }

    if (argi + 1 < argc) {
        fprintf(stderr, "ERROR: extra argument specified\n");
        exit(1);
    }

    inputFile = fopen(fileName, "rb");
    if (!inputFile) {
        fprintf(stderr, "FATAL: cannot open input file %s\n", fileName);
        return 1;
    }

    ReadElfHeader();

    if (!phnum) {
        fprintf(stderr, "ERROR: no program segments\n");
        return 1;
    }

    pSegments = malloc(sizeof(program_segment*) * phnum);
    if (!pSegments) {
        fprintf(stderr, "FATAL: insufficient memory\n");
        return 1;
    }

    // check if program segment header offset is valid
    if (fseek(inputFile, phoff, 0)) {
        fprintf(stderr, "ERROR: invalid input file format\n");
        return 1;
    }

    // read all program segment headers
    for (int i = 0; i < phnum; i++) {
        pSegments[i] = malloc(sizeof(program_segment));

        if (!pSegments[i]) {
            fprintf(stderr, "FATAL: insufficient memory\n");
            return 1;
        }

        // PT_LOAD | Loadable segment
        if (ReadInt32() != 1) {
            fprintf(stderr, "ERROR: invalid program segment\n");
            return 1;
        }

        pSegments[i]->p_offset = ReadInt32();
        pSegments[i]->p_vaddr = ReadInt32();
        ReadInt32();
        pSegments[i]->p_filesz = ReadInt32();
        ReadInt32();
        ReadInt32();
        ReadInt32();
    }

    // sort based on start address, or if start addresses are identical, sort based on size
    qsort(pSegments, phnum, sizeof(pSegments), CompareFunction);

    if ((get_module_end_address() < args_start_address && args_start_address_enabled == 1) ||
        (get_module_start_address() > args_end_address && args_end_address_enabled == 1)) {
        fprintf(stderr, "ERROR: invalid input address \n");
        fprintf(stderr, "   Module address: %08lx - %08lx\n", get_module_start_address(), get_module_end_address());
        return 1;
    }

    // check if any of the segments overlap
    for (int i = 0; i < phnum - 1; i++) {
        program_segment* curr = pSegments[i];
        program_segment* next = pSegments[i + 1];
        if (curr->p_filesz + curr->p_vaddr > next->p_vaddr) {
            fprintf(stderr, "phnum : %d\n", phnum);

            // this seems to be undefined behavior + all of this feels like debug printouts lol
            fprintf(stderr, "%x \n", pSegments[i - 1]->p_vaddr);

            fprintf(stdout, "%d / %d - %x > %x \n", i, phnum, curr->p_filesz + curr->p_vaddr, next->p_vaddr);

            fprintf(stderr, "ERROR: program segment overlap\n");
            return 1;
        }
    }

    // this is where copying in the filename into "del command" happened

    fprintf(stderr, "Module address: %08lx - %08lx\n", get_module_start_address(), get_module_end_address());
    if (!args_start_address_enabled)
        args_start_address = get_module_start_address();
    if (!args_end_address_enabled)
        args_end_address = get_module_end_address();
    fprintf(stderr, "Convert address: %08lx - %08lx\n", args_start_address, args_end_address);

    if ((args_start_address_enabled == 1 || args_end_address_enabled == 1) &&
        (args_end_address <= args_start_address || get_module_start_address() == args_end_address ||
         get_module_end_address() == args_start_address)) {
        if (args_end_address < args_start_address) {
            fprintf(stderr, "ERROR: input address error\n   start address >= end address\n");
            return 1;
        }

        if (get_module_start_address() == args_end_address) {
            fprintf(stderr, "ERROR: input address error\n   Module start address = end address\n");
            return 1;
        }

        if (get_module_end_address() == args_start_address) {
            fprintf(stderr, "ERROR: input address error\n   start address = Module end address\n");
            return 1;
        }

        fprintf(stderr, "ERROR: input address error\n   start address = end address\n");
        return 1;
    }

    outputFile = fopen(outFileName, "wb");
    if (!outputFile) {
        fprintf(stderr, "FATAL: cannot create output file %s\n", outFileName);
        exit(1);
    }

    // if the module starts after the specified start address, write filler in front of it
    if (get_module_start_address() > args_start_address)
        write_filler(args_start_address, get_module_start_address() - args_start_address);

    int last_v_addr = pSegments[0]->p_vaddr;
    int last_file_size = pSegments[0]->p_filesz;

    for (int i = 0; i < phnum - 1; i++) {
        if (fseek(inputFile, pSegments[i]->p_offset, 0)) {
            fprintf(stderr, "ERROR: invalid input file format\n");
            fclose(outputFile);
            remove(outFileName);
            return 1;
        }

        copy_bytes(pSegments[i]->p_vaddr, pSegments[i]->p_filesz);

        // if there's a gap between the two segments write filler
        program_segment* nextSegment = pSegments[i + 1];
        if (nextSegment->p_filesz) {
            int lastSegmentEnd = last_v_addr + last_file_size;
            write_filler(lastSegmentEnd, nextSegment->p_vaddr - (lastSegmentEnd));

            last_v_addr = nextSegment->p_vaddr;
            last_file_size = nextSegment->p_filesz;
        }
    }

    // the last segment, same sanity check as before
    if (fseek(inputFile, pSegments[phnum - 1]->p_offset, 0)) {
        printf("ERROR: invalid input file format\n");
        fclose(outputFile);
        remove(outFileName);
        return 1;
    }

    // copy the last segment
    copy_bytes(pSegments[phnum - 1]->p_vaddr, pSegments[phnum - 1]->p_filesz);

    // and if the segment doesn't end at the specified address but earlier, do filler till the end address
    if (get_module_end_address() < args_end_address && pSegments[phnum - 1]->p_filesz) {
        int lastSegmentEnd = last_v_addr + last_file_size;
        write_filler(lastSegmentEnd, args_end_address - (lastSegmentEnd));
    }

    fclose(inputFile);
    fclose(outputFile);
}