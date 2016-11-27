#include "gdt.h"
#include "idt.h"
void kernel_main()
{
    /* set up the global descriptor table */
    install_gdt();
    /* set up the interrupt descriptor table */
    install_idt();
    for(;;) {}
}
