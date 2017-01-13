#include <stdio.h>

#if defined(__is_libk)
#include <kernel/tty.h>
#endif
int putchar(int ch)
{
    #if defined(__is_libk)
    //  TODO: add kernel-space code
    return terminal_write_char(ch);
    #else
    // TODO: add user-space code
    #endif
}
