#ifndef IDT_H_INCLUDED
#define IDT_H_INCLUDED
#include <stdint.h>
#include <stddef.h>

typedef struct idt_entry_t
{
    uint16_t base_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attr;
    uint16_t base_high;
} __attribute__((packed)) idt_entry_t;

typedef struct idt_ptr_t
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr_t;

extern void load_idt();

void set_idt_gate(uint8_t gate, uint32_t base, uint16_t selector, uint8_t type_attr);

void install_idt();
#endif // IDT_H_INCLUDED
