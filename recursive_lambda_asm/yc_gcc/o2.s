	.file	"x.cpp"
	.text
	.align 2
	.p2align 4,,15
	.type	_ZZ4mainENKUlT_iE_clIN5boost4hana5fix_tIS0_EEEEiS_i.isra.1.part.2, @function
_ZZ4mainENKUlT_iE_clIN5boost4hana5fix_tIS0_EEEEiS_i.isra.1.part.2:
.LFB3042:
	.cfi_startproc
	movl	%edi, %eax
	subl	$1, %eax
	jne	.L20
	movl	$1, %ecx
	movl	$1, %edi
	movl	$1, %eax
.L2:
	imull	%edi, %eax
	imull	%ecx, %eax
	ret
	.p2align 4,,10
	.p2align 3
.L20:
	movl	%edi, %edx
	movl	$1, %ecx
	subl	$2, %edx
	jne	.L21
	movl	$2, %edi
	movl	$1, %edx
.L3:
	imull	%edx, %eax
	imull	%edi, %eax
	imull	%ecx, %eax
	ret
	.p2align 4,,10
	.p2align 3
.L21:
	cmpl	$3, %edi
	je	.L3
.L6:
	movl	%eax, %r8d
	leal	1(%rax), %esi
	leal	-2(%rax), %edi
	imull	%edx, %r8d
	imull	%r8d, %esi
	imull	%esi, %ecx
	subl	$3, %eax
	jne	.L22
	movl	$1, %eax
	jmp	.L2
	.p2align 4,,10
	.p2align 3
.L22:
	subl	$3, %edx
	jne	.L23
	movl	$1, %edx
	jmp	.L3
.L23:
	cmpl	$2, %eax
	je	.L3
	jmp	.L6
	.cfi_endproc
.LFE3042:
	.size	_ZZ4mainENKUlT_iE_clIN5boost4hana5fix_tIS0_EEEEiS_i.isra.1.part.2, .-_ZZ4mainENKUlT_iE_clIN5boost4hana5fix_tIS0_EEEEiS_i.isra.1.part.2
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB2622:
	.cfi_startproc
	subq	$4144, %rsp
	orq	$0, (%rsp)
	addq	$4128, %rsp
	.cfi_def_cfa_offset 24
	movl	$5, %edi
	call	_ZZ4mainENKUlT_iE_clIN5boost4hana5fix_tIS0_EEEEiS_i.isra.1.part.2
	leal	(%rax,%rax,2), %eax
	addl	%eax, %eax
	movl	%eax, 12(%rsp)
	xorl	%eax, %eax
	addq	$16, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE2622:
	.size	main, .-main
	.ident	"GCC: (GNU) 6.2.1 20160830"
	.section	.note.GNU-stack,"",@progbits
