.align 4
.global _start, _getPrevFP, _crash, _malloc_test

_start:
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

_malloc_test:
    mov x1, x0
    mov x0, 0
    mov x2, 3
    mov x3, 4098
    mov x4, -1
    mov x5, 0
    mov x16, 197
    svc 0
    ret
