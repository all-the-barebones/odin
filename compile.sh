set -ex

nasm -fbin bootelf/bootelf.asm -o build/bootelf.bin -Ibootelf
clang -target x86_64-elf -Iinc atb_odin/runtime.c -c -o build/runtime.o -ggdb -O0
odin build atb_odin -build-mode:llvm-ir -target:linux_amd64 -opt:0 -out:build/odincode.ll -debug
clang -target x86_64-elf build/odincode.ll -c -o build/odincode.o -ggdb -O0
nasm -felf64 atb_odin/runtime.s -o build/asmrt.o
ld.lld -Tatb_odin/linker.ld build/odincode.o build/runtime.o build/asmrt.o -o build/atb_odin.elf

cat build/bootelf.bin build/atb_odin.elf >build/atb_odin.bin
