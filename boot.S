//.section ".text.boot"

.global _start
.align 2

_start:
	mrs	x1, mpidr_el1 //Register contians ID stuff, bits 7:0 identifies threads
	and	x1, x1, #3 // Built for a 4 core cpu, so if x1 > 0 then not the primary core
	cbz	x1, 2f // if 0 jmp to 2 (compare and branch on zero)

1:	wfe //Wait for event (spinlock)
	b	1b	//???

2:
	ldr	x1, =_start
