#include "clock.h"

void set_clock_phase(int hz) {
    if(hz <= 0) {
        return;
    }
    int divisor = CLOCK_FREQUENCY / hz;
    write_byte_to_port(0x43, 0x36); /* set command byte */
    write_byte_to_port(0x40, divisor & 0xFF); /* send LSB */
    write_byte_to_port(0x40, divisor >> 0x8); /* send MSB */
}
