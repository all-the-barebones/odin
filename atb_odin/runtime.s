section .text

global _start
extern main

_start:
    .entry:
        mov rax, cr0
        and ax, 0xFFFB
        or ax, 0x2
        mov cr0, rax
        mov rax, cr4
        or ax, 3 << 9
        mov cr4, rax

        mov rsp, stack_top
        xor rax, rax
        mov rdi, 0xb8000
        mov rcx, 80*25*2
        rep stosb

        mov ecx, 0xc0000100 ; fsbase
        lea rax, [rel tlsaddr]
        mov rdx, rax
        shr rdx, 32
        wrmsr

        call main

    .doloop:
        cli
        hlt
        jmp .doloop

section .data
    times 64 db 0
tlsaddr:
    dq tlsaddr
    times 64 db 0
section .bss
    resq 0x4000
stack_top:
