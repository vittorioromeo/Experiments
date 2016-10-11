	.text
	.file	"./x.cpp"
	.globl	main
	.align	16, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# BB#0:
	subq	$24, %rsp
.Ltmp0:
	.cfi_def_cfa_offset 32
	movq	%fs:40, %rax
	movq	%rax, 16(%rsp)
	movl	$6, (%rsp)
	leaq	8(%rsp), %rdi
	leaq	(%rsp), %rsi
	callq	_ZNR5boost4hana5fix_tIZ4mainE3$_0EclIJiEEEDcDpOT_
	movl	%eax, 4(%rsp)
	movq	%fs:40, %rax
	cmpq	16(%rsp), %rax
	jne	.LBB0_2
# BB#1:                                 # %SP_return
	xorl	%eax, %eax
	addq	$24, %rsp
	retq
.LBB0_2:                                # %CallStackCheckFailBlk
	callq	__stack_chk_fail@PLT
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc

	.align	16, 0x90
	.type	_ZNR5boost4hana5fix_tIZ4mainE3$_0EclIJiEEEDcDpOT_,@function
_ZNR5boost4hana5fix_tIZ4mainE3$_0EclIJiEEEDcDpOT_: # @"_ZNR5boost4hana5fix_tIZ4mainE3$_0EclIJiEEEDcDpOT_"
	.cfi_startproc
# BB#0:
	movl	(%rsi), %edi
	jmp	_ZZ4mainENK3$_0clIN5boost4hana5fix_tIS_EEEEiT_i # TAILCALL
.Lfunc_end1:
	.size	_ZNR5boost4hana5fix_tIZ4mainE3$_0EclIJiEEEDcDpOT_, .Lfunc_end1-_ZNR5boost4hana5fix_tIZ4mainE3$_0EclIJiEEEDcDpOT_
	.cfi_endproc

	.align	16, 0x90
	.type	_ZZ4mainENK3$_0clIN5boost4hana5fix_tIS_EEEEiT_i,@function
_ZZ4mainENK3$_0clIN5boost4hana5fix_tIS_EEEEiT_i: # @"_ZZ4mainENK3$_0clIN5boost4hana5fix_tIS_EEEEiT_i"
	.cfi_startproc
# BB#0:
	pushq	%rbx
.Ltmp1:
	.cfi_def_cfa_offset 16
	subq	$32, %rsp
.Ltmp2:
	.cfi_def_cfa_offset 48
.Ltmp3:
	.cfi_offset %rbx, -16
	movl	%edi, %ebx
	movq	%fs:40, %rax
	movq	%rax, 24(%rsp)
	movl	$1, %eax
	testl	%ebx, %ebx
	je	.LBB2_2
# BB#1:
	leal	-1(%rbx), %eax
	movl	%eax, 12(%rsp)
	leaq	16(%rsp), %rdi
	leaq	12(%rsp), %rsi
	callq	_ZNR5boost4hana5fix_tIZ4mainE3$_0EclIJiEEEDcDpOT_
	imull	%ebx, %eax
.LBB2_2:
	movq	%fs:40, %rcx
	cmpq	24(%rsp), %rcx
	jne	.LBB2_4
# BB#3:                                 # %SP_return
	addq	$32, %rsp
	popq	%rbx
	retq
.LBB2_4:                                # %CallStackCheckFailBlk
	callq	__stack_chk_fail@PLT
.Lfunc_end2:
	.size	_ZZ4mainENK3$_0clIN5boost4hana5fix_tIS_EEEEiT_i, .Lfunc_end2-_ZZ4mainENK3$_0clIN5boost4hana5fix_tIS_EEEEiT_i
	.cfi_endproc

	.type	_ZN5boost4hanaL3fixE,@object # @_ZN5boost4hanaL3fixE
	.section	.rodata,"a",@progbits
_ZN5boost4hanaL3fixE:
	.zero	1
	.size	_ZN5boost4hanaL3fixE, 1


	.ident	"clang version 3.8.1 (tags/RELEASE_381/final)"
	.section	".note.GNU-stack","",@progbits
