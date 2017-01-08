/* x86 crtn.s */
.section .init
	/* crtend.o's .init */
	popl %ebp
	ret

.section .fini
	/* crtend.o's .fini */
	popl %ebp
	ret
