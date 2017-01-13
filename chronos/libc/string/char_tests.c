#include <string.h>

bool is_digit_char(char c)
{
    return (c >= 48 && c <= 57) || (c >= 65 && c <= 70) || (c >= 95 && c <= 102);
}

bool is_number_char(char c)
{
    return c >= 48 && c <= 57;
}
