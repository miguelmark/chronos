#ifndef TTY_H_INCLUDED
#define TTY_H_INCLUDED

#include <stdbool.h>
#include <kernel/vga.h>

#define SCROLLBACK_BUFFER_SIZE VGA_SCREEN_SIZE * 4 + 1
#define SCROLLBACK_TRIM_COUNT 10

typedef struct screen_position_t
{
    unsigned int row;
    unsigned int col;
} screen_position_t;

typedef struct cursor_t
{
    // the position is mainly for debugging purposes
    // in reality its pointless because this value may
    // not reflect whats stored in the vga registers
    screen_position_t position;
    vga_color_t color;
} cursor_t;

typedef struct scrollback_buffer_t
{
    bool is_enabled;
    unsigned int line_char_count;  // total char count for the current line
    unsigned int line_count; // the total number of scrollable lines
    unsigned int display_line_start; // the first line displayed to the screen
    unsigned int display_line_end; // the second line displayed to the screen
    unsigned int buffer_pointer; // the index of the next char that will be written
    unsigned int buffer_size;
    unsigned int max_scrollable_rows;
    char* buffer;
} scrollback_buffer_t;

typedef struct simple_terminal_t
{
    unsigned int text_res_width;
    unsigned int text_res_height;
    unsigned int text_buffer_size;
    unsigned int text_buffer_pointer;
    unsigned int line_pointer;
    unsigned int column_pointer;
    vga_color_t back_color;
    vga_color_t fore_color;
    short* buffer;  // this points to the VGA text mode memory
} simple_terminal_t;

void display_scrollback();

void clear_scrollback();

void clear_terminal();

void initialize_terminal();

bool can_scroll_up();

bool can_scroll_down();

void terminal_scroll_up(unsigned int lines);

void terminal_scroll_down(unsigned int lines);

int terminal_write_char(int ch);

void terminal_process_next_char(unsigned char ch);

int terminal_write_string(const char* const str);

int terminal_write_line(const char* const str);

void terminal_show_layout();

void terminal_set_fore_color(vga_color_t color);

void terminal_set_back_color(vga_color_t color);

void terminal_set_color_at(unsigned int row, unsigned int col, vga_color_t back_color, vga_color_t fore_color);

void terminal_put_char_at(unsigned short ch, unsigned int row, unsigned col);

void terminal_move_cursor(unsigned int x, unsigned int y);

bool cursor_is_enabled();

unsigned int get_cursor_height();

unsigned char get_max_cursor_height();

void terminal_enable_cursor();

void terminal_disable_cursor();

void terminal_set_cursor_color(vga_color_t color);

void terminal_set_cursor_height(unsigned int scan_lines);

screen_position_t get_cursor_position_via_regs();

#endif // TTY_H_INCLUDED
