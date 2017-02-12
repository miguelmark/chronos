#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/tty.h>
#include <kernel/pit.h>
#include <kernel/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

void kernel_main()
{
    /* set up the global descriptor table */
    install_gdt();
    /* set up the interrupt descriptor table */
    install_idt();
    initialize_terminal();
    terminal_disable_cursor();
    install_pit();
    initialize_interrupts();
    printf("* Welcome to the %s kernel\n", "chronos");
    printf("* This the first boot ever!\n");
    for(;;) {}
}
