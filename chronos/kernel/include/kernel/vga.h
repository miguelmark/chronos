#ifndef VGA_H_INCLUDED
#define VGA_H_INCLUDED

// 80x25 screen mode
#define VGA_SCREEN_WIDTH 80
#define VGA_SCREEN_HEIGHT 25
#define VGA_SCREEN_SIZE VGA_SCREEN_HEIGHT * VGA_SCREEN_WIDTH
#define VGA_TEXT_BUF_ADDR 0xB8000
enum vga_color {
    BLACK = 0x0,
    BLUE = 0x1,
    GREEN = 0x2,
    CYAN = 0x3,
    RED = 0x4,
    MAGENTA = 0x5,
    BROWN = 0x6,
    LIGHT_GREY = 0x7,
    DARK_GREY = 0x8,
    LIGHT_BLUE = 0x9,
    LIGHT_GREEN = 0xA,
    LIGHT_CYAN = 0xB,
    LIGHT_RED = 0xC,
    LIGHT_MAGENTA = 0xD,
    LIGHT_BROWN = 0xE,
    WHITE = 0xF
};

typedef enum vga_color vga_color_t;

static inline short get_vga_encoded_char(vga_color_t back_color, vga_color_t fore_color, unsigned char ch)
{
    return back_color << 12 | fore_color << 8 | ch;
}

static inline unsigned char get_raw_char_from_vga_encode(unsigned short ch)
{
    return ch & 0xFF;
}

static inline vga_color_t get_fore_color(unsigned short ch)
{
    return ch >> 8 & 0x0F;
}

static inline vga_color_t get_back_color(unsigned short ch)
{
    return ch >> 12 & 0x0F;
}



#endif // VGA_H_INCLUDED
