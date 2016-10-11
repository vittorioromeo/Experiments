	.file	"baseline.cpp"
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB2622:
	.cfi_startproc
	movl	$720, -4(%rsp)
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE2622:
	.size	main, .-main
	.ident	"GCC: (GNU) 6.2.1 20160830"
	.section	.note.GNU-stack,"",@progbits
