#include <kernel/pit.h>
#include <kernel/interrupt.h>
#include <asm/io.h>
#include <stdio.h>

static unsigned int pit_tick_rate = 18;
static unsigned int pit_clock_ticks = 0;

void pit_set_clock_phase(unsigned int hz)
{
    if(hz == 0)
    {
        return;
    }
    int divisor = PIT_CLOCK_FREQUENCY / hz;
    write_byte_to_port(0x43, 0x36); /* set command byte */
    write_byte_to_port(0x40, divisor & 0xFF); /* send LSB */
    write_byte_to_port(0x40, divisor >> 0x8); /* send MSB */
    pit_tick_rate = hz;
}

void handle_pit_irq(interrupt_t* interrupt)
{
    pit_clock_ticks += 1;
}

void pit_install_irq_handler()
{
    set_irq_handler(PROGRAMMABLE_INTERRUPT_TIMER, handle_pit_irq);
}

void install_pit()
{
    pit_install_irq_handler();
}
