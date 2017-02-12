/* Entry point */
ENTRY(start)

SECTIONS
{
        /* 
        The following constants are defined in boot.S.
        This setup is ideally for a higher half kernel setup.
        KERNEL_LOAD_ADDRESS --> Where the kernel should be loaded in memory
        KERNEL_VIRTUAL_BASE  --> Where the kernel should think it is in memory when paging is on
        */        
	. = KERNEL_VIRTUAL_BASE + KERNEL_LOAD_ADDRESS;

	/* Multiboot section. */
	.text ALIGN(4K) : AT(ADDR(.text)-KERNEL_VIRTUAL_BASE)
	{
		*(.multiboot)
		*(.text)
	}

	/* Read-only data. */
	.rodata ALIGN(4K) : AT(ADDR(.rodata)-KERNEL_VIRTUAL_BASE)
	{
		*(.rodata)
	}

	/* Read-write data (initialized) */
	.data ALIGN(4K) : AT(ADDR(.data)-KERNEL_VIRTUAL_BASE)
	{
		*(.data)
	}

	/* Read-write data (uninitialized) and stack */
	.bss ALIGN(4K) : AT(ADDR(.bss)-KERNEL_VIRTUAL_BASE)
	{
		*(COMMON)
		*(.bss)
	}
        KERNEL_VIRTUAL_END = .;
        KERNEL_SIZE_IN_BYTES = KERNEL_VIRTUAL_END - KERNEL_VIRTUAL_BASE;

}
