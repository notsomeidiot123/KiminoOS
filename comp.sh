nasm -f bin -o bin/CLIOSBOOT Assembly/boot.s
nasm -f elf Assembly/kentry.s -o o/kentry.o
nasm -f elf Assembly/idt.s -o o/idt.o
gcc c/main.c -c -g -ffreestanding -fno-pie -o o/main.o -m32 -w -Wimplicit-function-declaration -Wimplicit
ld --oformat binary o/kentry.o o/main.o o/idt.o -o bin/kernel -Ttext 0x1000 -melf_i386
gcc applications/test.c -ffreestanding -fno-pie -w -o o/test.o -c -g -m32
ld --oformat binary o/test.o -o bin/test -melf_i386
cat bin/CLIOSBOOT bin/kernel headers/txt > CLIOS.iso

# qemu-img resize CLIOS.iso 2g

qemu-system-x86_64 CLIOS.iso -d int -M smm=off -no-reboot -no-shutdown
rm hd.txt
hd CLIOS.iso> hd.txt
### hexdump contents to see successful drive write  ###