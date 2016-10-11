	.text
	.file	"./x.cpp"
	.globl	main
	.align	16, 0x90
	.type	main,@function
main:                                   # @main
.Lfunc_begin0:
	.cfi_startproc
	.cfi_personality 155, DW.ref.__gxx_personality_v0
	.cfi_lsda 27, .Lexception0
# BB#0:
	pushq	%rbx
.Ltmp11:
	.cfi_def_cfa_offset 16
	subq	$64, %rsp
.Ltmp12:
	.cfi_def_cfa_offset 80
.Ltmp13:
	.cfi_offset %rbx, -16
	movq	%fs:40, %rax
	movq	%rax, 56(%rsp)
	leaq	16(%rsp), %rax
	movq	%rax, 16(%rsp)
	leaq	_ZNSt17_Function_handlerIFiiEZ4mainE3$_0E9_M_invokeERKSt9_Any_dataOi(%rip), %rax
	movq	%rax, 40(%rsp)
	leaq	_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E10_M_managerERSt9_Any_dataRKS3_St18_Manager_operation(%rip), %rax
	movq	%rax, 32(%rsp)
	movl	$5, 52(%rsp)
	cmpq	$0, 32(%rsp)
	je	.LBB0_1
.LBB0_2:
	movq	40(%rsp), %rax
.Ltmp0:
	leaq	16(%rsp), %rdi
	leaq	52(%rsp), %rsi
	callq	*%rax
.Ltmp1:
# BB#3:
	addl	%eax, %eax
	leal	(%rax,%rax,2), %eax
	movl	%eax, 12(%rsp)
	movq	32(%rsp), %rax
	testq	%rax, %rax
	je	.LBB0_5
# BB#4:
.Ltmp2:
	leaq	16(%rsp), %rdi
	movl	$3, %edx
	movq	%rdi, %rsi
	callq	*%rax
.Ltmp3:
.LBB0_5:                                # %_ZNSt14_Function_baseD2Ev.exit2
	movq	%fs:40, %rax
	cmpq	56(%rsp), %rax
	jne	.LBB0_10
# BB#6:                                 # %SP_return
	xorl	%eax, %eax
	addq	$64, %rsp
	popq	%rbx
	retq
.LBB0_1:
.Ltmp5:
	callq	_ZSt25__throw_bad_function_callv@PLT
.Ltmp6:
	jmp	.LBB0_2
.LBB0_7:
.Ltmp7:
	movq	%rax, %rbx
	movq	32(%rsp), %rax
	testq	%rax, %rax
	je	.LBB0_9
# BB#8:
.Ltmp8:
	leaq	16(%rsp), %rdi
	movl	$3, %edx
	movq	%rdi, %rsi
	callq	*%rax
.Ltmp9:
.LBB0_9:                                # %_ZNSt14_Function_baseD2Ev.exit
	movq	%rbx, %rdi
	callq	_Unwind_Resume@PLT
.LBB0_10:                               # %CallStackCheckFailBlk
	callq	__stack_chk_fail@PLT
.LBB0_11:
.Ltmp4:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.LBB0_12:
.Ltmp10:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.align	4
GCC_except_table0:
.Lexception0:
	.byte	255                     # @LPStart Encoding = omit
	.byte	155                     # @TType Encoding = indirect pcrel sdata4
	.byte	73                      # @TType base offset
	.byte	3                       # Call site Encoding = udata4
	.byte	65                      # Call site table length
	.long	.Ltmp0-.Lfunc_begin0    # >> Call Site 1 <<
	.long	.Ltmp1-.Ltmp0           #   Call between .Ltmp0 and .Ltmp1
	.long	.Ltmp7-.Lfunc_begin0    #     jumps to .Ltmp7
	.byte	0                       #   On action: cleanup
	.long	.Ltmp2-.Lfunc_begin0    # >> Call Site 2 <<
	.long	.Ltmp3-.Ltmp2           #   Call between .Ltmp2 and .Ltmp3
	.long	.Ltmp4-.Lfunc_begin0    #     jumps to .Ltmp4
	.byte	1                       #   On action: 1
	.long	.Ltmp5-.Lfunc_begin0    # >> Call Site 3 <<
	.long	.Ltmp6-.Ltmp5           #   Call between .Ltmp5 and .Ltmp6
	.long	.Ltmp7-.Lfunc_begin0    #     jumps to .Ltmp7
	.byte	0                       #   On action: cleanup
	.long	.Ltmp8-.Lfunc_begin0    # >> Call Site 4 <<
	.long	.Ltmp9-.Ltmp8           #   Call between .Ltmp8 and .Ltmp9
	.long	.Ltmp10-.Lfunc_begin0   #     jumps to .Ltmp10
	.byte	1                       #   On action: 1
	.long	.Ltmp9-.Lfunc_begin0    # >> Call Site 5 <<
	.long	.Lfunc_end0-.Ltmp9      #   Call between .Ltmp9 and .Lfunc_end0
	.long	0                       #     has no landing pad
	.byte	0                       #   On action: cleanup
	.byte	1                       # >> Action Record 1 <<
                                        #   Catch TypeInfo 1
	.byte	0                       #   No further actions
                                        # >> Catch TypeInfos <<
	.long	0                       # TypeInfo 1
	.align	4

	.section	.text.__clang_call_terminate,"axG",@progbits,__clang_call_terminate,comdat
	.hidden	__clang_call_terminate
	.weak	__clang_call_terminate
	.align	16, 0x90
	.type	__clang_call_terminate,@function
__clang_call_terminate:                 # @__clang_call_terminate
# BB#0:
	pushq	%rax
	callq	__cxa_begin_catch@PLT
	callq	_ZSt9terminatev@PLT
.Lfunc_end1:
	.size	__clang_call_terminate, .Lfunc_end1-__clang_call_terminate

	.text
	.align	16, 0x90
	.type	_ZNSt17_Function_handlerIFiiEZ4mainE3$_0E9_M_invokeERKSt9_Any_dataOi,@function
_ZNSt17_Function_handlerIFiiEZ4mainE3$_0E9_M_invokeERKSt9_Any_dataOi: # @"_ZNSt17_Function_handlerIFiiEZ4mainE3$_0E9_M_invokeERKSt9_Any_dataOi"
	.cfi_startproc
# BB#0:
	pushq	%rbx
.Ltmp14:
	.cfi_def_cfa_offset 16
	subq	$16, %rsp
.Ltmp15:
	.cfi_def_cfa_offset 32
.Ltmp16:
	.cfi_offset %rbx, -16
	movq	%fs:40, %rax
	movq	%rax, 8(%rsp)
	movl	(%rsi), %ebx
	movl	$1, %eax
	testl	%ebx, %ebx
	je	.LBB2_3
# BB#1:
	movq	(%rdi), %rdi
	leal	-1(%rbx), %eax
	movl	%eax, 4(%rsp)
	cmpq	$0, 16(%rdi)
	je	.LBB2_5
# BB#2:                                 # %_ZNKSt8functionIFiiEEclEi.exit.i
	leaq	4(%rsp), %rsi
	callq	*24(%rdi)
	imull	%ebx, %eax
.LBB2_3:                                # %"_ZZ4mainENK3$_0clEi.exit"
	movq	%fs:40, %rcx
	cmpq	8(%rsp), %rcx
	jne	.LBB2_4
# BB#6:                                 # %SP_return
	addq	$16, %rsp
	popq	%rbx
	retq
.LBB2_4:                                # %CallStackCheckFailBlk
	callq	__stack_chk_fail@PLT
.LBB2_5:
	callq	_ZSt25__throw_bad_function_callv@PLT
.Lfunc_end2:
	.size	_ZNSt17_Function_handlerIFiiEZ4mainE3$_0E9_M_invokeERKSt9_Any_dataOi, .Lfunc_end2-_ZNSt17_Function_handlerIFiiEZ4mainE3$_0E9_M_invokeERKSt9_Any_dataOi
	.cfi_endproc

	.align	16, 0x90
	.type	_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E10_M_managerERSt9_Any_dataRKS3_St18_Manager_operation,@function
_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E10_M_managerERSt9_Any_dataRKS3_St18_Manager_operation: # @"_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E10_M_managerERSt9_Any_dataRKS3_St18_Manager_operation"
	.cfi_startproc
# BB#0:
	cmpl	$2, %edx
	je	.LBB3_5
# BB#1:
	cmpl	$1, %edx
	je	.LBB3_4
# BB#2:
	testl	%edx, %edx
	jne	.LBB3_7
# BB#3:
	leaq	_ZTIZ4mainE3$_0(%rip), %rax
	jmp	.LBB3_6
.LBB3_5:
	movq	(%rsi), %rax
.LBB3_6:
	movq	%rax, (%rdi)
.LBB3_7:
	xorl	%eax, %eax
	retq
.LBB3_4:
	movq	%rsi, (%rdi)
	xorl	%eax, %eax
	retq
.Lfunc_end3:
	.size	_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E10_M_managerERSt9_Any_dataRKS3_St18_Manager_operation, .Lfunc_end3-_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E10_M_managerERSt9_Any_dataRKS3_St18_Manager_operation
	.cfi_endproc

	.type	_ZTSZ4mainE3$_0,@object # @"_ZTSZ4mainE3$_0"
	.section	.rodata,"a",@progbits
_ZTSZ4mainE3$_0:
	.asciz	"Z4mainE3$_0"
	.size	_ZTSZ4mainE3$_0, 12

	.type	_ZTIZ4mainE3$_0,@object # @"_ZTIZ4mainE3$_0"
	.section	.data.rel.ro,"aw",@progbits
	.align	8
_ZTIZ4mainE3$_0:
	.quad	_ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	_ZTSZ4mainE3$_0
	.size	_ZTIZ4mainE3$_0, 16

	.hidden	DW.ref.__gxx_personality_v0
	.weak	DW.ref.__gxx_personality_v0
	.section	.data.DW.ref.__gxx_personality_v0,"aGw",@progbits,DW.ref.__gxx_personality_v0,comdat
	.align	8
	.type	DW.ref.__gxx_personality_v0,@object
	.size	DW.ref.__gxx_personality_v0, 8
DW.ref.__gxx_personality_v0:
	.quad	__gxx_personality_v0

	.ident	"clang version 3.8.1 (tags/RELEASE_381/final)"
	.section	".note.GNU-stack","",@progbits
