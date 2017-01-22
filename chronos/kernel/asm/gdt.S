.section .text
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
