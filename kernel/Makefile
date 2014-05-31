###################################
# Makefile for Kernel of nansl    #
###################################

ENTRYPOINT	= 0x30000000

# Programs, flags, etc.
ARM-TOOLS-PATH	= /home/bigc/Apps/arm-linux-toolchain-4.4.3/bin
HARDWARE-PATH	= ./hardware
SYS-FILES-PATH	= ./sys
OBJ-FILES-PATH	= ./OBJS
ASM		= $(ARM-TOOLS-PATH)/arm-linux-as
DASM		= $(ARM-TOOLS-PATH)/arm-linux-objdump
CC		= $(ARM-TOOLS-PATH)/arm-linux-gcc
LD		= $(ARM-TOOLS-PATH)/arm-linux-ld
OBJ		= $(ARM-TOOLS-PATH)/arm-linux-objcopy
ASMFLAGS	= 
CFLAGS		= -c
LDFLAGS		= -Ttext $(ENTRYPOINT)
OBJFLAGS	= -O binary -S
DASMFLAGS	= -D -b binary -m arm

# This Program
NANSLKERNEL	= kernel.bin
NANSLKERNELELF	= kernel.elf
OBJS		= $(OBJ-FILES-PATH)/main.o $(OBJ-FILES-PATH)/startup.o $(OBJ-FILES-PATH)/nand_flash.o $(OBJ-FILES-PATH)/lcd.o $(OBJ-FILES-PATH)/exception_handlers.o
DASMOUTPUT	= kernel.bin.asm

# All Phony Targets
.PHONY : everything final clean realclean disasm all

# Default starting position
everything : $(NANSLKERNEL)

all : realclean everything

final : all clean

clean :
	rm -f $(OBJS) $(DASMOUTPUT) $(NANSLKERNELELF)

realclean :
	rm -f $(OBJS) $(DASMOUTPUT) $(NANSLKERNELELF) $(NANSLKERNEL)

disasm :
	$(DASM) $(DASMFLAGS) $(NANSLKERNEL) > $(DASMOUTPUT)

# All object dependencies files
$(NANSLKERNEL) : $(NANSLKERNELELF)
	$(OBJ) $(OBJFLAGS) $< $@

$(NANSLKERNELELF) : $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

$(OBJ-FILES-PATH)/main.o: main.c
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ-FILES-PATH)/startup.o: startup.S
	$(ASM) $(ASMFLAGS) -o $@ $<

$(OBJ-FILES-PATH)/nand_flash.o: $(HARDWARE-PATH)/nand_flash.c
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ-FILES-PATH)/lcd.o: $(HARDWARE-PATH)/lcd.c
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ-FILES-PATH)/exception_handlers.o: $(SYS-FILES-PATH)/exception_handlers.c
	$(CC) $(CFLAGS) -o $@ $<
