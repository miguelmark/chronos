#ifndef STDIO_H_INCLUDED
#define STDIO_H_INCLUDED

#include <stdarg.h>

#define EOF (-1)

#ifdef __cplusplus
extern "C" {
#endif

int printf(const char* const, ...);
int puts(const char* const);
int putchar(int);

#ifdef __cplusplus
}
#endif
#endif // STDIO_H_INCLUDED
