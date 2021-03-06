#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

#include <stdint.h>

static inline void write_byte_to_port(uint16_t port, uint8_t value)
{
    asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline void write_word_to_port(uint16_t port, uint16_t value)
{
    asm volatile("outw %0, %1" : : "a"(value), "Nd"(port));
}

static inline void write_dword_to_port(uint16_t port, uint32_t value)
{
    asm volatile("outl %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t read_byte_from_port(uint16_t port)
{
    int8_t value = 0;
    asm volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static inline uint16_t read_word_from_port(uint16_t port)
{
    int16_t value = 0;
    asm volatile("inw %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static inline uint32_t read_dword_from_port(uint16_t port)
{
    int32_t value = 0;
    asm volatile("inl %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

#endif // IO_H_INCLUDED
