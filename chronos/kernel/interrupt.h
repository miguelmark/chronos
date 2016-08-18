#ifndef INTERRUPT_H_INCLUDED
#define INTERRUPT_H_INCLUDED

#include <stdint.h>
#include "int_handlers.h"
#include "pic.h"
#include "idt.h"

/* Notes: irq = Hardware generated interrupts
          isr = Exceptions
*/

typedef struct interrupt_t {
    int32_t gs;
    int32_t fs;
    int32_t es;
    int32_t ds;
    int32_t edi;
    int32_t esi;
    int32_t ebp;
    int32_t esp;
    int32_t ebx;
    int32_t edx;
    int32_t ecx;
    int32_t eax;
    int32_t interrupt_no;
    int32_t error_code;
    int32_t eip;
    int32_t cs;
    int32_t eflags;
    int32_t user_esp;
    int32_t ss;
} interrupt_t;

char* exception_messages[] = {
    "Division by zero exception",
    "Debug Interrupt",
    "Non-Maskable Interrupt Exception",
    "Breakpoint Exception",
    "Into detected overflow exception",
    "Out of Bounds Exception",
    "Invalid Opcode Exception",
    "No Coprocessor Exception",
    "Double Fault Exception",
    "Coprocessor Segment Overrun Exception",
    "Bad TSS Segment Exception",
    "Stack Fault Exception",
    "General Protection Fault Exception",
    "Page Fault Exception",
    "Unknown Interrupt Exception",
    "Coprocessor Fault Exception",
    "Alignment Check Exception",
    "Machine Check Exception",
    "Reserved Exception",
    "Reserved Exception",
    "Reserved Exception",
    "Reserved Exception",
    "Reserved Exception",
    "Reserved Exception",
    "Reserved Exception",
    "Reserved Exception",
    "Reserved Exception",
    "Reserved Exception",
    "Reserved Exception",
    "Reserved Exception"
};

void *isr_routines[48] = {
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0
};

void install_isr_handlers();

void unset_isr_handler(int isr_id);

void set_isr_handler(int isr_id, void (*handler)(interrupt_t*));

void enable_hardware_interrupts();

void disable_hardware_interrupts();
#endif // INTERRUPT_H_INCLUDED
