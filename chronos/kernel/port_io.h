#ifndef PORT_IO_H_INCLUDED
#define PORT_IO_H_INCLUDED
#include <stdint.h>
#include <stddef.h>

void write_byte_to_port(uint16_t port, int8_t value);

void write_word_to_port(uint16_t port, int16_t value);

void write_dword_to_port(uint16_t port, int32_t value);

int8_t read_byte_from_port(uint16_t port);

int16_t read_word_from_port(uint16_t port);

int32_t read_dword_from_port(uint16_t port);

#endif // PORT_IO_H_INCLUDED
