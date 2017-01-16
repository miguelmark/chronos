#ifndef INTERRUPT_H_INCLUDED
#define INTERRUPT_H_INCLUDED

#include <stdint.h>
#include <kernel/int_handlers.h>
#include <kernel/pic.h>
#include <kernel/idt.h>


enum processor_exception {
    DIVISION_BY_ZERO = 0,
    DEBUG_INTERRUPT = 1,
    NON_MASKABLE_INTERRUPT = 2,
    BREAKPOINT = 3,
    OVERFLOW = 4,
    OUT_OF_BOUNDS = 5,
    INVALID_OPCODE = 6,
    NO_COPROCESSOR = 7,
    DOUBLE_FAULT = 8,
    COPROCESSOR_SEGMENT_OVERRUN = 9,
    INVALID_TSS_SEGMENT = 10,
    SEGMENT_NOT_PRESENT = 11,
    STACK_EXCEPTION = 12,
    GENERAL_PROTECTION_VIOLATION = 13,
    PAGE_FAULT = 14,
    COPROCESSOR_ERROR = 16
};

enum irq_id {
    CLOCK = 32
};

typedef enum processor_exception proc_exception_t;
typedef enum irq_id irq_id_t;

typedef struct interrupt_t
{
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

void install_isr_handlers();

void unset_isq_handler(irq_id_t id);

void unset_exception_handler(proc_exception_t id);

void set_irq_handler(irq_id_t id, void (*handler)(interrupt_t*));

void set_exception_handler(proc_exception_t id, void (*handler)(interrupt_t*));

void initialize_interrupts();

void enable_hardware_interrupts();

void disable_hardware_interrupts();
#endif // INTERRUPT_H_INCLUDED
