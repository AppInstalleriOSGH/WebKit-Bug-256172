.align 4
.pool
.set target, 0xbadbad10badbad10
.global _start, _crash

_start:
    ldr x0, =target  // address of the JS object
    ldr x0, [x0, 16] // set x0 to the address of the UInt8 array
    b _c_start
    ret

_crash:
    mov x16, 20
    svc 0
    mov x1, 6
    mov x16, 37
    svc 0
    ret
