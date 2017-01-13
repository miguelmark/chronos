#include <stdlib.h>

char* uitoa(unsigned int integer, int base, char* const buffer, bool use_upper_case)
{
    int buff_len = 0;
    int temp_buff_len = 0;
    unsigned int dividend = integer;
    char temp_buffer[32];
    int alpha_base = use_upper_case ? 65 : 97;
    unsigned int remainder = 0;
    while (dividend > 0)
    {
        remainder = dividend % base;
        dividend /= base;
        if (remainder >= 10)
        {
            temp_buffer[temp_buff_len] = (remainder - 10) + alpha_base;
        }
        else
        {
            temp_buffer[temp_buff_len] = remainder + 48;
        }
        temp_buff_len += 1;
    }
    for(int c = temp_buff_len -1; c >= 0; c--, buff_len++)
    {
        buffer[buff_len] = temp_buffer[c];
    }
    buffer[buff_len] = '\0';
    return buffer;
}
