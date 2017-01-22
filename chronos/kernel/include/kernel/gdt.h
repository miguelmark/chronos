#ifndef GDT_H_INCLUDED
#define GDT_H_INCLUDED
#include <stddef.h>
#include <stdint.h>

typedef struct gdt_entry_t
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed)) gdt_entry_t;

typedef struct gdt_ptr_t
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) gdt_ptr_t;

extern void flush_gdt();

void set_gdt_gate(int index, uint32_t base, \
                  uint32_t limit, uint8_t access, uint8_t granularity);

void install_gdt();
#endif // GDT_H_INCLUDED
