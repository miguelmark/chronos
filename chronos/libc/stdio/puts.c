#include <stdio.h>
#if defined(__is_libk)
#include <kernel/tty.h>
#endif // defined

int puts(const char* const str)
{
    #if defined(__is_libk)
    return terminal_write_string(str);
    #endif // defined
}
