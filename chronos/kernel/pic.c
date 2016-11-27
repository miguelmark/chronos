#include "pic.h"

void remap_pic(uint16_t offset1, uint16_t offset2)
{
    // save the interrupt mask registers
    uint8_t imr_mask1 = read_byte_from_port(PIC1_DATA);
    uint8_t imr_mask2 = read_byte_from_port(PIC2_DATA);
    // tell the controllers to start the init process
    write_byte_to_port(PIC1_COMMAND, ICW1_INIT + ICW1_ICW4);
    write_byte_to_port(PIC2_COMMAND, ICW1_INIT + ICW1_ICW4);
    // set the offsets for the IRQ entries in the IDT
    write_byte_to_port(PIC1_DATA, offset1);
    write_byte_to_port(PIC2_DATA, offset2);
    // tell the master pic about the slave pic wired to IRQ2
    write_byte_to_port(PIC1_DATA, 0x04);
    // tell the slave pic its cascade identity
    write_byte_to_port(PIC2_DATA, 0x02);

    write_byte_to_port(PIC1_DATA, ICW4_8086);
    write_byte_to_port(PIC2_DATA, ICW4_8086);
    // restore masks
    write_byte_to_port(PIC1_DATA, imr_mask1);
    write_byte_to_port(PIC2_DATA, imr_mask2);
}
