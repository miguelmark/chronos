#ifndef CLOCK_H_INCLUDED
#define CLOCK_H_INCLUDED
#define CLOCK_FREQUENCY 1193180

void intstall_clock();
/* should be set to a 100 */
void set_clock_phase(unsigned int hz);
#endif // CLOCK_H_INCLUDED
