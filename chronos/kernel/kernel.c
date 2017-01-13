#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/tty.h>
#include <stdio.h>
#include <stdlib.h>

const char* KERNEL_NAME = "chronos";

void kernel_main()
{
    /* set up the global descriptor table */
    install_gdt();
    /* set up the interrupt descriptor table */
    install_idt();
    initialize_terminal();
    terminal_disable_cursor();
    printf("* Welcome to the %s kernel\n", KERNEL_NAME);
    printf("* This the first boot ever!\n");
    for(;;) {}
}
