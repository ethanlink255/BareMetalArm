#include "gpio.h"


#define AUX_ENABLE      ((volatile unsigned int*)(MMIO_BASE+0x00215004))
#define AUX_MU_IO       ((volatile unsigned int*)(MMIO_BASE+0x00215040))
#define AUX_MU_IER      ((volatile unsigned int*)(MMIO_BASE+0x00215044))
#define AUX_MU_IIR      ((volatile unsigned int*)(MMIO_BASE+0x00215048))
#define AUX_MU_LCR      ((volatile unsigned int*)(MMIO_BASE+0x0021504C))
#define AUX_MU_MCR      ((volatile unsigned int*)(MMIO_BASE+0x00215050))
#define AUX_MU_LSR      ((volatile unsigned int*)(MMIO_BASE+0x00215054))
#define AUX_MU_MSR      ((volatile unsigned int*)(MMIO_BASE+0x00215058))
#define AUX_MU_SCRATCH  ((volatile unsigned int*)(MMIO_BASE+0x0021505C))
#define AUX_MU_CNTL     ((volatile unsigned int*)(MMIO_BASE+0x00215060))
#define AUX_MU_STAT     ((volatile unsigned int*)(MMIO_BASE+0x00215064))
#define AUX_MU_BAUD     ((volatile unsigned int*)(MMIO_BASE+0x00215068))


void uart_init()
{
	register unsigned int r; //unsigned int 4 bytes, (or 2?)

	*AUX_ENABLE |= 1;
	*AUX_MU_CNTL = 0; //extra control register?
	*AUX_MU_LCR = 3; // 3 => 11 8 databits
	*AUX_MU_MCR = 0; // Modem Control Register 0 = > Clear register
	*AUX_MU_IER = 0; // Interrupt Enable Register 0 = > Clear register
	*AUX_MU_IIR = 0xc6; //Interrupt Identification Register 11000110 =>
			    /*	7, 6 11 =>  FIFO Enabled
			        3, 2, 1  011 => Line status change
				0  0 => Interrupt Pending
				Disable Interrupts?
			    */
	*AUX_MU_BAUD = 270; //baudrate = clockfreq / (8*rate+1) 115200 baud

	//map UART1 to GPIO bins
	r = *GPFSEL1; //gets value at GPFSEL 4 bytes
	r &= ~((7<<12)|(7<<15)); /* Some kind of mask, GPIO 14, 15
					Bit Shift left 7 by 12:
					0111 => 111,0000,0000,0000 => 0x7000

					Bit Shift left 7 by 15:
					0111 => 11,1000,0000,0000,0000 => 0x38000

					BitWise Or 0x7000 and 0x38000 = > 0x3f000

					BitWise Not => -0x3f000
					1111,1111,1111,1100,0000,1111,1111,1111
					0 bits are between 17-12, hardware mapping for pins 15, 14

					Val of r <= *0x3f200000
					CLEARS 15, 14
				 */
	*GPFSEL1 = r;
	*GPPUD = 0; //pin up down enable 15, 14
	r = 150; while(r--) { asm volatile("nop"); } //wait 150 clock cycles (maybe)
	*GPPUDCLK0 =


