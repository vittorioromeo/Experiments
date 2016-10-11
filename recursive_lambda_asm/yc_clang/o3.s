	.text
	.file	"./x.cpp"
	.globl	main
	.align	16, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# BB#0:                                 # %"_ZNR5boost4hana5fix_tIZ4mainE3$_0EclIJiEEEDcDpOT_.exit"
	subq	$24, %rsp
.Ltmp0:
	.cfi_def_cfa_offset 32
	movq	%fs:40, %rax
	movq	%rax, 16(%rsp)
	movl	$720, 12(%rsp)          # imm = 0x2D0
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


	.ident	"clang version 3.8.1 (tags/RELEASE_381/final)"
	.section	".note.GNU-stack","",@progbits
