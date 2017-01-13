#include <string.h>

int strlen(const char* const str)
{
    int count = 0;
    while(str[count] != '\0')
    {
        count += 1;
    }
    return count;
}
