.section ".text.boot"

.globl _start
//.align 2

_start:
	mrs	x1, mpidr_el1 //Register contains ID stuff, bits 7:0 identifies threads
	and	x1, x1, #3 // Built for a 4 core cpu, so if x1 > 0 then not the primary core
	cbz	x1, 2f // if 0 jmp to 2 compare and branch on zero f  forwards 

1:	wfe //Wait for event  spinlock 
	b	1b	//Branching statement, jumps to 1, b backwards 

2:
	ldr	x1, =_start
	mov	sp, x1

	ldr	x1, =__bss_start
	ldr	w2, =__bss_size
	cbz	w2, 4f //Forward jump to 4 if _bss_size = 0

3:
	str	xzr, [x1], #8 //post index addition "loads" x1 into xzr  zero register  and performs x1 += 8
	sub	w2, w2, #1 //unkown function of incrementing x1 by 8
	cbnz	w2, 3b

4:
	bl	main //branch to main while copying adress of next inst into  r14
	b	1b

	//Exit
	/*
	mov	x0, #0
	mov	w8, #93
	svc	#0
	*/

