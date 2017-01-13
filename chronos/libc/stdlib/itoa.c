#include <stdlib.h>

char* get_binary_string(int num, char* const buffer)
{
    bool start_adding = false;
    unsigned int bit_window = 0x80000000;
    int bit_window_index = 31;
    int buf_index = 0;
    int bit = 0;
    while(bit_window > 0)
    {
        bit = (num & bit_window) >> bit_window_index;
        start_adding |= bit;
        if(start_adding)
        {
            buffer[buf_index] = bit + 48;
            buf_index += 1;
        }
        bit_window >>= 1;
        bit_window_index -= 1;
    }
    buffer[buf_index] = '\0';
    return buffer;
}

char* get_non_binary_string(int integer, int base, char* const buffer, bool use_upper_case)
{
    int buff_len = 0;
    int temp_buff_len = 0;
    int dividend = integer;
    char temp_buffer[32];
    int alpha_base = use_upper_case ? 65 : 97;
    if (integer < 0)
    {
        dividend *= -1;
        buffer[0] = '-';
        buff_len += 1;
    }
    int remainder = 0;
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

char* itoa(int integer, int base, char* const buffer, bool use_upper_case)
{
    if(base == 2)
    {
        return get_binary_string(integer, buffer);
    }
    else
    {
        return get_non_binary_string(integer, base, buffer, use_upper_case);
    }
}
