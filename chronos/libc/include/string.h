#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif
bool is_digit_char(char c);
bool is_number_char(char c);
int strlen(const char* const str);
#ifdef __cplusplus
}
#endif

#endif // STRING_H_INCLUDED
