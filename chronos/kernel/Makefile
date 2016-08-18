TARGETARCH:=i386
PROJECTNAME=kernel
PROJECTDIR:=.
INCLUDEDIRS:=$(shell find $(PROJECTDIR) -type d -name "include")
LINKERSCRIPT:=linker.ld
CSRCFILES:=$(shell find $(PROJECTDIRS) -type f -name "*.c")
ASMSRCFILES:=$(shell find $(PROJECTDIRS) -type f -name "*.S" ! -name "*crt*.S")
HDRFILES:=$(shell find $(PROJECTDIRS) -type f -name "*.h")

OBJS=$(CSRCFILES:%.c=%.o)
OBJS += $(ASMSRCFILES:%.S=%.o)

INCLUDESTRING:=$(INCLUDEDIRS:%=-I%)

# Cross compiler/assembler
CC:=i686-elf-gcc
AS:=i686-elf-as
# Declaring some options for building
CFLAGS?=-O2 -g
CFLAGS:=$(CFLAGS) -std=gnu99 -ffreestanding -fbuiltin -Wall -Wextra
CPPFLAGS:=$(INCLUDESTRING) -D_kernel_code
LIBFLAGS:=-nostdlib -lgcc
ASFLAGS:=-gstabs+

ALLOBJS:=\
$(OBJS)

IMAGENAME = chronos_$(TARGETARCH).elf

all: $(IMAGENAME)

$(IMAGENAME): $(ALLOBJS) $(LINKERSCRIPT)
	$(CC) -T $(LINKERSCRIPT) -o $@ $(CFLAGS) $(ALLOBJS) $(LIBFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

%.o: %.s
	$(AS) $< -o $@ $(ASFLAGS)

print-%:
	@echo $* = $($*)

clean:
	find . -type f -name "*.o" -delete
	find . -type f -name "*.d" -delete
