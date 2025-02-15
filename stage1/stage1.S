.align 4

.pool
.set target, 0xbadbad10badbad10

.global _start

_start:
    ldr x0, =target
    b _c_start
    ret
