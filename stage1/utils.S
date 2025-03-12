.global _crash

_crash:
    mov x12, x0
    mov x13, 0x0

    mov x16, 20
    svc 0
    mov x1, 6
    mov x16, 37
    svc 0
    ret
