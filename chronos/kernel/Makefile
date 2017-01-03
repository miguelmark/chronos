TARGET_ARCH:=i386
PROJECT_NAME:=kernel
PROJECT_DIR=
LINKER_SCRIPT:=linker.ld
INCLUDE_DIRS=$(shell find $(PROJECT_DIRS) -type d -name "include")
C_SRC_FILES=$(shell find $(PROJECT_DIRS) -type f -name "*.c")
C_SRC_FILES_NO_DIR=$(shell find $(PROJECT_DIRS) -type f -name "*.c" -exec basename {} \;)
ASM_SRC_FILES=$(shell find $(PROJECT_DIRS) -type f -name "*.S" ! -name "*crt*.S")
ASM_SRC_FILES_NO_DIR=$(shell find $(PROJECT_DIRS) \
                           -type f -name "*.S" ! -name "*crt*.S" -exec basename {} \;)
HDR_FILES=$(shell find $(PROJECT_DIRS) -type f -name "*.h")

OBJECT_DIR:=bin
OBJS=$(C_SRC_FILES:./%.c=%.o)
OBJS += $(ASM_SRC_FILES:./%.S=%.o)
BIN_OBJS=$(C_SRC_FILES_NO_DIR:%.c=$(OBJECT_DIR)/%.o)
BIN_OBJS += $(ASM_SRC_FILES_NO_DIR:%.S=$(OBJECT_DIR)/%.o)
ALL_OBJS=$(OBJS)

INCLUDE_STRING:=$(INCLUDE_DIRS:%=-I%)

# Cross compiler/assembler
CC:=i686-elf-gcc
AS:=i686-elf-as
# Declaring some options for building
CFLAGS?=-O0 -g
CFLAGS:=$(CFLAGS) -std=gnu99 -ffreestanding -fbuiltin -Wall -Wextra
CPPFLAGS:=$(INCLUDE_STRING) -D_kernel_code
LIBFLAGS:=-nostdlib -lgcc
ASFLAGS:=-gstabs+

IMAGE_NAME = chronos_$(TARGET_ARCH).elf

all: $(IMAGE_NAME)

$(IMAGE_NAME): $(LINKER_SCRIPT) $(OBJECT_DIR) $(ALL_OBJS)
	$(CC) -T $(LINKER_SCRIPT) -o $(OBJECT_DIR)/$@ $(CFLAGS) $(BIN_OBJS) $(LIBFLAGS)

%.o: %.c $(OBJECT_DIR)
	$(CC) -c $< -o $(OBJECT_DIR)/$(notdir $@) $(CFLAGS) $(CPPFLAGS)

%.o: %.S $(OBJECT_DIR)
	$(AS) $< -o $(OBJECT_DIR)/$(notdir $@) $(ASFLAGS)

$(OBJECT_DIR):
	mkdir -p $(OBJECT_DIR)

print-%:
	@echo $* = $($*)

clean:
	find . -type f -name "*.o" -delete
	find . -type f -name "*.d" -delete
