mkdir obj
mips64-elf-gcc -Wall -O2 -mtune=vr4300 -march=vr4300 -mabi=32 -fomit-frame-pointer -G0 -c src/main.c
move main.o obj/

armips main2.asm
n64crc "rom/Quest64.mod.z64"