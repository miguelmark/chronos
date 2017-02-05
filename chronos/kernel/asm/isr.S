.section .text
.macro isr_no_code id=0
.global do_isr\id
.type do_isr\id, @function
do_isr\id:
    cli
    push $0 # push a dummy value on the stack
    push $\id
    jmp isr_stub
.endm

.macro isr_w_code id=0
.global do_isr\id
.type do_isr\id, @function
do_isr\id:
    cli
    push $\id
    jmp isr_stub
.endm

isr_no_code 0
isr_no_code 1
isr_no_code 2
isr_no_code 3
isr_no_code 4
isr_no_code 5
isr_no_code 6
isr_no_code 7
isr_w_code 8
isr_no_code 9
isr_w_code 10
isr_w_code 11
isr_w_code 12
isr_w_code 13
isr_w_code 14
isr_no_code 15
isr_no_code 16
isr_w_code 17
isr_no_code 18
isr_no_code 19
isr_no_code 20
isr_no_code 21
isr_no_code 22
isr_no_code 23
isr_no_code 24
isr_no_code 25
isr_no_code 26
isr_no_code 27
isr_no_code 28
isr_no_code 29
isr_no_code 30
isr_no_code 31

# isrs here will be mapped to gates 32 --> 47 (0x20 - 0x2F)
# hardware interrupts

isr_no_code 32
isr_no_code 33
isr_no_code 34
isr_no_code 35
isr_no_code 36
isr_no_code 37
isr_no_code 38
isr_no_code 39
isr_no_code 40
isr_no_code 41
isr_no_code 42
isr_no_code 43
isr_no_code 44
isr_no_code 45
isr_no_code 46
isr_no_code 47


.extern handle_fault

isr_stub:
   pushal # save processor state
   push %ds # save segment details
   push %es
   push %fs
   push %gs
   mov $0x10, %ax # 0x10 points to the kernel data segment
   mov %ax, %ds
   mov %ax, %es
   mov %ax, %fs
   mov %ax, %gs
   movl %esp, %eax
   pushl %eax
   call handle_interrupt
   pop %eax
   pop %gs
   pop %fs
   pop %es
   pop %ds
   popal
   add $8, %esp # removes the isr id and error code
   iret # pops CS, EIP, EFLAGS, SS, and ESP
