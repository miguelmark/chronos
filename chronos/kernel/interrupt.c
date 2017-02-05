#include <kernel/pic.h>
#include <kernel/idt.h>
#include <kernel/isr_decl.h>
#include <kernel/interrupt.h>

void *isr_routines[48] =
{
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

void set_isr_handler(int isr_id, void (*handler)(interrupt_t*))
{
    isr_routines[isr_id] = handler;
}

void unset_isr_handler(int isr_id)
{
    isr_routines[isr_id] = 0;
}

void unset_isq_handler(irq_id_t id)
{
    unset_isr_handler(id);
}

void unset_exception_handler(prot_excep_t id)
{
    unset_isr_handler(id);
}

void set_irq_handler(irq_id_t id, void (*handler)(interrupt_t*))
{
    set_isr_handler(id, handler);
}

void set_exception_handler(prot_excep_t id, void (*handler)(interrupt_t*))
{
    set_isr_handler(id, handler);
}

void initialize_interrupts()
{
    install_isr_handlers();
    enable_hardware_interrupts();
}

void enable_hardware_interrupts()
{
    asm volatile("sti");
}

void disable_hardware_interrupts()
{
    asm volatile("cli");
}

void install_isr_handlers()
{
    /*
    By default isrs 0-7 are mapped to IDT entries 8-15.
    However these are reserved for exceptions by Intel.
    Since IDT entries 0-31 are takes, we have to map
    isr 0-15 to IDT entries 32-47
    */
    set_idt_gate(0, (unsigned)do_isr0, 0x08, 0x8E);
    set_idt_gate(1, (unsigned)do_isr1, 0x08, 0x8E);
    set_idt_gate(2, (unsigned)do_isr2, 0x08, 0x8E);
    set_idt_gate(3, (unsigned)do_isr3, 0x08, 0x8E);
    set_idt_gate(4, (unsigned)do_isr4, 0x08, 0x8E);
    set_idt_gate(5, (unsigned)do_isr5, 0x08, 0x8E);
    set_idt_gate(6, (unsigned)do_isr6, 0x08, 0x8E);
    set_idt_gate(7, (unsigned)do_isr7, 0x08, 0x8E);
    set_idt_gate(8, (unsigned)do_isr8, 0x08, 0x8E);
    set_idt_gate(9, (unsigned)do_isr9, 0x08, 0x8E);
    set_idt_gate(10, (unsigned)do_isr10, 0x08, 0x8E);
    set_idt_gate(11, (unsigned)do_isr11, 0x08, 0x8E);
    set_idt_gate(12, (unsigned)do_isr12, 0x08, 0x8E);
    set_idt_gate(13, (unsigned)do_isr13, 0x08, 0x8E);
    set_idt_gate(14, (unsigned)do_isr14, 0x08, 0x8E);
    set_idt_gate(15, (unsigned)do_isr15, 0x08, 0x8E);
    set_idt_gate(16, (unsigned)do_isr16, 0x08, 0x8E);
    set_idt_gate(17, (unsigned)do_isr17, 0x08, 0x8E);
    set_idt_gate(18, (unsigned)do_isr18, 0x08, 0x8E);
    set_idt_gate(19, (unsigned)do_isr19, 0x08, 0x8E);
    set_idt_gate(20, (unsigned)do_isr20, 0x08, 0x8E);
    set_idt_gate(21, (unsigned)do_isr21, 0x08, 0x8E);
    set_idt_gate(22, (unsigned)do_isr22, 0x08, 0x8E);
    set_idt_gate(23, (unsigned)do_isr23, 0x08, 0x8E);
    set_idt_gate(24, (unsigned)do_isr24, 0x08, 0x8E);
    set_idt_gate(25, (unsigned)do_isr25, 0x08, 0x8E);
    set_idt_gate(26, (unsigned)do_isr26, 0x08, 0x8E);
    set_idt_gate(28, (unsigned)do_isr28, 0x08, 0x8E);
    set_idt_gate(29, (unsigned)do_isr29, 0x08, 0x8E);
    set_idt_gate(30, (unsigned)do_isr30, 0x08, 0x8E);
    set_idt_gate(31, (unsigned)do_isr31, 0x08, 0x8E);
    /* install hardware isr handlers */
    remap_pic(0x20, 0x28);
    set_idt_gate(0x20, (uint32_t)do_isr32, 0x08, 0x8E);
    set_idt_gate(0x21, (uint32_t)do_isr33, 0x08, 0x8E);
    set_idt_gate(0x22, (uint32_t)do_isr34, 0x08, 0x8E);
    set_idt_gate(0x23, (uint32_t)do_isr35, 0x08, 0x8E);
    set_idt_gate(0x24, (uint32_t)do_isr36, 0x08, 0x8E);
    set_idt_gate(0x25, (uint32_t)do_isr37, 0x08, 0x8E);
    set_idt_gate(0x26, (uint32_t)do_isr38, 0x08, 0x8E);
    set_idt_gate(0x27, (uint32_t)do_isr39, 0x08, 0x8E);
    set_idt_gate(0x28, (uint32_t)do_isr40, 0x08, 0x8E);
    set_idt_gate(0x29, (uint32_t)do_isr41, 0x08, 0x8E);
    set_idt_gate(0x2A, (uint32_t)do_isr42, 0x08, 0x8E);
    set_idt_gate(0x2B, (uint32_t)do_isr43, 0x08, 0x8E);
    set_idt_gate(0x2C, (uint32_t)do_isr44, 0x08, 0x8E);
    set_idt_gate(0x2D, (uint32_t)do_isr45, 0x08, 0x8E);
    set_idt_gate(0x2E, (uint32_t)do_isr46, 0x08, 0x8E);
    set_idt_gate(0x2F, (uint32_t)do_isr47, 0x08, 0x8E);

}

void handle_interrupt(interrupt_t* intp)
{
    int int_no = intp->interrupt_no;
    if(int_no < 48 && int_no >= 0)
    {
        void (*handler_func)(interrupt_t* i) = \
            handler_func = isr_routines[intp->interrupt_no];
        if(handler_func > 0)
        {
            handler_func(intp);
        }
        if(int_no < 32)
        {
            // halt/panic
            for(;;) {}
        }
    }
    // isr8 - isr15 are mapped to IDT entries
    // are napped to IDT entries 40 - 48
    if(intp->interrupt_no >= 0x28)
    {
        // send EOI (End of interrupt to slave PIC
        pic_signal_eoi_to_slave();
    }
    // We always send an EOI to the master PIC
    pic_signal_eoi_to_master();
}
