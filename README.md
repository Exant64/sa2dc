# Sonic Adventure 2 (Dreamcast)
This is a matching decompilation of Sonic Adventure 2 (US).
Currently it's set up for building 1ST_READ.bin (the main executable), CHAO.bin, CHAOSTGRACE.bin.

# Setup
I only tested on WSL, I doubt it currently works with Wine but it's worth a shot.
- The splitter requires sh4dis (and python)
```
pip install sh4dis
```
- Obtain Katana r11, and the folders /Include, /Dev/Hitachi, /Lib
- Put /Include in the repo
- create an "shc" folder
- put /Dev/Hitachi's contents in shc/bin
- put /Lib's contents in shc/lib
- Obtain and extract the rom for Sonic Adventure 2 (US) for the Dreamcast
- Decompress CHAO.PRS and CHAOSTGRACE.PRS
- Put 1ST_READ.bin and the decompressed files (make sure their extension is .bin) in the "rom" folder in the repo
- Run "make setup"

# Extra info
This project has been a thing since like 2021 but I always only had a couple of months a year to work on it. It originally used a very manual version of this splitter,
without the functions being split up. Now that I don't need to put the asm in the repo, I could finally make it public.

Todo:
- dcsplit section order auto-detection based on the split order
- dcsplit in separate repo
- wine support (or maybe that new wibo thing)
- rewrite the inline_asm_c/fix_inline_asm scripts in some more performant language, I'm not blaming python though, I wrote really lazy code lol
- the current setup in the makefile for the overlay files doesn't seem very sustainable considering how many are in the game, maybe it should somehow be autogenerated
- progress script
