#include "idt.h"

void set_idt_gate(uint8_t gate, uint32_t base, \
                   uint16_t selector, uint8_t type_attr) {
    idt[gate].base_low = base & 0xFFFF;
    idt[gate].base_high = (base >> 0x10) & 0xFFFF;
    idt[gate].selector = selector;
    idt[gate].type_attr = type_attr;
    idt[gate].zero = 0;
}

void install_idt() {
    idt_p.base = &idt;
    idt_p.limit = (sizeof(idt_entry_t) * 256) - 1;
    load_idt();
}
