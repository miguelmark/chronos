TARGET_ARCH?=i686

DEST_DIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOT_DIR=$(EXEC_PREFIX)/boot
INCLUDE_DIR=$(PREFIX)/include
LIB_DIR=$(PREFIX)/lib
ASM_DIR?=asm

HOST?=${TARGET_ARCH}-elf
CC=$(HOST)-gcc
AS=$(HOST)-as
# Declaring some options for building
CFLAGS=-O0 -g
CFLAGS:=$(CFLAGS) -std=gnu99 -ffreestanding -fbuiltin -Wall -Wextra
CPPFLAGS:=-D_kernel_code -I$(INCLUDE_DIR)
LIBFLAGS:=-nostdlib -lgcc -L$(LIB_DIR) -lk
ASFLAGS:=-gstabs+

C_SRC_FILES=$(shell find . -type f -name "*.c")
ASM_SRC_FILES=$(shell find . -type f -name "*.S")
KERNEL_OBJS?=$(ASM_SRC_FILES:.S=.S.o)
KERNEL_OBJS+=$(C_SRC_FILES:.c=.o)

ALL_OBJS=\
$(ASM_DIR)/crti.o \
$(ASM_DIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(ASM_DIR)/crtend.o \
$(ASM_DIR)/crtn.o

IMAGE_NAME = chronos_$(TARGET_ARCH).elf

.PHONY: all clean print install-headers install-kernel install
.SUFFIXES: .o .S .c .S.o

.SECONDARY:

all: $(IMAGE_NAME)

$(IMAGE_NAME): linker.ld $(ALL_OBJS)
	$(CC) -T linker.ld -o $@ $(CFLAGS) $(ALL_OBJS) $(LIBFLAGS)

$(ASM_DIR)/crtbegin.o $(ASM_DIR)/crtend.o:
	OBJ=`$(CC) $(CFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

%.o: %.c
	$(CC) -c $< -o  $@ $(CFLAGS) $(CPPFLAGS)

%.o: %.S
	$(AS) $< -o $@ $(ASFLAGS)

%.S.o: %.s
	$(AS) $< -o $@ $(ASFLAGS)

install: install-headers install-kernel

install-kernel:
	mkdir -p $(DEST_DIR)$(BOOT_DIR)
	cp $(IMAGE_NAME) $(DEST_DIR)$(BOOT_DIR)

install-headers:
	mkdir -p $(DEST_DIR)$(INCLUDE_DIR)
	cp -rp include/* $(DEST_DIR)$(INCLUDE_DIR)

clean:
	find . -name "*.d" -o -name "*.o" -delete

print-%:
	@echo $* = $($*)
