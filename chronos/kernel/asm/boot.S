# Declare constants used for creating a multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Constants expected by multi-boot
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Set up stack
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:


.section .text
.global _start
.type _start, @function
_start:
	# Set up stack
	movl $stack_top, %esp

        # Call kernel entry point
	call kernel_main

	# Disable interrupts
	cli
	# Halt cpu
	hlt
.Lhang:
	jmp .Lhang

# Stage-1 interrupt handler routines
# These functions act as forwarders
# to the main handler stub
.global do_isr0
.global do_isr1
.global do_isr2
.global do_isr3
.global do_isr4
.global do_isr5
.global do_isr6
.global do_isr7
.global do_isr8
.global do_isr9
.global do_isr10
.global do_isr11
.global do_isr12
.global do_isr13
.global do_isr14
.global do_isr15
.global do_isr16
.global do_isr17
.global do_isr18
.global do_isr19
.global do_isr20
.global do_isr21
.global do_isr22
.global do_isr23
.global do_isr24
.global do_isr25
.global do_isr26
.global do_isr27
.global do_isr28
.global do_isr29
.global do_isr30
.global do_isr31
.global do_isr32
.global do_isr33
.global do_isr34
.global do_isr35
.global do_isr36
.global do_isr37
.global do_isr38
.global do_isr39
.global do_isr40
.global do_isr41
.global do_isr42
.global do_isr43
.global do_isr44
.global do_isr45
.global do_isr46
.global do_isr47

# TODO: finish renaming these functions
do_isr0:
    cli
    push $0 # dummy value for stack uniformity
    push $0 # isr id
    jmp isr_stub
do_isr1:
    cli
    push $0 # dummy value for stack uniformity
    push $1 # isr id
    jmp isr_stub
do_isr2:
    cli
    push $0 # dummy value for stack uniformity
    push $2 # isr id
    jmp isr_stub
do_isr3:
    cli
    push $0 # dummy value for stack uniformity
    push $3 # isr id
    jmp isr_stub
do_isr4:
    cli
    push $0 # dummy value for stack uniformity
    push $4 # isr id
    jmp isr_stub
do_isr5:
    cli
    push $0 # dummy value for stack uniformity
    push $5 # isr id
    jmp isr_stub
do_isr6:
    cli
    push $0 # dummy value for stack uniformity
    push $6 # isr id
    jmp isr_stub
do_isr7:
    cli
    push $0 # dummy value for stack uniformity
    push $7 # isr id
    jmp isr_stub
do_isr8:
    cli
    push $8 # isr id
    jmp isr_stub
do_isr9:
    cli
    push $0 # dummy value for stack uniformity
    push $9 # isr id
    jmp isr_stub
do_isr10:
    cli
    push $10 # isr id
    jmp isr_stub
do_isr11:
    cli
    push $11 # isr id
    jmp isr_stub
do_isr12:
    cli
    push $12 # isr id
    jmp isr_stub
do_isr13:
    cli
    push $13 # isr id
    jmp isr_stub
do_isr14:
    cli
    push $14 # isr id
    jmp isr_stub
do_isr15:
    cli
    push $0 # dummy value for stack uniformity
    push $15 # isr id
    jmp isr_stub
do_isr16:
    cli
    push $0 # dummy value for stack uniformity
    push $16 # isr id
    jmp isr_stub
do_isr17:
    cli
    push $0 # dummy value for stack uniformity
    push $17 # isr id
    jmp isr_stub
do_isr18:
    cli
    push $0 # dummy value for stack uniformity
    push $18 # isr id
    jmp isr_stub
do_isr19:
    cli
    push $0 # dummy value for stack uniformity
    push $19 # isr id
    jmp isr_stub
do_isr20:
    cli
    push $0 # dummy value for stack uniformity
    push $20 # isr id
    jmp isr_stub
do_isr21:
    cli
    push $0 # dummy value for stack uniformity
    push $21 # isr id
    jmp isr_stub
do_isr22:
    cli
    push $0 # dummy value for stack uniformity
    push $22 # isr id
    jmp isr_stub
do_isr23:
    cli
    push $0 # dummy value for stack uniformity
    push $23 # isr id
    jmp isr_stub
do_isr24:
    cli
    push $0 # dummy value for stack uniformity
    push $24 # isr id
    jmp isr_stub
do_isr25:
    cli
    push $0 # dummy value for stack uniformity
    push $25 # isr id
    jmp isr_stub
do_isr26:
    cli
    push $0 # dummy value for stack uniformity
    push $26 # isr id
    jmp isr_stub
do_isr27:
    cli
    push $0 # dummy value for stack uniformity
    push $27 # isr id
    jmp isr_stub
do_isr28:
    cli
    push $0 # dummy value for stack uniformity
    push $28 # isr id
    jmp isr_stub
do_isr29:
    cli
    push $0 # dummy value for stack uniformity
    push $29 # isr id
    jmp isr_stub
do_isr30:
    cli
    push $0 # dummy value for stack uniformity
    push $30 # isr id
    jmp isr_stub
do_isr31:
    cli
    push $0 # dummy value for stack uniformity
    push $31 # isr id
    jmp isr_stub


# isrs here will be mapped to gates 32 --> 47 (0x20 - 0x2F)
# hardware interrupts

do_isr32:
    cli
    push $0
    push $32 # idt gate index
    jmp isr_stub
do_isr33:
    cli
    push $0
    push $33
    jmp isr_stub
do_isr34:
    cli
    push $0
    push $34
    jmp isr_stub
do_isr35:
    cli
    push $0
    push $35
    jmp isr_stub
do_isr36:
    cli
    push $0
    push $36
    jmp isr_stub
do_isr37:
    cli
    push $0
    push $37
    jmp isr_stub
do_isr38:
    cli
    push $0
    push $38
    jmp isr_stub
do_isr39:
    cli
    push $0
    push $39
    jmp isr_stub
do_isr40:
    cli
    push $0
    push $40
    jmp isr_stub
do_isr41:
    cli
    push $0
    push $41
    jmp isr_stub
do_isr42:
    cli
    push $0
    push $42
    jmp isr_stub
do_isr43:
    cli
    push $0
    push $043
    jmp isr_stub
do_isr44:
    cli
    push $0
    push $44
    jmp isr_stub
do_isr45:
    cli
    push $0
    push $45
    jmp isr_stub
do_isr46:
    cli
    push $0
    push $46
    jmp isr_stub
do_isr47:
    cli
    push $0
    push $47
    jmp isr_stub

.extern handle_fault

isr_stub:
   pushal # save processor state
   push %ds # save segment details
   push %es
   push %fs
   push %gs
   mov $0x10, %ax # 0x10 points to the kernel data segment
   mov %ax, %ds
   mov %ax, %es
   mov %ax, %fs
   mov %ax, %gs
   movl %esp, %eax
   pushl %eax
   call handle_interrupt
   pop %eax
   pop %gs
   pop %fs
   pop %es
   pop %ds
   popal
   add $8, %esp # removes the isr id and error code
   iret # pops CS, EIP, EFLAGS, SS, and ESP


.extern idt_p
.global load_idt
.type load_idt, @function
load_idt:
	lidt (idt_p)
	ret

.extern gdt_p
.global flush_gdt
.type flush_gdt, @function
flush_gdt:
	lgdt (gdt_p)
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	jmp $0x08, $gdt_flush_finish
gdt_flush_finish:
	ret
