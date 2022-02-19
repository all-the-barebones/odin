package odin_atb;

import "core:strings"

vga_ptr: rawptr = cast(rawptr)cast(uintptr)0xb8000;
vga: [^]u8 = cast([^]u8)vga_ptr;
vgaoff: uintptr = 2;

putc :: proc(c: rune) {
    vga[vgaoff+0] = cast(u8)c;
    vga[vgaoff+1] = 0xf;
    vgaoff += 2;
}
foreign {
    @(link_name = "debugc")
    debugc :: proc "c"(c: u8) ---

    @(link_name = "inb")
    inb :: proc "c"(c: u16) -> u8 ---
}
puts :: proc(s: string) {
    for chr, i in s {
        debugc(cast(u8)chr)
        putc(chr)
    }
    debugc(cast(u8)'\n')
    vgaoff = (vgaoff / 160 + 1) * 160 + 2;
    vga[vgaoff] = cast(u8)' ';
    vga[vgaoff+1] = 0xf0;
}

main :: proc() {
    m := make(map[string]string)
    m["maps_test"] = "With maps!"
    defer delete(m)
    puts("Welcome to odin on bare metal x86_64!")
    puts(m["maps_test"])
    puts("  => github.com/all-the-barebones/odin")
}
