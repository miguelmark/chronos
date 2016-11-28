#include <kernel/gdt.h>

void set_gdt_gate(int index,  uint32_t base, \
                  uint32_t limit, uint8_t access, uint8_t granularity)
{
    // encode the descriptor base address
    gdt[index].base_low = (base & 0xFFFF);
    gdt[index].base_middle = (base >> 16) & 0xFF;
    gdt[index].base_high = (base >> 24) & 0xFF;
    // encode the descriptor limits
    gdt[index].limit_low = (limit & 0xFFFF);
    gdt[index].granularity = (limit >> 16) & 0x0F;
    // encode the granularity and access flags
    gdt[index].granularity |= (granularity & 0xF0);
    gdt[index].access = access;
}

void install_gdt()
{
    // set up pointer to gdt
    gdt_p.limit = (sizeof(gdt_entry_t) * 3) - 1;
    gdt_p.base = &gdt;

    // set null descriptor
    set_gdt_gate(0, 0, 0, 0, 0);

    //set up the code segment descriptor
    // It has a limit of 4GB, granularity of 4MB
    set_gdt_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    // Set up the data segment
    // similar to code segment but type is different
    set_gdt_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // reload gdt by flushing the old one and reloading segment registers
    flush_gdt();

}
