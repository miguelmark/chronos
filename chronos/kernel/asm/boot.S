# Constants for initial memory configuration
# This is a higher-half kernel that starts at 3.5GB mark
# However we want to load it at 1MB in physical memory
.set KERNEL_LOAD_ADDRESS, 0x100000
.set KERNEL_VIRTUAL_BASE, 0xE0000000
.set VIRTUAL_MEMORY_SIZE, 0xFFFFFFFF
.global KERNEL_LOAD_ADDRESS
.global KERNEL_VIRTUAL_BASE
# Constants set by the linker
.extern KERNEL_VIRTUAL_END
.extern KERNEL_SIZE_IN_BYTES
# Kernel entry point
.extern kernel_main

# Declare constants used for creating a multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Constants expected by multi-boot
.section .multiboot
.align 0x4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Set up stack
.section .bss
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

.section .data
.align 0x1000
page_table_1:
    .skip 0x400 * 4, 0x0
page_table_2:
    .skip 0x400 * 4, 0x0
page_directory:
    .long 0x00000083 # dummy 4mb page
    .skip 0x3FF * 4, 0x0

.section .data
.align 0x1000
load_config:
    .long KERNEL_LOAD_ADDRESS
    .long KERNEL_SIZE_IN_BYTES

# constants for table setup
.set TABLE_COUNT, 0x2
.set ENTRY_COUNT, TABLE_COUNT * 1024
.set MAX_PTE_ENTRY_OFFSET, ENTRY_COUNT * 4
.set MAX_PDE_ENTRY_OFFSET, TABLE_COUNT * 4

.section .text
.global start
start:
    movl $0x0, %ecx
    movl $0x1000, %esi
    # start at the first entry
    movl $(page_table_1 - KERNEL_VIRTUAL_BASE), %edi
    # start mapping from address 0
    movl $0x0, %eax
    # eax = frame number
    div %esi
fill_page_tables:
    cmpl $MAX_PTE_ENTRY_OFFSET, %ecx
    jge setup_page_directory

    # create PTE in esi
    movl %eax, %esi
    shl $0xC, %esi
    orl $0x3, %esi
    # move PTE to table
    movl %esi, (%ecx,%edi,)

    # prepare next iteration
    addl $0x4, %ecx
    # point to next frame
    inc %eax
    jmp fill_page_tables
setup_page_directory:
    # ecx = page_directory index * 4
    movl $KERNEL_VIRTUAL_BASE, %eax
    shr $0x16, %eax
    movl $0x4, %esi
    mul %esi
    movl %eax, %ecx

    # esi = 4096 = size of frame
    movl $0x1000, %esi
    # point to address of first table
    movl $(page_table_1 - KERNEL_VIRTUAL_BASE), %eax
    # eax = frame number of first table
    div %esi
    # edi = first entry for kernel
    movl $(page_directory - KERNEL_VIRTUAL_BASE), %esi
    leal (%ecx,%esi,), %edi
    # ecx = loop counter
    movl $0x0, %ecx
fill_page_directory:
    cmpl $MAX_PDE_ENTRY_OFFSET, %ecx
    jge enable_paging
    # create PDE in esi
    movl %eax, %esi
    shl $0xC, %esi
    orl $0x3, %esi

     # move PDE to table
    movl %esi, (%ecx,%edi,)
    # prepare next iteration
    addl $0x4, %ecx
    # point to next table frame
    inc %eax
    jmp fill_page_directory
enable_paging:
    # store page directory in cr3
    movl $(page_directory - KERNEL_VIRTUAL_BASE), %eax
    movl %eax, %cr3

    # Enable 4mb pages
    movl %cr4, %eax
    orl $0x00000010, %eax
    movl %eax, %cr4

    # Enable paging
    movl %cr0, %eax
    orl $0x80000000, %eax
    movl %eax, %cr0
jump_to_higher_half:
    # Long jump to the higher half to start linear address translation
    movl $start_higher_half, %eax
    jmp *%eax
start_higher_half:
    # Remove the first 4mb dummy mapping
    movl $0x0, (page_directory)
    # Invalidate TLB entry for first 4mb page
    invlpg (0)
    # we are using linear address translation from now on
    # Set up stack
    movl $stack_top, %esp
    call kernel_main
    # Disable interrupts
    cli
    # Halt cpu
    hlt
.Lhang:
	jmp .Lhang

