#define VGA_GRAPHICS_BASE (char*) 0xA0000;

void putpx(int x, int y, int mask, char set){
    char * vga_base = VGA_GRAPHICS_BASE;
    *(vga_base + 640/8 * y + x) = set;
}