.section .text
.extern idt_p
.global load_idt
.type load_idt, @function
load_idt:
	lidt (idt_p)
	ret
