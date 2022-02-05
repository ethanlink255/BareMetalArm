CFILES = $(wildcard *.c)
OFILES = $(CFILES:.c=.o)
GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles
GCCPATH = ../gcc-arm-10.3-2021.07-aarch64-aarch64-none-elf/bin

all: clean kernel8.img

start.o: start.S
	$(GCCPATH)/aarch64-none-elf-gcc $(GCCFLAGS) -c start.S -o start.o
%.o: %.c
	$(GCCPATH)/aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

kernel8.img: start.o $(OFILES)
	$(GCCPATH)/aarch64-none-elf-ld -nostdlib start.o $(OFILES) -T link.ld -o kernel8.elf
	$(GCCPATH)/aarch64-none-elf-objcopy -O binary kernel8.elf kernel8.img

clean:
	/bin/rm kernel8.elf *.o *.img > /dev/null 2> /dev/null || true

run:
	cat kernel8.img | nc 192.168.64.1 282
