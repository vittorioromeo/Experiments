	.file	"x.cpp"
	.text
	.align 2
	.type	_ZNR5boost4hana5fix_tIZ4mainEUlT_iE_EclIJiEEEDcDpOT_, @function
_ZNR5boost4hana5fix_tIZ4mainEUlT_iE_EclIJiEEEDcDpOT_:
.LFB2627:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	leaq	-4144(%rsp), %rsp
	orq	$0, (%rsp)
	leaq	4128(%rsp), %rsp
	.cfi_def_cfa_offset 48
	movq	%fs:40, %rax
	movq	%rax, 8(%rsp)
	xorl	%eax, %eax
	movl	(%rsi), %ebx
	movl	$1, %eax
	testl	%ebx, %ebx
	jne	.L10
.L1:
	movq	8(%rsp), %rdx
	xorq	%fs:40, %rdx
	jne	.L11
	addq	$16, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 32
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
.L10:
	.cfi_restore_state
	movl	%ebx, %ebp
	subl	$1, %ebp
	jne	.L12
.L3:
	imull	%ebx, %eax
	jmp	.L1
.L12:
	movb	$0, 3(%rsp)
	movl	%ebx, %r12d
	subl	$2, %r12d
	jne	.L13
.L4:
	imull	%ebp, %eax
	jmp	.L3
.L13:
	leal	-3(%rbx), %eax
	movl	%eax, 4(%rsp)
	leaq	4(%rsp), %rsi
	leaq	3(%rsp), %rdi
	call	_ZNR5boost4hana5fix_tIZ4mainEUlT_iE_EclIJiEEEDcDpOT_
	imull	%r12d, %eax
	jmp	.L4
.L11:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE2627:
	.size	_ZNR5boost4hana5fix_tIZ4mainEUlT_iE_EclIJiEEEDcDpOT_, .-_ZNR5boost4hana5fix_tIZ4mainEUlT_iE_EclIJiEEEDcDpOT_
	.globl	main
	.type	main, @function
main:
.LFB2622:
	.cfi_startproc
	leaq	-4168(%rsp), %rsp
	orq	$0, (%rsp)
	leaq	4128(%rsp), %rsp
	.cfi_def_cfa_offset 48
	movq	%fs:40, %rax
	movq	%rax, 24(%rsp)
	xorl	%eax, %eax
	movl	$6, 20(%rsp)
	leaq	20(%rsp), %rsi
	leaq	15(%rsp), %rdi
	call	_ZNR5boost4hana5fix_tIZ4mainEUlT_iE_EclIJiEEEDcDpOT_
	movl	%eax, 16(%rsp)
	movq	24(%rsp), %rdx
	xorq	%fs:40, %rdx
	jne	.L17
	movl	$0, %eax
	addq	$40, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L17:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE2622:
	.size	main, .-main
	.ident	"GCC: (GNU) 6.2.1 20160830"
	.section	.note.GNU-stack,"",@progbits
