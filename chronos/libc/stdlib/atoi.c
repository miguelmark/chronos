#include <string.h>
#include <stdlib.h>

int atoi(const char* const str)
{
    int number = 0;
    int temp_number = 0;
    int digit_index = 0;
    int str_index = 0;
    char chr;
    bool is_negative = false;
    while((chr = str[str_index++]) != '\0')
    {
        if(chr == '-' && digit_index == 0)
        {
            is_negative = true;
        }
        else if(is_digit_char(chr))
        {
            temp_number = chtoi(chr);
            number = digit_index == 0 ? temp_number : number * 10 + temp_number;
            digit_index += 1;
        }
        else
        {
            break;
        }
    }
    if(is_negative)
    {
        return number * -1;
    }
    else
    {
        return number;
    }
}
