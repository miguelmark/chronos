#include <kernel/tty.h>

char scrollback_buffer[SCROLLBACK_BUFFER_SIZE];

static cursor_t terminal_cursor = {
    .position = {.row = 0, .col = 0},
    .color = WHITE
};
static scrollback_buffer_t kernel_scrollback_buffer = {
    .is_enabled = true,
    .line_count = 1,
    .display_line_start = 1,
    .display_line_end = VGA_SCREEN_HEIGHT,
    .buffer_pointer = 0,
    .buffer_size = SCROLLBACK_BUFFER_SIZE,
    .max_scrollable_rows = SCROLLBACK_BUFFER_SIZE / VGA_SCREEN_HEIGHT,
    .buffer = (char*)&scrollback_buffer
};
static simple_terminal_t kernel_terminal = {
    .text_res_width = VGA_SCREEN_WIDTH,
    .text_res_height = VGA_SCREEN_HEIGHT,
    .text_buffer_size = VGA_SCREEN_SIZE,
    .line_pointer = 1,
    .column_pointer = 1,
    .text_buffer_pointer = 0,
    .back_color = BLACK,
    .fore_color = WHITE,
    .buffer = (short*)VGA_TEXT_BUF_ADDR
};


bool is_control_character(unsigned char ch)
{
    return ch <= 31 || ch == 127;
}

bool is_printable_char(unsigned char ch)
{
    return ch > 31;
}

bool terminal_is_full()
{
    return kernel_terminal.text_buffer_pointer >= kernel_terminal.text_buffer_size;
}

bool scrollback_is_full()
{
    // The minus one accounts for the End of String char '\0'
    return kernel_scrollback_buffer.buffer_pointer >= (kernel_scrollback_buffer.buffer_size - 1);
}

bool scroll_down_is_needed()
{
    return kernel_terminal.line_pointer == VGA_SCREEN_HEIGHT;
}

bool terminal_line_is_full()
{
    return kernel_terminal.column_pointer == VGA_SCREEN_WIDTH;
}

bool screen_coordinate_is_valid(unsigned int row, unsigned int col)
{
    return row > 0 && row <= VGA_SCREEN_HEIGHT && col > 0 && col <= VGA_SCREEN_WIDTH;
}

screen_position_t get_sane_screen_coordinates(unsigned int row, unsigned int col)
{
    // We want all row/col pairs to be non-zero based
    // so this function will come in handy
    screen_position_t pos = {
        .row = row == 0 ? 1 : row,
        .col = col == 0 ? 1 : col
    };
    return pos;
}

unsigned int get_flat_index(const screen_position_t* const pos)
{
    unsigned int row = pos->row == 0 ? 1 : pos->row;
    unsigned int col = pos->col == 0 ? 1 : pos->col;
    return (row - 1) * VGA_SCREEN_WIDTH + (col - 1);
}

void scrollback_scroll_up(unsigned int lines)
{
    if(kernel_scrollback_buffer.is_enabled == false)
    {
        return;
    }
    else if(lines >= kernel_scrollback_buffer.display_line_start)
    {
        unsigned int temp_start = kernel_scrollback_buffer.display_line_start;
        kernel_scrollback_buffer.display_line_end -= temp_start > 1 ? temp_start : 0;
        kernel_scrollback_buffer.display_line_start -= temp_start > 1 ? temp_start : 0;
    }
    else
    {
        kernel_scrollback_buffer.display_line_start -= lines;
        kernel_scrollback_buffer.display_line_end -= lines;
    }

}

void scrollback_scroll_down(unsigned int lines)
{
    if(kernel_scrollback_buffer.is_enabled == false)
    {
        return;
    }
    else if((lines + kernel_scrollback_buffer.display_line_end) > kernel_scrollback_buffer.line_count)
    {
        int diff = kernel_scrollback_buffer.line_count - kernel_scrollback_buffer.display_line_end;
        diff = diff < 0 ? 0 : diff;
        kernel_scrollback_buffer.display_line_start += (unsigned)diff;
        kernel_scrollback_buffer.display_line_end += (unsigned)diff;
    }
    else
    {
        kernel_scrollback_buffer.display_line_start += lines;
        kernel_scrollback_buffer.display_line_end += lines ;
    }

}

void terminal_scroll_up(unsigned int lines)
{
    scrollback_scroll_up(lines);
    clear_terminal();
    display_scrollback();
    terminal_set_cursor_color(terminal_cursor.color);
}

void terminal_scroll_down(unsigned int lines)
{
    scrollback_scroll_down(lines);
    clear_terminal();
    display_scrollback();
    terminal_set_cursor_color(terminal_cursor.color);
}

unsigned int get_vga_buf_index_for_line(unsigned int line_no)
{
    return line_no * kernel_terminal.text_res_height;
}

void append_char_to_vga_memory(short ch)
{
    if(kernel_terminal.text_buffer_pointer < kernel_terminal.text_buffer_size)
    {
        kernel_terminal.buffer[kernel_terminal.text_buffer_pointer] = ch;
        kernel_terminal.column_pointer += 1;
        if(kernel_terminal.column_pointer > VGA_SCREEN_WIDTH)
        {
            kernel_terminal.column_pointer = 1;
            kernel_terminal.line_pointer += 1;
        }
        kernel_terminal.text_buffer_pointer += 1;
    }
}

void append_char_to_scrollback(unsigned char ch)
{
    if(kernel_scrollback_buffer.is_enabled == false || scrollback_is_full())
    {
        return;
    }
    if(ch == '\n' || kernel_scrollback_buffer.line_char_count >= VGA_SCREEN_WIDTH)
    {
        kernel_scrollback_buffer.line_count += 1;
        kernel_scrollback_buffer.line_char_count = 0;
    }
    else
    {
        kernel_scrollback_buffer.line_char_count += 1;
    }
    kernel_scrollback_buffer.buffer[kernel_scrollback_buffer.buffer_pointer] = ch;
    kernel_scrollback_buffer.buffer_pointer += 1;
    kernel_scrollback_buffer.buffer[kernel_scrollback_buffer.buffer_pointer] = '\0';
}

void update_terminal_buffer_pointer(unsigned int row, unsigned int col)
{
    screen_position_t sane_pos = get_sane_screen_coordinates(row, col);
    if(screen_coordinate_is_valid(sane_pos.row, sane_pos.col))
    {
        kernel_terminal.line_pointer = sane_pos.row;
        kernel_terminal.column_pointer = sane_pos.col;
        kernel_terminal.text_buffer_pointer = (sane_pos.row - 1) * VGA_SCREEN_WIDTH + (sane_pos.col - 1);
    }
}

void up_shift_scrollback(unsigned int shift_count)
{
    unsigned int i = 0;
    unsigned int f = shift_count - 1;
    if(kernel_scrollback_buffer.is_enabled == false)
    {
        return;
    }
    while(kernel_scrollback_buffer.buffer[f] != '\0')
    {
        kernel_scrollback_buffer.buffer[i] = kernel_scrollback_buffer.buffer[f];
        i += 1;
        f += 1;
    }
    // lazy deletion is preferred here but its messy
    kernel_scrollback_buffer.buffer[i] = '\0';
    kernel_scrollback_buffer.buffer_pointer = i;
}

void trim_top_lines(unsigned int trim_count)
{
    unsigned int i = 0;
    unsigned int line_count = 0;
    unsigned int col_count = 0;
    char ch = '\0';
    if(kernel_scrollback_buffer.is_enabled == false)
    {
        return;
    }
    while((ch = kernel_scrollback_buffer.buffer[i]) != '\0')
    {
        if(trim_count == line_count)
        {
            break;
        }
        else if(ch == '\n' || col_count >= kernel_terminal.text_res_width)
        {
            line_count += 1;
            col_count = 0;
        }
        else if(is_printable_char(ch))
        {
            col_count += 1;
        }
        i += 1;
    }
    up_shift_scrollback(i + 1);
    if(line_count > 0)
    {
        kernel_scrollback_buffer.line_count -= line_count;
        kernel_scrollback_buffer.display_line_start -= line_count;
        kernel_scrollback_buffer.display_line_end -= line_count;
        kernel_scrollback_buffer.line_char_count = col_count;
    }
}

void display_scrollback()
{
    char raw_char = 0;
    unsigned int display_row = 1;
    unsigned int display_col = 1;
    update_terminal_buffer_pointer(1, 1);
    kernel_scrollback_buffer.is_enabled = false;
    for(unsigned int i = 0; i < kernel_scrollback_buffer.buffer_size; i++)
    {
        raw_char = kernel_scrollback_buffer.buffer[i];
        if(terminal_is_full() || raw_char == '\0' || (raw_char == '\n' && scroll_down_is_needed()))
        {
            break;
        }
        else if(raw_char == '\n')
        {
            if(display_row >= kernel_scrollback_buffer.display_line_start)
            {
                terminal_process_next_char(raw_char);
            }
            display_col = 1;
            display_row += 1;
        }
        else if(display_col > VGA_SCREEN_WIDTH)
        {
            display_col = 1;
            display_row += 1;
            if(display_row >= kernel_scrollback_buffer.display_line_start)
            {
                terminal_process_next_char(raw_char);
            }
        }
        else if(display_row >= kernel_scrollback_buffer.display_line_start)
        {
            terminal_process_next_char(raw_char);
            if(is_printable_char(raw_char))
                display_col += 1;
        }
        else if(is_printable_char(raw_char))
        {
            display_col += 1;
        }
    }
    kernel_scrollback_buffer.is_enabled = true;
}

void clear_scrollback()
{
    if(kernel_scrollback_buffer.is_enabled == false)
    {
        return;
    }
    for(unsigned int c = 0; c < kernel_scrollback_buffer.buffer_size; c++)
    {
        kernel_scrollback_buffer.buffer[c] = '\0';
    }
    kernel_scrollback_buffer.buffer_pointer = 0;
    kernel_scrollback_buffer.display_line_start = 1;
    kernel_scrollback_buffer.display_line_end = VGA_SCREEN_HEIGHT;
    kernel_scrollback_buffer.line_count = 1;
    kernel_scrollback_buffer.line_char_count = 0;
}

void terminal_process_ctrl_char(unsigned char ctrl_char)
{
    if(ctrl_char == '\n')
    {
        if(kernel_terminal.line_pointer < VGA_SCREEN_HEIGHT)
        {
            update_terminal_buffer_pointer(kernel_terminal.line_pointer + 1, 1);
        }
        else
        {
            terminal_scroll_down(1);
        }
    }
}

void terminal_process_next_char(unsigned char ch)
{
    if(is_printable_char(ch))
    {
        if(terminal_is_full())
        {
            // we expect that the char to be already in the
            // scollback buffer!
            terminal_scroll_down(1);
        }
        else
        {
            short vga_encode = \
                get_vga_encoded_char(kernel_terminal.back_color, kernel_terminal.fore_color, ch);
            append_char_to_vga_memory(vga_encode);
        }
    }
    else if(is_control_character(ch))
    {
        terminal_process_ctrl_char(ch);
    }
}

void clear_terminal()
{
    for(unsigned int c = 0; c < kernel_terminal.text_buffer_size; c++)
    {
        kernel_terminal.buffer[c] = get_vga_encoded_char(BLACK, WHITE, '\0');
    }
    update_terminal_buffer_pointer(1, 1);
}

void initialize_terminal()
{
    screen_position_t csr_pos = get_cursor_position_via_regs();
    terminal_cursor.position.row = csr_pos.row;
    terminal_cursor.position.col = csr_pos.col;
    clear_terminal();
    clear_scrollback();
}

int terminal_write_char(int ch)
{
    if(scrollback_is_full())
    {
        trim_top_lines(SCROLLBACK_TRIM_COUNT);
    }
    unsigned char chx = ch;
    append_char_to_scrollback(chx);
    terminal_process_next_char(chx);
    return ch;
}

int terminal_write_string(const char* const str)
{
    int str_index = 0;
    char ch = '\0';
    while((ch = str[str_index]) != '\0')
    {
        terminal_write_char(ch);
        str_index += 1;
    }
    return str_index;
}

int terminal_write_line(const char* const str)
{
    // TODO: Handle potential errors
    int write_count = 0;
    write_count += terminal_write_string(str);
    terminal_write_char('\n');
    return write_count + 1;
}

void terminal_put_char_at(unsigned short ch, unsigned int row, unsigned col)
{
    // to keep things simple the scrollback buffer is not affected
    // but this might change in the future
    // for now this is mainly for testing purposes
    screen_position_t sane_pos = get_sane_screen_coordinates(row, col);
    unsigned int flat_index = get_flat_index(&sane_pos);
    if(screen_coordinate_is_valid(sane_pos.row, sane_pos.col))
    {
        // we don't modify the buffer pointer here...
        unsigned short vga_encode = get_vga_encoded_char
                                    (kernel_terminal.back_color,
                                    kernel_terminal.fore_color,
                                    ch);
        if(is_printable_char(ch))
        {
            kernel_terminal.buffer[flat_index] = vga_encode;
        }
    }
}

void terminal_show_layout()
{
    vga_color_t back_color = BLACK;
    vga_color_t fore_color = WHITE;
    short vga_encode = 0;
    for(int c = 0; c < VGA_SCREEN_SIZE; c++)
    {
        vga_encode = get_vga_encoded_char(' ', back_color, fore_color);
        back_color = c % 16;
        kernel_terminal.buffer[c] = vga_encode;
    }
}

void terminal_set_fore_color(vga_color_t color)
{
    kernel_terminal.fore_color = color;
}

void terminal_set_back_color(vga_color_t color)
{
    kernel_terminal.back_color = color;
}

vga_color_t get_fore_color_at(unsigned int row, unsigned int col)
{
    screen_position_t sane_pos = get_sane_screen_coordinates(row, col);
    if(screen_coordinate_is_valid(sane_pos.row, sane_pos.col))
    {
        unsigned int flat_index = get_flat_index(&sane_pos);
        unsigned short vga_encode = kernel_terminal.buffer[flat_index];
        return get_fore_color(vga_encode);
    }
    else
    {
        return WHITE;
    }
}

vga_color_t get_back_color_at(unsigned int row, unsigned int col)
{
    screen_position_t sane_pos = get_sane_screen_coordinates(row, col);
    if(screen_coordinate_is_valid(sane_pos.row, sane_pos.col))
    {
        unsigned int flat_index = get_flat_index(&sane_pos);
        unsigned short vga_encode = kernel_terminal.buffer[flat_index];
        return get_back_color(vga_encode);
    }
    else
    {
        return BLACK;
    }
}


char terminal_get_raw_char_at(unsigned int row, unsigned int col)
{
    screen_position_t sane_pos = get_sane_screen_coordinates(row, col);
    if(screen_coordinate_is_valid(sane_pos.row, sane_pos.col))
    {
        unsigned flat_index = get_flat_index(&sane_pos);
        return get_raw_char_from_vga_encode(kernel_terminal.buffer[flat_index]);
    }
    else
    {
        return -1;
    }
}

void terminal_set_color_at(unsigned int row, unsigned int col, vga_color_t back_color, vga_color_t fore_color)
{
    screen_position_t sane_pos = get_sane_screen_coordinates(row, col);
    if(screen_coordinate_is_valid(sane_pos.row, sane_pos.col))
    {
        char raw_char = terminal_get_raw_char_at(row, col);
        short vga_encode = 0;
        if(raw_char > -1)
        {
            unsigned flat_index = get_flat_index(&sane_pos);
            vga_encode = get_vga_encoded_char(back_color, fore_color, raw_char);
            kernel_terminal.buffer[flat_index] = vga_encode;
        }
    }
}

// cursor functions

void terminal_set_cursor_color(vga_color_t color)
{
    terminal_set_color_at(
                          terminal_cursor.position.row,
                          terminal_cursor.position.col,
                          kernel_terminal.back_color,
                          color);
    terminal_cursor.color = color;
}

void terminal_move_cursor(unsigned int row, unsigned int col)
{
    screen_position_t sane_pos = get_sane_screen_coordinates(row, col);
    if(screen_coordinate_is_valid(sane_pos.row, sane_pos.col))
    {
        // set the cursor color
        //terminal_set_cursor_color(terminal_cursor.color);
        // move the cursor
        unsigned int flat_index = get_flat_index(&sane_pos);
        char high_byte = (flat_index >> 8) & 0xFF;
        char low_byte = (flat_index) & 0xFF;
        write_byte_to_port(0x3D4, 0xE);
        write_byte_to_port(0x3D5, high_byte);
        write_byte_to_port(0x3D4, 0xF);
        write_byte_to_port(0x3D5, low_byte);
        terminal_cursor.position.col = sane_pos.col;
        terminal_cursor.position.row = sane_pos.row;
    }
}

screen_position_t get_cursor_position_via_regs()
{
    write_byte_to_port(0x3D4, 0xF);
    unsigned char low_byte = read_byte_from_port(0x3D5);
    write_byte_to_port(0x3D4, 0xE);
    unsigned char high_byte = read_byte_from_port(0x3D5);
    unsigned int flat_index = (high_byte << 8) | low_byte;
    unsigned int col = flat_index % VGA_SCREEN_WIDTH;
    unsigned int row = (flat_index - col) / VGA_SCREEN_WIDTH;
    screen_position_t pos = {.row = row + 1, .col = col + 1};
    return pos;
}

unsigned char get_max_cursor_height()
{
    // returns the max number of scan lines - 1
    // this is also the number of scan lines in a character cell
    write_byte_to_port(0x3D4, 0x9);
    unsigned char max_height = read_byte_from_port(0x3D5) & 0x1F;
    return max_height;
}

unsigned int get_cursor_height()
{
    write_byte_to_port(0x3D4, 0xA);
    unsigned char csr_scan_start = read_byte_from_port(0x3D5) & 0x1F;
    write_byte_to_port(0x3D4, 0xB);
    unsigned char csr_scan_end = read_byte_from_port(0x3D5) & 0x1F;
    unsigned int height = csr_scan_end - csr_scan_start;
    return height;
}

void terminal_set_cursor_height(unsigned int scan_lines)
{
    unsigned int max_height = get_max_cursor_height();
    // the cursor grows upward
    write_byte_to_port(0x3D4, 0xB);
    unsigned char new_csr_scan_end = (read_byte_from_port(0x3D5) & 0xE0) | (max_height - 1);
    write_byte_to_port(0x3D5, new_csr_scan_end);
    write_byte_to_port(0x3D4, 0xA);
    unsigned char new_csr_scan_start = (read_byte_from_port(0x3D5) & 0xE0) | (max_height - scan_lines);
    write_byte_to_port(0x3D5, new_csr_scan_start);
}

bool cursor_is_enabled()
{
    write_byte_to_port(0x3D4, 0xA);
    unsigned char cursor_start_register = read_byte_from_port(0x3D5);
    bool is_enabled = ~((cursor_start_register & 0x20) >> 5);
    return is_enabled;
}

void terminal_enable_cursor()
{
    write_byte_to_port(0x3D4, 0xA);
    unsigned char cursor_start_register = read_byte_from_port(0x3D5);
    // zero out cursor disable bit
    unsigned char output = cursor_start_register & 0xD;
    write_byte_to_port(0x3D5, output);
}

void terminal_disable_cursor()
{
    write_byte_to_port(0x3D4, 0xA);
    unsigned char cursor_start_register = read_byte_from_port(0x3D5);
    unsigned char output = cursor_start_register | 0x20;
    write_byte_to_port(0x3D5, output);
}

