#I used the zeldaret oot makefile as reference cuz i never did makefile before, sorry about that lol

#idk how we should do this with other overlay files + 1st_read later on

1ST_READ_KEY := 8C010000
1ST_READ_ROM := build/1ST_READ.bin
1ST_READ_ELF := $(1ST_READ_ROM:.bin=.elf)

CHAO_KEY := 0C500000
CHAO_ROM := build/CHAO.bin
CHAO_ELF := $(CHAO_ROM:.bin=.elf)

CHAOSTGRACE_KEY := 0C9C0000
CHAOSTGRACE_ROM := build/CHAOSTGRACE.bin
CHAOSTGRACE_ELF := $(CHAOSTGRACE_ROM:.bin=.elf)

COMPARE ?= 1

SRC_DIRS := $(shell find src -type d)
ASM_DIRS := $(shell find asm -type d -not -path '*/nonmatching/*')

C_FILES       := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
S_FILES       := $(foreach dir,$(ASM_DIRS),$(wildcard $(dir)/*.src))

#i use obj to "differentiate" between regular elf object files and the SYSROF thing that hitachi uses
O_FILES       := $(foreach f,$(S_FILES:.src=.obj),build/$f) \
                 $(foreach f,$(C_FILES:.c=.obj),build/$f) 

# path conversion for msys2 (msys2 doesnt seem to automatically convert foo/bar paths because windows apps are
# apparently supposed to support that, except SHC doesn't lol) (pls correct me if im wrong)
PATHHELP := tools/path_helper.sh

#using inline_asm automatically creates a .align 4, rts, and nop at the end of the function regardless of its contents
#we have to remove those to be able to include asm
#i guess this is sort of our equivalent of asm-processor
FIX_INLINE := tools/fix_inline_asm.py

FIX_INLINE_C := tools/inline_asm_c.py

DCSPLIT := tools/dcsplit/dcsplit.py
GENFSY := tools/genfsy.py

SHC_DIR := shc

MAKE = make
CC := $(SHC_DIR)/bin/shc.exe
AS := $(SHC_DIR)/bin/asmsh.exe
LD := $(SHC_DIR)/bin/lnk.exe
ELF2BIN := tools/elf2bin 
WINPATH := tools/winpath.sh
WIBO := tools/wibo

# wibo doesn't convert env vars yet afaik, so we do it manually
export SHC_LIB := $(shell $(WINPATH) $(abspath $(SHC_DIR)/bin))
export SHC_TMP := $(shell $(WINPATH) $(abspath $(SHC_DIR)/temp))

#the source dir (original layout was like that too) and katana includes
INCLUDEDIRS := src,Include,$(SHC_DIR)\\include

CFLAGS := -comment=nonest -cpu=sh4 -division=cpu -fpu=single -endian=little -extra=a=1800 -optimize=1 -pic=0 -macsave=0 \
	-speed -sjis -loop -string=const -round=nearest -inline -aggressive=2 -code=asmcode -include=$(INCLUDEDIRS)

ASFLAGS := -cpu=sh4 -endian=little -sjis -include=asm

LDFLAGS :=

$(shell mkdir -p build $(foreach dir, $(SRC_DIRS) $(ASM_DIRS), build/$(dir)))
$(shell mkdir -p shc/temp)

#also stolen from oot makefile
all: $(1ST_READ_ROM) $(CHAO_ROM) $(CHAOSTGRACE_ROM) 
ifeq ($(COMPARE),1)
	@md5sum -c checksum.md5
endif

setup: 
	$(MAKE) -C tools
	$(GENFSY) symbols.chaomodel.txt > asm/chaomodel_symbols.src
	$(DCSPLIT) 1st_read.yaml
	$(DCSPLIT) chao.yaml
	$(DCSPLIT) chaostgrace.yaml

clean:
	$(RM) -r $(ROM) $(ELF) CHAO.map build

.PHONY: all clean setup

$(CHAO_ROM): $(CHAO_ELF)
	$(ELF2BIN) -s $(CHAO_KEY) $< $@

$(CHAO_ELF): $(1ST_READ_ELF) build/asm/1st_read.obj $(O_FILES)
	$(LD) $(LDFLAGS) -sub=chao_lnk.sub

$(CHAOSTGRACE_ROM): $(CHAOSTGRACE_ELF)
	$(ELF2BIN) -s $(CHAOSTGRACE_KEY) $< $@

$(CHAOSTGRACE_ELF): $(1ST_READ_ELF) build/asm/1st_read.obj build/asm/chao.obj $(O_FILES)
	$(LD) $(LDFLAGS) -sub=chaostgrace_lnk.sub

$(1ST_READ_ROM): $(1ST_READ_ELF) 
	$(ELF2BIN) -s $(1ST_READ_KEY) $< $@

$(1ST_READ_ELF): $(O_FILES)
	$(LD) $(LDFLAGS) -sub=1st_read_lnk.sub
	$(shell mv 1st_read.fsy build/1st_read.fsy)

# wibo doesn't support envvar conversion so we use wine for shc
# wine crashes asmsh, and asmsh doesn't use the envvars
# so wibo works 
build/src/%.obj: src/%.c
	$(FIX_INLINE_C) $< build/$<
	$(PATHHELP) $(WIBO) $(CC) build/$< $(CFLAGS) -objectfile=$(@:.obj=.src)
	$(FIX_INLINE) $(@:.obj=.src)
	$(PATHHELP) $(WIBO) $(AS) $(@:.obj=.src) $(ASFLAGS) -object=$@

build/asm/%.obj: asm/%.src
	$(PATHHELP) $(WIBO) $(AS) $< $(ASFLAGS) -object=$@

build/asm/1st_read.obj:
	$(shell grep -v "_lbl_" build/1st_read.fsy > build/1st_read_tmp.fsy)
	$(PATHHELP) $(WIBO) $(AS) build/1st_read_tmp.fsy $(ASFLAGS) -object=$@

build/asm/chao.obj:
	$(shell grep -v "_lbl_" build/CHAO.fsy > build/CHAO_tmp.fsy)
	$(PATHHELP) $(WIBO) $(AS) build/CHAO_tmp.fsy $(ASFLAGS) -object=$@

