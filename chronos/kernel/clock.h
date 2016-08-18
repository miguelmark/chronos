#ifndef CLOCK_H_INCLUDED
#define CLOCK_H_INCLUDED

#include <stdint.h>
#include "port_io.h"

uint32_t CLOCK_DEFAULT_TICK_RATE = 1822;
uint32_t CLOCK_FREQUENCY = 1193180; /* hz */

/* should be set to a 100 */
void set_clock_phase(int hz);


#endif // CLOCK_H_INCLUDED
