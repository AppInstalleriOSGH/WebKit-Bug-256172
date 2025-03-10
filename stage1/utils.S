.align 4
.pool
.set arg1, 0xbadbad10badbad10
.set arg2, 0xbadbad20badbad20
.set arg3, 0xbadbad30badbad30
.set arg4, 0xbadbad40badbad40
.global _start, _crash

_start:
    ldr x0, =arg1
    ldr x1, =arg2
    ldr x1, [x1, 16]
    ldr x2, =arg3
    ldr x2, [x2, 16]
    ldr x3, =arg4
    b _c_start
    ret

_crash:
    mov x12, x0
    mov x13, 0x0

    mov x16, 20
    svc 0
    mov x1, 6
    mov x16, 37
    svc 0
    ret
