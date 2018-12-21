.section .text
/* obviously incurring the cost of a function call
   to read a register is inefficient */
.global get_ebp
get_ebp:
movl %ebp, %eax
ret
