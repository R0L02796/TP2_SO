include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES=$(wildcard [^_]*.c)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS=$(SOURCES:.c=.o)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c _loader.c $(SOURCES) 

%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@


all: $(MODULE)

$(MODULE): $(OBJECTS_ASM) $(OBJECTS)
	$(LD) -T sampleCodeModule.ld _loader.o $(OBJECTS) $(OBJECTS_ASM) -o ../$(MODULE)
	$(LD) -T sampleCodeModule.ld --oformat=elf64-x86-64 _loader.o $(OBJECTS) $(OBJECTS_ASM) -o ../0000-sampleCodeModule.elf

clean:
	rm -rf *.o

.PHONY: all clean
