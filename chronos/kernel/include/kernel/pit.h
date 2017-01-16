#ifndef PIT_H_INCLUDED
#define PIT_H_INCLUDED

#define PIT_CLOCK_FREQUENCY 1193180

void intstall_pit();
/* should be set to a 100 */
void pit_set_clock_phase(unsigned int hz);
#endif // CLOCK_H_INCLUDED
