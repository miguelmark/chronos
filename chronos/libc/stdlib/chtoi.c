#include <stdlib.h>

int chtoi(char c)
{
    if(c >= 48 && c <= 57)
    {
        return c - 48;
    }
    else if(c >= 65 && c <= 70)
    {
        return c - 65 + 10;
    }
    else if(c >= 97 && c <= 102)
    {
        return c - 97 + 10;
    }
    else
    {
        return -1;
    }
}
