#ifndef STDLIB_H_INCLUDED
#define STDLIB_H_INCLUDED

#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif
int chtoi(char c);
int atoi(const char* const);

char* uitoa(unsigned int, int, char* const, bool);
char* itoa(int, int, char* const, bool );
#ifdef __cplusplus
}
#endif

#endif // STDLIB_H_INCLUDED
