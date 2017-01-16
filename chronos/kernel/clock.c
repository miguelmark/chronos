#include <kernel/clock.h>
#include <kernel/interrupt.h>
#include <asm/io.h>
#include <stdio.h>

static unsigned int clock_tick_rate = 18;
static unsigned int clock_ticks = 0;
static unsigned int seconds_counter = 0;

void set_clock_phase(unsigned int hz)
{
    if(hz == 0)
    {
        return;
    }
    int divisor = CLOCK_FREQUENCY / hz;
    write_byte_to_port(0x43, 0x36); /* set command byte */
    write_byte_to_port(0x40, divisor & 0xFF); /* send LSB */
    write_byte_to_port(0x40, divisor >> 0x8); /* send MSB */
    clock_tick_rate = hz;
}

void handle_clock_irq(interrupt_t* interrupt)
{
    clock_ticks += 1;
    if(clock_ticks % clock_tick_rate == 0)
    {
        seconds_counter += 1;
    }
}

void clock_install_irq_handler()
{
    set_irq_handler(CLOCK, handle_clock_irq);
}

void install_clock()
{
    clock_install_irq_handler();
}
