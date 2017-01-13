TARGET_ARCH:=i386

DEST_DIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOT_DIR=$(EXEC_PREFIX)/boot
INCLUDE_DIR=$(PREFIX)/include
LIB_DIR?=$(EXEC_PREFIX)/lib

HOST?=i686-elf
CC=$(HOST)-gcc
AR=$(HOST)-ar
# Declaring some options for building
CFLAGS=-O0 -g
CFLAGS:=$(CFLAGS) -std=gnu99 -ffreestanding -fbuiltin -Wall -Wextra
LIBK_CFLAGS:=$(CFLAGS)
CPPFLAGS:=-I$(INCLUDE_DIR) -D__is_libc
LIBK_CPPFLAGS:=$(CPPFLAGS) -D__is_libk
LIBFLAGS:=-nostdlib -lgcc
ASFLAGS:=-gstabs+

C_SRC_FILES=$(shell find . -type f -name "*.c")

LIBK_OBJS=$(C_SRC_FILES:.c=.libk.o)
 
.PHONY: all clean install install-headers install-libs print
.SUFFIXES: .o .c .S .libk.o

BINARY_LIST=\
libk.a

all: $(BINARY_LIST)

libk.a: $(LIBK_OBJS)
	$(AR) rcs $@ $(LIBK_OBJS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

%.libk.o: %.c
	$(CC) -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

install: install-headers install-libs
 
install-headers:
	mkdir -p $(DEST_DIR)$(INCLUDE_DIR)
	cp -rp include/* $(DEST_DIR)$(INCLUDE_DIR)/
 
install-libs: $(BINARY_LIST)
	mkdir -p $(DEST_DIR)$(LIB_DIR)
	cp $(BINARY_LIST) $(DEST_DIR)$(LIB_DIR)

clean:
	find . -name "*.d" -o -name "*.o" -delete

print-%:
	@echo $* = $($*)
