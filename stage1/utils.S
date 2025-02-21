.align 4
.pool
.set target, 0xbadbad10badbad10
.global _start, _getPrevFP, _crash

_start:
    ldr x0, =target  // address of the JS object
    ldr x0, [x0, 16] // set x0 to the address of the UInt8 array
    b _c_start
    ret

_getPrevFP:
    ldr x0, [fp]
    ret

_crash:
    mov x12, x0
    mov x1, 0x4141
    mov x2, 0x4141
    mov x3, 0x4141
    mov x4, 0x4141
    mov x5, 0x4141
    mov x6, 0x4141
    mov x7, 0x4141
    mov x8, 0x4141
    mov x9, 0x4141
    mov x10, 0x4141
    mov x11, 0x4141
    mov x13, 0x0
    mov x16, 20
    svc 0
    mov x1, 6
    mov x16, 37
    svc 0
    ret
