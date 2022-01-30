boot.o: boot.S
	as -g -o boot.o boot.S
	ld -g -o boot boot.o
