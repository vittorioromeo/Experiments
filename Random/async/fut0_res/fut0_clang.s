	.text
	.file	"./fut0.cpp"
	.globl	_Z9get_valuev
	.p2align	4, 0x90
	.type	_Z9get_valuev,@function
_Z9get_valuev:                          # @_Z9get_valuev
	.cfi_startproc
# BB#0:
	xorl	%eax, %eax
	retq
.Lfunc_end0:
	.size	_Z9get_valuev, .Lfunc_end0-_Z9get_valuev
	.cfi_endproc

	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
.Lfunc_begin0:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception0
# BB#0:
	pushq	%rbp
.Ltmp3:
	.cfi_def_cfa_offset 16
	pushq	%rbx
.Ltmp4:
	.cfi_def_cfa_offset 24
	subq	$24, %rsp
.Ltmp5:
	.cfi_def_cfa_offset 48
.Ltmp6:
	.cfi_offset %rbx, -24
.Ltmp7:
	.cfi_offset %rbp, -16
	leaq	8(%rsp), %rbx
	movl	$1, %esi
	movl	$_Z9get_valuev, %edx
	movq	%rbx, %rdi
	callq	_ZSt5asyncIRFivEJEESt6futureINSt9result_ofIFNSt5decayIT_E4typeEDpNS4_IT0_E4typeEEE4typeEESt6launchOS5_DpOS8_
.Ltmp0:
	movq	%rbx, %rdi
	callq	_ZNSt6futureIiE3getEv
	movl	%eax, %ebp
.Ltmp1:
# BB#1:
	movq	16(%rsp), %rbx
	testq	%rbx, %rbx
	je	.LBB1_11
# BB#2:
	leaq	8(%rbx), %rax
	movl	$__pthread_key_create, %ecx
	testq	%rcx, %rcx
	je	.LBB1_4
# BB#3:
	movl	$-1, %ecx
	lock		xaddl	%ecx, (%rax)
	jmp	.LBB1_5
.LBB1_4:
	movl	(%rax), %ecx
	leal	-1(%rcx), %edx
	movl	%edx, (%rax)
.LBB1_5:
	cmpl	$1, %ecx
	jne	.LBB1_11
# BB#6:
	movq	(%rbx), %rax
	movq	%rbx, %rdi
	callq	*16(%rax)
	leaq	12(%rbx), %rax
	movl	$__pthread_key_create, %ecx
	testq	%rcx, %rcx
	je	.LBB1_8
# BB#7:
	movl	$-1, %ecx
	lock		xaddl	%ecx, (%rax)
	jmp	.LBB1_9
.LBB1_8:
	movl	(%rax), %ecx
	leal	-1(%rcx), %edx
	movl	%edx, (%rax)
.LBB1_9:
	cmpl	$1, %ecx
	jne	.LBB1_11
# BB#10:
	movq	(%rbx), %rax
	movq	%rbx, %rdi
	callq	*24(%rax)
.LBB1_11:
	movl	%ebp, %eax
	addq	$24, %rsp
	popq	%rbx
	popq	%rbp
	retq
.LBB1_12:
.Ltmp2:
	movq	%rax, %rbx
	movq	16(%rsp), %rbp
	testq	%rbp, %rbp
	je	.LBB1_22
# BB#13:
	leaq	8(%rbp), %rax
	movl	$__pthread_key_create, %ecx
	testq	%rcx, %rcx
	je	.LBB1_15
# BB#14:
	movl	$-1, %ecx
	lock		xaddl	%ecx, (%rax)
	jmp	.LBB1_16
.LBB1_15:
	movl	(%rax), %ecx
	leal	-1(%rcx), %edx
	movl	%edx, (%rax)
.LBB1_16:
	cmpl	$1, %ecx
	jne	.LBB1_22
# BB#17:
	movq	(%rbp), %rax
	movq	%rbp, %rdi
	callq	*16(%rax)
	leaq	12(%rbp), %rax
	movl	$__pthread_key_create, %ecx
	testq	%rcx, %rcx
	je	.LBB1_19
# BB#18:
	movl	$-1, %ecx
	lock		xaddl	%ecx, (%rax)
	jmp	.LBB1_20
.LBB1_19:
	movl	(%rax), %ecx
	leal	-1(%rcx), %edx
	movl	%edx, (%rax)
.LBB1_20:
	cmpl	$1, %ecx
	jne	.LBB1_22
# BB#21:
	movq	(%rbp), %rax
	movq	%rbp, %rdi
	callq	*24(%rax)
.LBB1_22:
	movq	%rbx, %rdi
	callq	_Unwind_Resume
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table1:
.Lexception0:
	.byte	255                     # @LPStart Encoding = omit
	.byte	3                       # @TType Encoding = udata4
	.byte	41                      # @TType base offset
	.byte	3                       # Call site Encoding = udata4
	.byte	39                      # Call site table length
	.long	.Lfunc_begin0-.Lfunc_begin0 # >> Call Site 1 <<
	.long	.Ltmp0-.Lfunc_begin0    #   Call between .Lfunc_begin0 and .Ltmp0
	.long	0                       #     has no landing pad
	.byte	0                       #   On action: cleanup
	.long	.Ltmp0-.Lfunc_begin0    # >> Call Site 2 <<
	.long	.Ltmp1-.Ltmp0           #   Call between .Ltmp0 and .Ltmp1
	.long	.Ltmp2-.Lfunc_begin0    #     jumps to .Ltmp2
	.byte	0                       #   On action: cleanup
	.long	.Ltmp1-.Lfunc_begin0    # >> Call Site 3 <<
	.long	.Lfunc_end1-.Ltmp1      #   Call between .Ltmp1 and .Lfunc_end1
	.long	0                       #     has no landing pad
	.byte	0                       #   On action: cleanup
	.p2align	2

	.section	.text._ZSt5asyncIRFivEJEESt6futureINSt9result_ofIFNSt5decayIT_E4typeEDpNS4_IT0_E4typeEEE4typeEESt6launchOS5_DpOS8_,"axG",@progbits,_ZSt5asyncIRFivEJEESt6futureINSt9result_ofIFNSt5decayIT_E4typeEDpNS4_IT0_E4typeEEE4typeEESt6launchOS5_DpOS8_,comdat
	.weak	_ZSt5asyncIRFivEJEESt6futureINSt9result_ofIFNSt5decayIT_E4typeEDpNS4_IT0_E4typeEEE4typeEESt6launchOS5_DpOS8_
	.p2align	4, 0x90
	.type	_ZSt5asyncIRFivEJEESt6futureINSt9result_ofIFNSt5decayIT_E4typeEDpNS4_IT0_E4typeEEE4typeEESt6launchOS5_DpOS8_,@function
_ZSt5asyncIRFivEJEESt6futureINSt9result_ofIFNSt5decayIT_E4typeEDpNS4_IT0_E4typeEEE4typeEESt6launchOS5_DpOS8_: # @_ZSt5asyncIRFivEJEESt6futureINSt9result_ofIFNSt5decayIT_E4typeEDpNS4_IT0_E4typeEEE4typeEESt6launchOS5_DpOS8_
.Lfunc_begin1:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception1
# BB#0:
	pushq	%rbp
.Ltmp37:
	.cfi_def_cfa_offset 16
	pushq	%r15
.Ltmp38:
	.cfi_def_cfa_offset 24
	pushq	%r14
.Ltmp39:
	.cfi_def_cfa_offset 32
	pushq	%r13
.Ltmp40:
	.cfi_def_cfa_offset 40
	pushq	%r12
.Ltmp41:
	.cfi_def_cfa_offset 48
	pushq	%rbx
.Ltmp42:
	.cfi_def_cfa_offset 56
	subq	$40, %rsp
.Ltmp43:
	.cfi_def_cfa_offset 96
.Ltmp44:
	.cfi_offset %rbx, -56
.Ltmp45:
	.cfi_offset %r12, -48
.Ltmp46:
	.cfi_offset %r13, -40
.Ltmp47:
	.cfi_offset %r14, -32
.Ltmp48:
	.cfi_offset %r15, -24
.Ltmp49:
	.cfi_offset %rbp, -16
	movq	%rdx, %r13
	movl	%esi, %ebp
	movq	%rdi, %r14
	xorl	%r15d, %r15d
	testb	$1, %bpl
	je	.LBB2_11
# BB#1:
	movq	%r13, (%rsp)
	xorps	%xmm0, %xmm0
	movaps	%xmm0, 16(%rsp)
.Ltmp8:
	movl	$80, %edi
	callq	_Znwm
	movq	%rax, %rbx
.Ltmp9:
# BB#2:
	movl	$1, 8(%rbx)
	movl	$1, 12(%rbx)
	movq	$_ZTVSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE+16, (%rbx)
	movq	%rbx, %rdi
	addq	$16, %rdi
.Ltmp11:
	leaq	(%rsp), %rsi
	callq	_ZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEC2EOS5_
.Ltmp12:
# BB#3:
	leaq	24(%rsp), %rbp
	movq	%rbx, 24(%rsp)
	movq	(%rbx), %rax
	movl	$_ZTISt19_Sp_make_shared_tag, %esi
	movq	%rbx, %rdi
	callq	*32(%rax)
	movq	%rax, %rcx
	movq	%rcx, 16(%rsp)
	xorl	%eax, %eax
	movq	%rbp, %rdi
	movq	%rcx, %rsi
	movq	%rcx, %rdx
	callq	_ZSt32__enable_shared_from_this_helperILN9__gnu_cxx12_Lock_policyE2EEvRKSt14__shared_countIXT_EEz
	movq	16(%rsp), %rbp
	movq	24(%rsp), %r15
	testq	%rbp, %rbp
	movq	%r15, %rbx
	jne	.LBB2_24
	jmp	.LBB2_11
.LBB2_4:
.Ltmp13:
	movq	%rdx, %r15
	movq	%rax, %r12
	movq	%rbx, %rdi
	callq	_ZdlPv
	jmp	.LBB2_6
.LBB2_5:
.Ltmp10:
	movq	%rdx, %r15
	movq	%rax, %r12
.LBB2_6:
	cmpl	$1, %r15d
	jne	.LBB2_74
# BB#7:
	movq	%r12, %rdi
	callq	__cxa_begin_catch
	movq	%rax, %rbx
	callq	_ZNSt3_V216generic_categoryEv
	movl	$11, (%rsp)
	movq	%rax, 8(%rsp)
	movq	24(%rbx), %rdi
	movq	(%rdi), %rax
	movl	16(%rbx), %esi
	leaq	(%rsp), %rdx
	callq	*48(%rax)
	testb	%al, %al
	jne	.LBB2_9
# BB#8:
	addq	$16, %rbx
	movq	8(%rsp), %rdi
	movq	(%rdi), %rax
	movl	(%rsp), %edx
	movq	%rbx, %rsi
	callq	*56(%rax)
	testb	%al, %al
	je	.LBB2_45
.LBB2_9:
	testb	$2, %bpl
	je	.LBB2_45
# BB#10:
	xorl	%r15d, %r15d
.Ltmp14:
	callq	__cxa_end_catch
.Ltmp15:
.LBB2_11:
	xorps	%xmm0, %xmm0
	movaps	%xmm0, (%rsp)
.Ltmp23:
	movl	$64, %edi
	callq	_Znwm
	movq	%rax, %rbx
.Ltmp24:
# BB#12:
	movl	$1, 8(%rbx)
	movl	$1, 12(%rbx)
	movq	$_ZTVSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE+16, (%rbx)
	movq	$0, 24(%rbx)
	movl	$0, 32(%rbx)
	movb	$0, 36(%rbx)
	movl	$0, 40(%rbx)
	movq	$_ZTVNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEE+16, 16(%rbx)
.Ltmp26:
	movl	$24, %edi
	callq	_Znwm
	movq	%rax, %rbp
.Ltmp27:
# BB#13:
.Ltmp29:
	movq	%rbp, %rdi
	callq	_ZNSt13__future_base12_Result_baseC2Ev
.Ltmp30:
# BB#14:
	leaq	8(%rsp), %r12
	movq	$_ZTVNSt13__future_base7_ResultIiEE+16, (%rbp)
	movb	$0, 20(%rbp)
	movq	%rbp, 48(%rbx)
	movq	%r13, 56(%rbx)
	movq	%rbx, 8(%rsp)
	movq	(%rbx), %rax
	movl	$_ZTISt19_Sp_make_shared_tag, %esi
	movq	%rbx, %rdi
	callq	*32(%rax)
	movq	%rax, %rcx
	movq	%rcx, (%rsp)
	xorl	%eax, %eax
	movq	%r12, %rdi
	movq	%rcx, %rsi
	movq	%rcx, %rdx
	callq	_ZSt32__enable_shared_from_this_helperILN9__gnu_cxx12_Lock_policyE2EEvRKSt14__shared_countIXT_EEz
	movq	(%rsp), %rbp
	movq	8(%rsp), %rbx
	testq	%r15, %r15
	je	.LBB2_24
# BB#15:
	leaq	8(%r15), %rax
	movl	$__pthread_key_create, %ecx
	testq	%rcx, %rcx
	je	.LBB2_17
# BB#16:
	movl	$-1, %ecx
	lock		xaddl	%ecx, (%rax)
	jmp	.LBB2_18
.LBB2_17:
	movl	(%rax), %ecx
	leal	-1(%rcx), %edx
	movl	%edx, (%rax)
.LBB2_18:
	cmpl	$1, %ecx
	jne	.LBB2_24
# BB#19:
	movq	(%r15), %rax
	movq	%r15, %rdi
	callq	*16(%rax)
	leaq	12(%r15), %rax
	movl	$__pthread_key_create, %ecx
	testq	%rcx, %rcx
	je	.LBB2_21
# BB#20:
	movl	$-1, %ecx
	lock		xaddl	%ecx, (%rax)
	jmp	.LBB2_22
.LBB2_21:
	movl	(%rax), %ecx
	leal	-1(%rcx), %edx
	movl	%edx, (%rax)
.LBB2_22:
	cmpl	$1, %ecx
	jne	.LBB2_24
# BB#23:
	movq	(%r15), %rax
	movq	%r15, %rdi
	callq	*24(%rax)
.LBB2_24:
	movq	%rbp, (%r14)
	movq	%rbx, 8(%r14)
	testq	%rbx, %rbx
	je	.LBB2_28
# BB#25:
	leaq	8(%rbx), %rax
	movl	$__pthread_key_create, %ecx
	testq	%rcx, %rcx
	je	.LBB2_27
# BB#26:
	lock		incl	(%rax)
	movq	(%r14), %rbp
	jmp	.LBB2_28
.LBB2_27:
	incl	(%rax)
.LBB2_28:
	testq	%rbp, %rbp
	je	.LBB2_41
# BB#29:
	movb	$1, %al
	xchgb	%al, 20(%rbp)
	testb	%al, %al
	jne	.LBB2_43
# BB#30:
	testq	%rbx, %rbx
	je	.LBB2_40
# BB#31:
	leaq	8(%rbx), %rax
	movl	$__pthread_key_create, %ecx
	testq	%rcx, %rcx
	je	.LBB2_33
# BB#32:
	movl	$-1, %ecx
	lock		xaddl	%ecx, (%rax)
	jmp	.LBB2_34
.LBB2_33:
	movl	(%rax), %ecx
	leal	-1(%rcx), %edx
	movl	%edx, (%rax)
.LBB2_34:
	cmpl	$1, %ecx
	jne	.LBB2_40
# BB#35:
	movq	(%rbx), %rax
	movq	%rbx, %rdi
	callq	*16(%rax)
	leaq	12(%rbx), %rax
	movl	$__pthread_key_create, %ecx
	testq	%rcx, %rcx
	je	.LBB2_37
# BB#36:
	movl	$-1, %ecx
	lock		xaddl	%ecx, (%rax)
	jmp	.LBB2_38
.LBB2_37:
	movl	(%rax), %ecx
	leal	-1(%rcx), %edx
	movl	%edx, (%rax)
.LBB2_38:
	cmpl	$1, %ecx
	jne	.LBB2_40
# BB#39:
	movq	(%rbx), %rax
	movq	%rbx, %rdi
	callq	*24(%rax)
.LBB2_40:
	movq	%r14, %rax
	addq	$40, %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	retq
.LBB2_41:
.Ltmp34:
	movl	$3, %edi
	callq	_ZSt20__throw_future_errori
.Ltmp35:
# BB#42:
.LBB2_43:
.Ltmp32:
	movl	$1, %edi
	callq	_ZSt20__throw_future_errori
.Ltmp33:
# BB#44:
.LBB2_45:
.Ltmp17:
	callq	__cxa_rethrow
.Ltmp18:
# BB#46:
.LBB2_47:
.Ltmp19:
	movq	%rax, %r12
.Ltmp20:
	callq	__cxa_end_catch
.Ltmp21:
	jmp	.LBB2_74
.LBB2_48:
.Ltmp22:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.LBB2_49:
.Ltmp16:
	movq	%rax, %r12
	xorl	%ebx, %ebx
	jmp	.LBB2_60
.LBB2_50:
.Ltmp36:
	movq	%rax, %r12
	movq	8(%r14), %r14
	testq	%r14, %r14
	je	.LBB2_60
# BB#51:
	leaq	8(%r14), %rax
	movl	$__pthread_key_create, %ecx
	testq	%rcx, %rcx
	je	.LBB2_53
# BB#52:
	movl	$-1, %ecx
	lock		xaddl	%ecx, (%rax)
	jmp	.LBB2_54
.LBB2_53:
	movl	(%rax), %ecx
	leal	-1(%rcx), %edx
	movl	%edx, (%rax)
.LBB2_54:
	cmpl	$1, %ecx
	jne	.LBB2_60
# BB#55:
	movq	(%r14), %rax
	movq	%r14, %rdi
	callq	*16(%rax)
	leaq	12(%r14), %rax
	movl	$__pthread_key_create, %ecx
	testq	%rcx, %rcx
	je	.LBB2_57
# BB#56:
	movl	$-1, %ecx
	lock		xaddl	%ecx, (%rax)
	jmp	.LBB2_58
.LBB2_57:
	movl	(%rax), %ecx
	leal	-1(%rcx), %edx
	movl	%edx, (%rax)
.LBB2_58:
	cmpl	$1, %ecx
	jne	.LBB2_60
# BB#59:
	movq	(%r14), %rax
	movq	%r14, %rdi
	callq	*24(%rax)
.LBB2_60:
	movq	%rbx, %r15
	jmp	.LBB2_64
.LBB2_61:
.Ltmp31:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.LBB2_62:
.Ltmp28:
	movq	%rax, %r12
	movq	%rbx, %rdi
	callq	_ZdlPv
	jmp	.LBB2_64
.LBB2_63:
.Ltmp25:
	movq	%rax, %r12
.LBB2_64:
	testq	%r15, %r15
	je	.LBB2_74
# BB#65:
	leaq	8(%r15), %rax
	movl	$__pthread_key_create, %ecx
	testq	%rcx, %rcx
	je	.LBB2_67
# BB#66:
	movl	$-1, %ecx
	lock		xaddl	%ecx, (%rax)
	jmp	.LBB2_68
.LBB2_67:
	movl	(%rax), %ecx
	leal	-1(%rcx), %edx
	movl	%edx, (%rax)
.LBB2_68:
	cmpl	$1, %ecx
	jne	.LBB2_74
# BB#69:
	movq	(%r15), %rax
	movq	%r15, %rdi
	callq	*16(%rax)
	leaq	12(%r15), %rax
	movl	$__pthread_key_create, %ecx
	testq	%rcx, %rcx
	je	.LBB2_71
# BB#70:
	movl	$-1, %ecx
	lock		xaddl	%ecx, (%rax)
	jmp	.LBB2_72
.LBB2_71:
	movl	(%rax), %ecx
	leal	-1(%rcx), %edx
	movl	%edx, (%rax)
.LBB2_72:
	cmpl	$1, %ecx
	jne	.LBB2_74
# BB#73:
	movq	(%r15), %rax
	movq	%r15, %rdi
	callq	*24(%rax)
.LBB2_74:
	movq	%r12, %rdi
	callq	_Unwind_Resume
.Lfunc_end2:
	.size	_ZSt5asyncIRFivEJEESt6futureINSt9result_ofIFNSt5decayIT_E4typeEDpNS4_IT0_E4typeEEE4typeEESt6launchOS5_DpOS8_, .Lfunc_end2-_ZSt5asyncIRFivEJEESt6futureINSt9result_ofIFNSt5decayIT_E4typeEDpNS4_IT0_E4typeEEE4typeEESt6launchOS5_DpOS8_
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table2:
.Lexception1:
	.byte	255                     # @LPStart Encoding = omit
	.byte	3                       # @TType Encoding = udata4
	.asciz	"\255\201\200\200"      # @TType base offset
	.byte	3                       # Call site Encoding = udata4
	.ascii	"\234\001"              # Call site table length
	.long	.Ltmp8-.Lfunc_begin1    # >> Call Site 1 <<
	.long	.Ltmp9-.Ltmp8           #   Call between .Ltmp8 and .Ltmp9
	.long	.Ltmp10-.Lfunc_begin1   #     jumps to .Ltmp10
	.byte	3                       #   On action: 2
	.long	.Ltmp11-.Lfunc_begin1   # >> Call Site 2 <<
	.long	.Ltmp12-.Ltmp11         #   Call between .Ltmp11 and .Ltmp12
	.long	.Ltmp13-.Lfunc_begin1   #     jumps to .Ltmp13
	.byte	3                       #   On action: 2
	.long	.Ltmp12-.Lfunc_begin1   # >> Call Site 3 <<
	.long	.Ltmp14-.Ltmp12         #   Call between .Ltmp12 and .Ltmp14
	.long	0                       #     has no landing pad
	.byte	0                       #   On action: cleanup
	.long	.Ltmp14-.Lfunc_begin1   # >> Call Site 4 <<
	.long	.Ltmp15-.Ltmp14         #   Call between .Ltmp14 and .Ltmp15
	.long	.Ltmp16-.Lfunc_begin1   #     jumps to .Ltmp16
	.byte	0                       #   On action: cleanup
	.long	.Ltmp23-.Lfunc_begin1   # >> Call Site 5 <<
	.long	.Ltmp24-.Ltmp23         #   Call between .Ltmp23 and .Ltmp24
	.long	.Ltmp25-.Lfunc_begin1   #     jumps to .Ltmp25
	.byte	0                       #   On action: cleanup
	.long	.Ltmp26-.Lfunc_begin1   # >> Call Site 6 <<
	.long	.Ltmp27-.Ltmp26         #   Call between .Ltmp26 and .Ltmp27
	.long	.Ltmp28-.Lfunc_begin1   #     jumps to .Ltmp28
	.byte	0                       #   On action: cleanup
	.long	.Ltmp29-.Lfunc_begin1   # >> Call Site 7 <<
	.long	.Ltmp30-.Ltmp29         #   Call between .Ltmp29 and .Ltmp30
	.long	.Ltmp31-.Lfunc_begin1   #     jumps to .Ltmp31
	.byte	5                       #   On action: 3
	.long	.Ltmp30-.Lfunc_begin1   # >> Call Site 8 <<
	.long	.Ltmp34-.Ltmp30         #   Call between .Ltmp30 and .Ltmp34
	.long	0                       #     has no landing pad
	.byte	0                       #   On action: cleanup
	.long	.Ltmp34-.Lfunc_begin1   # >> Call Site 9 <<
	.long	.Ltmp33-.Ltmp34         #   Call between .Ltmp34 and .Ltmp33
	.long	.Ltmp36-.Lfunc_begin1   #     jumps to .Ltmp36
	.byte	0                       #   On action: cleanup
	.long	.Ltmp17-.Lfunc_begin1   # >> Call Site 10 <<
	.long	.Ltmp18-.Ltmp17         #   Call between .Ltmp17 and .Ltmp18
	.long	.Ltmp19-.Lfunc_begin1   #     jumps to .Ltmp19
	.byte	0                       #   On action: cleanup
	.long	.Ltmp20-.Lfunc_begin1   # >> Call Site 11 <<
	.long	.Ltmp21-.Ltmp20         #   Call between .Ltmp20 and .Ltmp21
	.long	.Ltmp22-.Lfunc_begin1   #     jumps to .Ltmp22
	.byte	5                       #   On action: 3
	.long	.Ltmp21-.Lfunc_begin1   # >> Call Site 12 <<
	.long	.Lfunc_end2-.Ltmp21     #   Call between .Ltmp21 and .Lfunc_end2
	.long	0                       #     has no landing pad
	.byte	0                       #   On action: cleanup
	.byte	0                       # >> Action Record 1 <<
                                        #   Cleanup
	.byte	0                       #   No further actions
	.byte	1                       # >> Action Record 2 <<
                                        #   Catch TypeInfo 1
	.byte	125                     #   Continue to action 1
	.byte	2                       # >> Action Record 3 <<
                                        #   Catch TypeInfo 2
	.byte	0                       #   No further actions
                                        # >> Catch TypeInfos <<
	.long	0                       # TypeInfo 2
	.long	_ZTISt12system_error    # TypeInfo 1
	.p2align	2

	.section	.text._ZNSt6futureIiE3getEv,"axG",@progbits,_ZNSt6futureIiE3getEv,comdat
	.weak	_ZNSt6futureIiE3getEv
	.p2align	4, 0x90
	.type	_ZNSt6futureIiE3getEv,@function
_ZNSt6futureIiE3getEv:                  # @_ZNSt6futureIiE3getEv
.Lfunc_begin2:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception2
# BB#0:
	pushq	%rbp
.Ltmp53:
	.cfi_def_cfa_offset 16
	pushq	%r14
.Ltmp54:
	.cfi_def_cfa_offset 24
	pushq	%rbx
.Ltmp55:
	.cfi_def_cfa_offset 32
.Ltmp56:
	.cfi_offset %rbx, -32
.Ltmp57:
	.cfi_offset %r14, -24
.Ltmp58:
	.cfi_offset %rbp, -16
	movq	%rdi, %rbp
.Ltmp50:
	callq	_ZNKSt14__basic_futureIiE13_M_get_resultEv
.Ltmp51:
# BB#1:
	movl	16(%rax), %r14d
	movq	8(%rbp), %rbx
	xorps	%xmm0, %xmm0
	movups	%xmm0, (%rbp)
	testq	%rbx, %rbx
	je	.LBB3_11
# BB#2:
	leaq	8(%rbx), %rax
	movl	$__pthread_key_create, %ecx
	testq	%rcx, %rcx
	je	.LBB3_4
# BB#3:
	movl	$-1, %ecx
	lock		xaddl	%ecx, (%rax)
	jmp	.LBB3_5
.LBB3_4:
	movl	(%rax), %ecx
	leal	-1(%rcx), %edx
	movl	%edx, (%rax)
.LBB3_5:
	cmpl	$1, %ecx
	jne	.LBB3_11
# BB#6:
	movq	(%rbx), %rax
	movq	%rbx, %rdi
	callq	*16(%rax)
	leaq	12(%rbx), %rax
	movl	$__pthread_key_create, %ecx
	testq	%rcx, %rcx
	je	.LBB3_8
# BB#7:
	movl	$-1, %ecx
	lock		xaddl	%ecx, (%rax)
	jmp	.LBB3_9
.LBB3_8:
	movl	(%rax), %ecx
	leal	-1(%rcx), %edx
	movl	%edx, (%rax)
.LBB3_9:
	cmpl	$1, %ecx
	jne	.LBB3_11
# BB#10:
	movq	(%rbx), %rax
	movq	%rbx, %rdi
	callq	*24(%rax)
.LBB3_11:
	movl	%r14d, %eax
	popq	%rbx
	popq	%r14
	popq	%rbp
	retq
.LBB3_12:
.Ltmp52:
	movq	%rax, %r14
	movq	8(%rbp), %rbx
	xorps	%xmm0, %xmm0
	movups	%xmm0, (%rbp)
	testq	%rbx, %rbx
	je	.LBB3_22
# BB#13:
	leaq	8(%rbx), %rax
	movl	$__pthread_key_create, %ecx
	testq	%rcx, %rcx
	je	.LBB3_15
# BB#14:
	movl	$-1, %ecx
	lock		xaddl	%ecx, (%rax)
	jmp	.LBB3_16
.LBB3_15:
	movl	(%rax), %ecx
	leal	-1(%rcx), %edx
	movl	%edx, (%rax)
.LBB3_16:
	cmpl	$1, %ecx
	jne	.LBB3_22
# BB#17:
	movq	(%rbx), %rax
	movq	%rbx, %rdi
	callq	*16(%rax)
	leaq	12(%rbx), %rax
	movl	$__pthread_key_create, %ecx
	testq	%rcx, %rcx
	je	.LBB3_19
# BB#18:
	movl	$-1, %ecx
	lock		xaddl	%ecx, (%rax)
	jmp	.LBB3_20
.LBB3_19:
	movl	(%rax), %ecx
	leal	-1(%rcx), %edx
	movl	%edx, (%rax)
.LBB3_20:
	cmpl	$1, %ecx
	jne	.LBB3_22
# BB#21:
	movq	(%rbx), %rax
	movq	%rbx, %rdi
	callq	*24(%rax)
.LBB3_22:
	movq	%r14, %rdi
	callq	_Unwind_Resume
.Lfunc_end3:
	.size	_ZNSt6futureIiE3getEv, .Lfunc_end3-_ZNSt6futureIiE3getEv
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table3:
.Lexception2:
	.byte	255                     # @LPStart Encoding = omit
	.byte	3                       # @TType Encoding = udata4
	.asciz	"\234"                  # @TType base offset
	.byte	3                       # Call site Encoding = udata4
	.byte	26                      # Call site table length
	.long	.Ltmp50-.Lfunc_begin2   # >> Call Site 1 <<
	.long	.Ltmp51-.Ltmp50         #   Call between .Ltmp50 and .Ltmp51
	.long	.Ltmp52-.Lfunc_begin2   #     jumps to .Ltmp52
	.byte	0                       #   On action: cleanup
	.long	.Ltmp51-.Lfunc_begin2   # >> Call Site 2 <<
	.long	.Lfunc_end3-.Ltmp51     #   Call between .Ltmp51 and .Lfunc_end3
	.long	0                       #     has no landing pad
	.byte	0                       #   On action: cleanup
	.p2align	2

	.section	.text.__clang_call_terminate,"axG",@progbits,__clang_call_terminate,comdat
	.hidden	__clang_call_terminate
	.weak	__clang_call_terminate
	.p2align	4, 0x90
	.type	__clang_call_terminate,@function
__clang_call_terminate:                 # @__clang_call_terminate
# BB#0:
	pushq	%rax
	callq	__cxa_begin_catch
	callq	_ZSt9terminatev
.Lfunc_end4:
	.size	__clang_call_terminate, .Lfunc_end4-__clang_call_terminate

	.section	.text._ZSt32__enable_shared_from_this_helperILN9__gnu_cxx12_Lock_policyE2EEvRKSt14__shared_countIXT_EEz,"axG",@progbits,_ZSt32__enable_shared_from_this_helperILN9__gnu_cxx12_Lock_policyE2EEvRKSt14__shared_countIXT_EEz,comdat
	.weak	_ZSt32__enable_shared_from_this_helperILN9__gnu_cxx12_Lock_policyE2EEvRKSt14__shared_countIXT_EEz
	.p2align	4, 0x90
	.type	_ZSt32__enable_shared_from_this_helperILN9__gnu_cxx12_Lock_policyE2EEvRKSt14__shared_countIXT_EEz,@function
_ZSt32__enable_shared_from_this_helperILN9__gnu_cxx12_Lock_policyE2EEvRKSt14__shared_countIXT_EEz: # @_ZSt32__enable_shared_from_this_helperILN9__gnu_cxx12_Lock_policyE2EEvRKSt14__shared_countIXT_EEz
	.cfi_startproc
# BB#0:
	retq
.Lfunc_end5:
	.size	_ZSt32__enable_shared_from_this_helperILN9__gnu_cxx12_Lock_policyE2EEvRKSt14__shared_countIXT_EEz, .Lfunc_end5-_ZSt32__enable_shared_from_this_helperILN9__gnu_cxx12_Lock_policyE2EEvRKSt14__shared_countIXT_EEz
	.cfi_endproc

	.section	.text._ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED2Ev,"axG",@progbits,_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED2Ev,comdat
	.weak	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED2Ev
	.p2align	4, 0x90
	.type	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED2Ev,@function
_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED2Ev: # @_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED2Ev
	.cfi_startproc
# BB#0:
	retq
.Lfunc_end6:
	.size	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED2Ev, .Lfunc_end6-_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED2Ev
	.cfi_endproc

	.section	.text._ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED0Ev,"axG",@progbits,_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED0Ev,comdat
	.weak	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED0Ev
	.p2align	4, 0x90
	.type	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED0Ev,@function
_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED0Ev: # @_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED0Ev
	.cfi_startproc
# BB#0:
	jmp	_ZdlPv                  # TAILCALL
.Lfunc_end7:
	.size	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED0Ev, .Lfunc_end7-_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED0Ev
	.cfi_endproc

	.section	.text._ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv,"axG",@progbits,_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv,comdat
	.weak	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv
	.p2align	4, 0x90
	.type	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv,@function
_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv: # @_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv
	.cfi_startproc
# BB#0:
	addq	$16, %rdi
	jmp	_ZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiED2Ev # TAILCALL
.Lfunc_end8:
	.size	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv, .Lfunc_end8-_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv
	.cfi_endproc

	.section	.text._ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv,"axG",@progbits,_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv,comdat
	.weak	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv
	.p2align	4, 0x90
	.type	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv,@function
_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv: # @_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv
	.cfi_startproc
# BB#0:
	testq	%rdi, %rdi
	je	.LBB9_1
# BB#2:
	jmp	_ZdlPv                  # TAILCALL
.LBB9_1:
	retq
.Lfunc_end9:
	.size	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv, .Lfunc_end9-_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv
	.cfi_endproc

	.section	.text._ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info,"axG",@progbits,_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info,comdat
	.weak	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info
	.p2align	4, 0x90
	.type	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info,@function
_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info: # @_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info
	.cfi_startproc
# BB#0:
	pushq	%rbx
.Ltmp59:
	.cfi_def_cfa_offset 16
.Ltmp60:
	.cfi_offset %rbx, -16
	movq	%rdi, %rbx
	movq	8(%rsi), %rdi
	movl	$_ZTSSt19_Sp_make_shared_tag, %eax
	cmpq	%rax, %rdi
	je	.LBB10_3
# BB#1:
	xorl	%eax, %eax
	cmpb	$42, (%rdi)
	je	.LBB10_4
# BB#2:
	movl	$_ZTSSt19_Sp_make_shared_tag, %esi
	callq	strcmp
	movl	%eax, %ecx
	xorl	%eax, %eax
	testl	%ecx, %ecx
	jne	.LBB10_4
.LBB10_3:
	addq	$16, %rbx
	movq	%rbx, %rax
.LBB10_4:
	popq	%rbx
	retq
.Lfunc_end10:
	.size	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info, .Lfunc_end10-_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info
	.cfi_endproc

	.section	.text._ZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEC2EOS5_,"axG",@progbits,_ZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEC2EOS5_,comdat
	.weak	_ZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEC2EOS5_
	.p2align	4, 0x90
	.type	_ZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEC2EOS5_,@function
_ZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEC2EOS5_: # @_ZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEC2EOS5_
.Lfunc_begin3:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception3
# BB#0:
	pushq	%r15
.Ltmp79:
	.cfi_def_cfa_offset 16
	pushq	%r14
.Ltmp80:
	.cfi_def_cfa_offset 24
	pushq	%rbx
.Ltmp81:
	.cfi_def_cfa_offset 32
	subq	$16, %rsp
.Ltmp82:
	.cfi_def_cfa_offset 48
.Ltmp83:
	.cfi_offset %rbx, -32
.Ltmp84:
	.cfi_offset %r14, -24
.Ltmp85:
	.cfi_offset %r15, -16
	movq	%rsi, %r14
	movq	%rdi, %r15
	movq	$0, 8(%r15)
	movl	$0, 16(%r15)
	movb	$0, 20(%r15)
	movl	$0, 24(%r15)
	movq	$0, 32(%r15)
	movl	$0, 40(%r15)
	movq	$_ZTVNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEE+16, (%r15)
.Ltmp61:
	movl	$24, %edi
	callq	_Znwm
	movq	%rax, %rbx
.Ltmp62:
# BB#1:
.Ltmp64:
	movq	%rbx, %rdi
	callq	_ZNSt13__future_base12_Result_baseC2Ev
.Ltmp65:
# BB#2:
	movq	$_ZTVNSt13__future_base7_ResultIiEE+16, (%rbx)
	movb	$0, 20(%rbx)
	movq	%rbx, 48(%r15)
	movq	(%r14), %rax
	movq	%rax, 56(%r15)
	movq	$0, 8(%rsp)
.Ltmp67:
	movl	$16, %edi
	callq	_Znwm
.Ltmp68:
# BB#3:
	movq	$_ZTVNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEEE+16, (%rax)
	movq	%r15, 8(%rax)
	movq	%rax, (%rsp)
.Ltmp70:
	leaq	8(%rsp), %rdi
	leaq	(%rsp), %rsi
	movl	$pthread_create, %edx
	callq	_ZNSt6thread15_M_start_threadESt10unique_ptrINS_6_StateESt14default_deleteIS1_EEPFvvE
.Ltmp71:
# BB#4:
	movq	(%rsp), %rdi
	testq	%rdi, %rdi
	je	.LBB11_6
# BB#5:
	movq	(%rdi), %rax
	callq	*8(%rax)
.LBB11_6:
	cmpq	$0, 32(%r15)
	jne	.LBB11_7
# BB#20:
	movq	8(%rsp), %rax
	movq	%rax, 32(%r15)
	addq	$16, %rsp
	popq	%rbx
	popq	%r14
	popq	%r15
	retq
.LBB11_7:
	callq	_ZSt9terminatev
.LBB11_8:
.Ltmp72:
	movq	%rax, %r14
	movq	(%rsp), %rdi
	testq	%rdi, %rdi
	je	.LBB11_11
# BB#9:
	movq	(%rdi), %rax
	callq	*8(%rax)
	jmp	.LBB11_11
.LBB11_10:
.Ltmp69:
	movq	%rax, %r14
.LBB11_11:
	movq	48(%r15), %rdi
	testq	%rdi, %rdi
	je	.LBB11_13
# BB#12:
	movq	(%rdi), %rax
	movq	(%rax), %rax
.Ltmp73:
	callq	*%rax
.Ltmp74:
.LBB11_13:
	movq	$0, 48(%r15)
	cmpq	$0, 32(%r15)
	movq	$_ZTVNSt13__future_base21_Async_state_commonV2E+16, (%r15)
	je	.LBB11_16
# BB#14:
	callq	_ZSt9terminatev
.LBB11_21:
.Ltmp75:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.LBB11_19:
.Ltmp66:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.LBB11_15:
.Ltmp63:
	movq	%rax, %r14
	movq	$_ZTVNSt13__future_base21_Async_state_commonV2E+16, (%r15)
.LBB11_16:
	movq	$_ZTVNSt13__future_base13_State_baseV2E+16, (%r15)
	movq	8(%r15), %rdi
	testq	%rdi, %rdi
	je	.LBB11_18
# BB#17:
	movq	(%rdi), %rax
	movq	(%rax), %rax
.Ltmp76:
	callq	*%rax
.Ltmp77:
.LBB11_18:
	movq	$0, 8(%r15)
	movq	%r14, %rdi
	callq	_Unwind_Resume
.LBB11_22:
.Ltmp78:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.Lfunc_end11:
	.size	_ZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEC2EOS5_, .Lfunc_end11-_ZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEC2EOS5_
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table11:
.Lexception3:
	.byte	255                     # @LPStart Encoding = omit
	.byte	3                       # @TType Encoding = udata4
	.byte	125                     # @TType base offset
	.byte	3                       # Call site Encoding = udata4
	.byte	117                     # Call site table length
	.long	.Ltmp61-.Lfunc_begin3   # >> Call Site 1 <<
	.long	.Ltmp62-.Ltmp61         #   Call between .Ltmp61 and .Ltmp62
	.long	.Ltmp63-.Lfunc_begin3   #     jumps to .Ltmp63
	.byte	0                       #   On action: cleanup
	.long	.Ltmp64-.Lfunc_begin3   # >> Call Site 2 <<
	.long	.Ltmp65-.Ltmp64         #   Call between .Ltmp64 and .Ltmp65
	.long	.Ltmp66-.Lfunc_begin3   #     jumps to .Ltmp66
	.byte	1                       #   On action: 1
	.long	.Ltmp67-.Lfunc_begin3   # >> Call Site 3 <<
	.long	.Ltmp68-.Ltmp67         #   Call between .Ltmp67 and .Ltmp68
	.long	.Ltmp69-.Lfunc_begin3   #     jumps to .Ltmp69
	.byte	0                       #   On action: cleanup
	.long	.Ltmp70-.Lfunc_begin3   # >> Call Site 4 <<
	.long	.Ltmp71-.Ltmp70         #   Call between .Ltmp70 and .Ltmp71
	.long	.Ltmp72-.Lfunc_begin3   #     jumps to .Ltmp72
	.byte	0                       #   On action: cleanup
	.long	.Ltmp71-.Lfunc_begin3   # >> Call Site 5 <<
	.long	.Ltmp73-.Ltmp71         #   Call between .Ltmp71 and .Ltmp73
	.long	0                       #     has no landing pad
	.byte	0                       #   On action: cleanup
	.long	.Ltmp73-.Lfunc_begin3   # >> Call Site 6 <<
	.long	.Ltmp74-.Ltmp73         #   Call between .Ltmp73 and .Ltmp74
	.long	.Ltmp75-.Lfunc_begin3   #     jumps to .Ltmp75
	.byte	1                       #   On action: 1
	.long	.Ltmp74-.Lfunc_begin3   # >> Call Site 7 <<
	.long	.Ltmp76-.Ltmp74         #   Call between .Ltmp74 and .Ltmp76
	.long	0                       #     has no landing pad
	.byte	0                       #   On action: cleanup
	.long	.Ltmp76-.Lfunc_begin3   # >> Call Site 8 <<
	.long	.Ltmp77-.Ltmp76         #   Call between .Ltmp76 and .Ltmp77
	.long	.Ltmp78-.Lfunc_begin3   #     jumps to .Ltmp78
	.byte	1                       #   On action: 1
	.long	.Ltmp77-.Lfunc_begin3   # >> Call Site 9 <<
	.long	.Lfunc_end11-.Ltmp77    #   Call between .Ltmp77 and .Lfunc_end11
	.long	0                       #     has no landing pad
	.byte	0                       #   On action: cleanup
	.byte	1                       # >> Action Record 1 <<
                                        #   Catch TypeInfo 1
	.byte	0                       #   No further actions
                                        # >> Catch TypeInfos <<
	.long	0                       # TypeInfo 1
	.p2align	2

	.section	.text._ZNSt10unique_ptrINSt13__future_base7_ResultIiEENS0_12_Result_base8_DeleterEED2Ev,"axG",@progbits,_ZNSt10unique_ptrINSt13__future_base7_ResultIiEENS0_12_Result_base8_DeleterEED2Ev,comdat
	.weak	_ZNSt10unique_ptrINSt13__future_base7_ResultIiEENS0_12_Result_base8_DeleterEED2Ev
	.p2align	4, 0x90
	.type	_ZNSt10unique_ptrINSt13__future_base7_ResultIiEENS0_12_Result_base8_DeleterEED2Ev,@function
_ZNSt10unique_ptrINSt13__future_base7_ResultIiEENS0_12_Result_base8_DeleterEED2Ev: # @_ZNSt10unique_ptrINSt13__future_base7_ResultIiEENS0_12_Result_base8_DeleterEED2Ev
.Lfunc_begin4:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception4
# BB#0:
	pushq	%rbx
.Ltmp89:
	.cfi_def_cfa_offset 16
.Ltmp90:
	.cfi_offset %rbx, -16
	movq	%rdi, %rbx
	movq	(%rbx), %rdi
	testq	%rdi, %rdi
	je	.LBB12_2
# BB#1:
	movq	(%rdi), %rax
	movq	(%rax), %rax
.Ltmp86:
	callq	*%rax
.Ltmp87:
.LBB12_2:
	movq	$0, (%rbx)
	popq	%rbx
	retq
.LBB12_3:
.Ltmp88:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.Lfunc_end12:
	.size	_ZNSt10unique_ptrINSt13__future_base7_ResultIiEENS0_12_Result_base8_DeleterEED2Ev, .Lfunc_end12-_ZNSt10unique_ptrINSt13__future_base7_ResultIiEENS0_12_Result_base8_DeleterEED2Ev
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table12:
.Lexception4:
	.byte	255                     # @LPStart Encoding = omit
	.byte	3                       # @TType Encoding = udata4
	.byte	21                      # @TType base offset
	.byte	3                       # Call site Encoding = udata4
	.byte	13                      # Call site table length
	.long	.Ltmp86-.Lfunc_begin4   # >> Call Site 1 <<
	.long	.Ltmp87-.Ltmp86         #   Call between .Ltmp86 and .Ltmp87
	.long	.Ltmp88-.Lfunc_begin4   #     jumps to .Ltmp88
	.byte	1                       #   On action: 1
	.byte	1                       # >> Action Record 1 <<
                                        #   Catch TypeInfo 1
	.byte	0                       #   No further actions
                                        # >> Catch TypeInfos <<
	.long	0                       # TypeInfo 1
	.p2align	2

	.section	.text._ZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiED2Ev,"axG",@progbits,_ZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiED2Ev,comdat
	.weak	_ZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiED2Ev
	.p2align	4, 0x90
	.type	_ZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiED2Ev,@function
_ZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiED2Ev: # @_ZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiED2Ev
.Lfunc_begin5:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception5
# BB#0:
	pushq	%r14
.Ltmp100:
	.cfi_def_cfa_offset 16
	pushq	%rbx
.Ltmp101:
	.cfi_def_cfa_offset 24
	pushq	%rax
.Ltmp102:
	.cfi_def_cfa_offset 32
.Ltmp103:
	.cfi_offset %rbx, -24
.Ltmp104:
	.cfi_offset %r14, -16
	movq	%rdi, %rbx
	movq	$_ZTVNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEE+16, (%rbx)
	cmpq	$0, 32(%rbx)
	je	.LBB13_2
# BB#1:
	leaq	32(%rbx), %rdi
.Ltmp91:
	callq	_ZNSt6thread4joinEv
.Ltmp92:
.LBB13_2:
	movq	48(%rbx), %rdi
	testq	%rdi, %rdi
	je	.LBB13_4
# BB#3:
	movq	(%rdi), %rax
	movq	(%rax), %rax
.Ltmp94:
	callq	*%rax
.Ltmp95:
.LBB13_4:
	movq	$0, 48(%rbx)
	movq	$_ZTVNSt13__future_base21_Async_state_commonV2E+16, (%rbx)
	cmpq	$0, 32(%rbx)
	jne	.LBB13_10
# BB#5:
	movq	$_ZTVNSt13__future_base13_State_baseV2E+16, (%rbx)
	movq	8(%rbx), %rdi
	testq	%rdi, %rdi
	je	.LBB13_7
# BB#6:
	movq	(%rdi), %rax
	movq	(%rax), %rax
.Ltmp97:
	callq	*%rax
.Ltmp98:
.LBB13_7:
	movq	$0, 8(%rbx)
	addq	$8, %rsp
	popq	%rbx
	popq	%r14
	retq
.LBB13_10:
	callq	_ZSt9terminatev
.LBB13_11:
.Ltmp99:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.LBB13_9:
.Ltmp96:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.LBB13_8:
.Ltmp93:
	movq	%rax, %r14
	leaq	48(%rbx), %rdi
	callq	_ZNSt10unique_ptrINSt13__future_base7_ResultIiEENS0_12_Result_base8_DeleterEED2Ev
	movq	%rbx, %rdi
	callq	_ZNSt13__future_base21_Async_state_commonV2D2Ev
	movq	%r14, %rdi
	callq	__clang_call_terminate
.Lfunc_end13:
	.size	_ZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiED2Ev, .Lfunc_end13-_ZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiED2Ev
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table13:
.Lexception5:
	.byte	255                     # @LPStart Encoding = omit
	.byte	3                       # @TType Encoding = udata4
	.asciz	"\274"                  # @TType base offset
	.byte	3                       # Call site Encoding = udata4
	.byte	52                      # Call site table length
	.long	.Ltmp91-.Lfunc_begin5   # >> Call Site 1 <<
	.long	.Ltmp92-.Ltmp91         #   Call between .Ltmp91 and .Ltmp92
	.long	.Ltmp93-.Lfunc_begin5   #     jumps to .Ltmp93
	.byte	1                       #   On action: 1
	.long	.Ltmp94-.Lfunc_begin5   # >> Call Site 2 <<
	.long	.Ltmp95-.Ltmp94         #   Call between .Ltmp94 and .Ltmp95
	.long	.Ltmp96-.Lfunc_begin5   #     jumps to .Ltmp96
	.byte	1                       #   On action: 1
	.long	.Ltmp97-.Lfunc_begin5   # >> Call Site 3 <<
	.long	.Ltmp98-.Ltmp97         #   Call between .Ltmp97 and .Ltmp98
	.long	.Ltmp99-.Lfunc_begin5   #     jumps to .Ltmp99
	.byte	1                       #   On action: 1
	.long	.Ltmp98-.Lfunc_begin5   # >> Call Site 4 <<
	.long	.Lfunc_end13-.Ltmp98    #   Call between .Ltmp98 and .Lfunc_end13
	.long	0                       #     has no landing pad
	.byte	0                       #   On action: cleanup
	.byte	1                       # >> Action Record 1 <<
                                        #   Catch TypeInfo 1
	.byte	0                       #   No further actions
                                        # >> Catch TypeInfos <<
	.long	0                       # TypeInfo 1
	.p2align	2

	.section	.text._ZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiED0Ev,"axG",@progbits,_ZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiED0Ev,comdat
	.weak	_ZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiED0Ev
	.p2align	4, 0x90
	.type	_ZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiED0Ev,@function
_ZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiED0Ev: # @_ZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiED0Ev
	.cfi_startproc
# BB#0:
	pushq	%rbx
.Ltmp105:
	.cfi_def_cfa_offset 16
.Ltmp106:
	.cfi_offset %rbx, -16
	movq	%rdi, %rbx
	callq	_ZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiED2Ev
	movq	%rbx, %rdi
	popq	%rbx
	jmp	_ZdlPv                  # TAILCALL
.Lfunc_end14:
	.size	_ZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiED0Ev, .Lfunc_end14-_ZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiED0Ev
	.cfi_endproc

	.section	.text._ZNSt13__future_base21_Async_state_commonV217_M_complete_asyncEv,"axG",@progbits,_ZNSt13__future_base21_Async_state_commonV217_M_complete_asyncEv,comdat
	.weak	_ZNSt13__future_base21_Async_state_commonV217_M_complete_asyncEv
	.p2align	4, 0x90
	.type	_ZNSt13__future_base21_Async_state_commonV217_M_complete_asyncEv,@function
_ZNSt13__future_base21_Async_state_commonV217_M_complete_asyncEv: # @_ZNSt13__future_base21_Async_state_commonV217_M_complete_asyncEv
	.cfi_startproc
# BB#0:
	subq	$24, %rsp
.Ltmp107:
	.cfi_def_cfa_offset 32
	leaq	32(%rdi), %rax
	movq	%rax, (%rsp)
	movl	$_ZNSt6thread4joinEv, %eax
	movd	%rax, %xmm0
	movups	%xmm0, 8(%rsp)
	movq	_ZSt15__once_callable@GOTTPOFF(%rip), %rax
	leaq	(%rsp), %rcx
	movq	%rcx, %fs:(%rax)
	movq	_ZSt11__once_call@GOTTPOFF(%rip), %rax
	movq	$_ZSt16__once_call_implISt12_Bind_simpleIFSt7_Mem_fnIMSt6threadFvvEEPS2_EEEvv, %fs:(%rax)
	movl	$-1, %eax
	movl	$__pthread_key_create, %ecx
	testq	%rcx, %rcx
	je	.LBB15_3
# BB#1:
	addq	$40, %rdi
	movl	$__once_proxy, %esi
	callq	pthread_once
	testl	%eax, %eax
	jne	.LBB15_3
# BB#2:
	addq	$24, %rsp
	retq
.LBB15_3:
	movl	%eax, %edi
	callq	_ZSt20__throw_system_errori
.Lfunc_end15:
	.size	_ZNSt13__future_base21_Async_state_commonV217_M_complete_asyncEv, .Lfunc_end15-_ZNSt13__future_base21_Async_state_commonV217_M_complete_asyncEv
	.cfi_endproc

	.section	.text._ZNKSt13__future_base13_State_baseV221_M_is_deferred_futureEv,"axG",@progbits,_ZNKSt13__future_base13_State_baseV221_M_is_deferred_futureEv,comdat
	.weak	_ZNKSt13__future_base13_State_baseV221_M_is_deferred_futureEv
	.p2align	4, 0x90
	.type	_ZNKSt13__future_base13_State_baseV221_M_is_deferred_futureEv,@function
_ZNKSt13__future_base13_State_baseV221_M_is_deferred_futureEv: # @_ZNKSt13__future_base13_State_baseV221_M_is_deferred_futureEv
	.cfi_startproc
# BB#0:
	xorl	%eax, %eax
	retq
.Lfunc_end16:
	.size	_ZNKSt13__future_base13_State_baseV221_M_is_deferred_futureEv, .Lfunc_end16-_ZNKSt13__future_base13_State_baseV221_M_is_deferred_futureEv
	.cfi_endproc

	.section	.text._ZNSt13__future_base21_Async_state_commonV2D2Ev,"axG",@progbits,_ZNSt13__future_base21_Async_state_commonV2D2Ev,comdat
	.weak	_ZNSt13__future_base21_Async_state_commonV2D2Ev
	.p2align	4, 0x90
	.type	_ZNSt13__future_base21_Async_state_commonV2D2Ev,@function
_ZNSt13__future_base21_Async_state_commonV2D2Ev: # @_ZNSt13__future_base21_Async_state_commonV2D2Ev
.Lfunc_begin6:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception6
# BB#0:
	pushq	%rbx
.Ltmp111:
	.cfi_def_cfa_offset 16
.Ltmp112:
	.cfi_offset %rbx, -16
	movq	%rdi, %rbx
	movq	$_ZTVNSt13__future_base21_Async_state_commonV2E+16, (%rbx)
	cmpq	$0, 32(%rbx)
	jne	.LBB17_4
# BB#1:
	movq	$_ZTVNSt13__future_base13_State_baseV2E+16, (%rbx)
	movq	8(%rbx), %rdi
	testq	%rdi, %rdi
	je	.LBB17_3
# BB#2:
	movq	(%rdi), %rax
	movq	(%rax), %rax
.Ltmp108:
	callq	*%rax
.Ltmp109:
.LBB17_3:
	movq	$0, 8(%rbx)
	popq	%rbx
	retq
.LBB17_4:
	callq	_ZSt9terminatev
.LBB17_5:
.Ltmp110:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.Lfunc_end17:
	.size	_ZNSt13__future_base21_Async_state_commonV2D2Ev, .Lfunc_end17-_ZNSt13__future_base21_Async_state_commonV2D2Ev
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table17:
.Lexception6:
	.byte	255                     # @LPStart Encoding = omit
	.byte	3                       # @TType Encoding = udata4
	.asciz	"\242\200\200"          # @TType base offset
	.byte	3                       # Call site Encoding = udata4
	.byte	26                      # Call site table length
	.long	.Ltmp108-.Lfunc_begin6  # >> Call Site 1 <<
	.long	.Ltmp109-.Ltmp108       #   Call between .Ltmp108 and .Ltmp109
	.long	.Ltmp110-.Lfunc_begin6  #     jumps to .Ltmp110
	.byte	1                       #   On action: 1
	.long	.Ltmp109-.Lfunc_begin6  # >> Call Site 2 <<
	.long	.Lfunc_end17-.Ltmp109   #   Call between .Ltmp109 and .Lfunc_end17
	.long	0                       #     has no landing pad
	.byte	0                       #   On action: cleanup
	.byte	1                       # >> Action Record 1 <<
                                        #   Catch TypeInfo 1
	.byte	0                       #   No further actions
                                        # >> Catch TypeInfos <<
	.long	0                       # TypeInfo 1
	.p2align	2

	.section	.text._ZNSt13__future_base21_Async_state_commonV2D0Ev,"axG",@progbits,_ZNSt13__future_base21_Async_state_commonV2D0Ev,comdat
	.weak	_ZNSt13__future_base21_Async_state_commonV2D0Ev
	.p2align	4, 0x90
	.type	_ZNSt13__future_base21_Async_state_commonV2D0Ev,@function
_ZNSt13__future_base21_Async_state_commonV2D0Ev: # @_ZNSt13__future_base21_Async_state_commonV2D0Ev
.Lfunc_begin7:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception7
# BB#0:
	pushq	%rbx
.Ltmp116:
	.cfi_def_cfa_offset 16
.Ltmp117:
	.cfi_offset %rbx, -16
	movq	%rdi, %rbx
	movq	$_ZTVNSt13__future_base21_Async_state_commonV2E+16, (%rbx)
	cmpq	$0, 32(%rbx)
	jne	.LBB18_4
# BB#1:
	movq	$_ZTVNSt13__future_base13_State_baseV2E+16, (%rbx)
	movq	8(%rbx), %rdi
	testq	%rdi, %rdi
	je	.LBB18_3
# BB#2:
	movq	(%rdi), %rax
	movq	(%rax), %rax
.Ltmp113:
	callq	*%rax
.Ltmp114:
.LBB18_3:
	movq	%rbx, %rdi
	popq	%rbx
	jmp	_ZdlPv                  # TAILCALL
.LBB18_4:
	callq	_ZSt9terminatev
.LBB18_5:
.Ltmp115:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.Lfunc_end18:
	.size	_ZNSt13__future_base21_Async_state_commonV2D0Ev, .Lfunc_end18-_ZNSt13__future_base21_Async_state_commonV2D0Ev
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table18:
.Lexception7:
	.byte	255                     # @LPStart Encoding = omit
	.byte	3                       # @TType Encoding = udata4
	.asciz	"\242\200\200"          # @TType base offset
	.byte	3                       # Call site Encoding = udata4
	.byte	26                      # Call site table length
	.long	.Ltmp113-.Lfunc_begin7  # >> Call Site 1 <<
	.long	.Ltmp114-.Ltmp113       #   Call between .Ltmp113 and .Ltmp114
	.long	.Ltmp115-.Lfunc_begin7  #     jumps to .Ltmp115
	.byte	1                       #   On action: 1
	.long	.Ltmp114-.Lfunc_begin7  # >> Call Site 2 <<
	.long	.Lfunc_end18-.Ltmp114   #   Call between .Ltmp114 and .Lfunc_end18
	.long	0                       #     has no landing pad
	.byte	0                       #   On action: cleanup
	.byte	1                       # >> Action Record 1 <<
                                        #   Catch TypeInfo 1
	.byte	0                       #   No further actions
                                        # >> Catch TypeInfos <<
	.long	0                       # TypeInfo 1
	.p2align	2

	.section	.text._ZNSt13__future_base13_State_baseV2D2Ev,"axG",@progbits,_ZNSt13__future_base13_State_baseV2D2Ev,comdat
	.weak	_ZNSt13__future_base13_State_baseV2D2Ev
	.p2align	4, 0x90
	.type	_ZNSt13__future_base13_State_baseV2D2Ev,@function
_ZNSt13__future_base13_State_baseV2D2Ev: # @_ZNSt13__future_base13_State_baseV2D2Ev
.Lfunc_begin8:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception8
# BB#0:
	pushq	%rbx
.Ltmp121:
	.cfi_def_cfa_offset 16
.Ltmp122:
	.cfi_offset %rbx, -16
	movq	%rdi, %rbx
	movq	$_ZTVNSt13__future_base13_State_baseV2E+16, (%rbx)
	movq	8(%rbx), %rdi
	testq	%rdi, %rdi
	je	.LBB19_2
# BB#1:
	movq	(%rdi), %rax
	movq	(%rax), %rax
.Ltmp118:
	callq	*%rax
.Ltmp119:
.LBB19_2:
	movq	$0, 8(%rbx)
	popq	%rbx
	retq
.LBB19_3:
.Ltmp120:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.Lfunc_end19:
	.size	_ZNSt13__future_base13_State_baseV2D2Ev, .Lfunc_end19-_ZNSt13__future_base13_State_baseV2D2Ev
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table19:
.Lexception8:
	.byte	255                     # @LPStart Encoding = omit
	.byte	3                       # @TType Encoding = udata4
	.byte	21                      # @TType base offset
	.byte	3                       # Call site Encoding = udata4
	.byte	13                      # Call site table length
	.long	.Ltmp118-.Lfunc_begin8  # >> Call Site 1 <<
	.long	.Ltmp119-.Ltmp118       #   Call between .Ltmp118 and .Ltmp119
	.long	.Ltmp120-.Lfunc_begin8  #     jumps to .Ltmp120
	.byte	1                       #   On action: 1
	.byte	1                       # >> Action Record 1 <<
                                        #   Catch TypeInfo 1
	.byte	0                       #   No further actions
                                        # >> Catch TypeInfos <<
	.long	0                       # TypeInfo 1
	.p2align	2

	.section	.text._ZNSt13__future_base13_State_baseV2D0Ev,"axG",@progbits,_ZNSt13__future_base13_State_baseV2D0Ev,comdat
	.weak	_ZNSt13__future_base13_State_baseV2D0Ev
	.p2align	4, 0x90
	.type	_ZNSt13__future_base13_State_baseV2D0Ev,@function
_ZNSt13__future_base13_State_baseV2D0Ev: # @_ZNSt13__future_base13_State_baseV2D0Ev
.Lfunc_begin9:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception9
# BB#0:
	pushq	%rbx
.Ltmp126:
	.cfi_def_cfa_offset 16
.Ltmp127:
	.cfi_offset %rbx, -16
	movq	%rdi, %rbx
	movq	$_ZTVNSt13__future_base13_State_baseV2E+16, (%rbx)
	movq	8(%rbx), %rdi
	testq	%rdi, %rdi
	je	.LBB20_2
# BB#1:
	movq	(%rdi), %rax
	movq	(%rax), %rax
.Ltmp123:
	callq	*%rax
.Ltmp124:
.LBB20_2:
	movq	%rbx, %rdi
	popq	%rbx
	jmp	_ZdlPv                  # TAILCALL
.LBB20_3:
.Ltmp125:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.Lfunc_end20:
	.size	_ZNSt13__future_base13_State_baseV2D0Ev, .Lfunc_end20-_ZNSt13__future_base13_State_baseV2D0Ev
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table20:
.Lexception9:
	.byte	255                     # @LPStart Encoding = omit
	.byte	3                       # @TType Encoding = udata4
	.byte	21                      # @TType base offset
	.byte	3                       # Call site Encoding = udata4
	.byte	13                      # Call site table length
	.long	.Ltmp123-.Lfunc_begin9  # >> Call Site 1 <<
	.long	.Ltmp124-.Ltmp123       #   Call between .Ltmp123 and .Ltmp124
	.long	.Ltmp125-.Lfunc_begin9  #     jumps to .Ltmp125
	.byte	1                       #   On action: 1
	.byte	1                       # >> Action Record 1 <<
                                        #   Catch TypeInfo 1
	.byte	0                       #   No further actions
                                        # >> Catch TypeInfos <<
	.long	0                       # TypeInfo 1
	.p2align	2

	.section	.text._ZNSt13__future_base13_State_baseV217_M_complete_asyncEv,"axG",@progbits,_ZNSt13__future_base13_State_baseV217_M_complete_asyncEv,comdat
	.weak	_ZNSt13__future_base13_State_baseV217_M_complete_asyncEv
	.p2align	4, 0x90
	.type	_ZNSt13__future_base13_State_baseV217_M_complete_asyncEv,@function
_ZNSt13__future_base13_State_baseV217_M_complete_asyncEv: # @_ZNSt13__future_base13_State_baseV217_M_complete_asyncEv
	.cfi_startproc
# BB#0:
	retq
.Lfunc_end21:
	.size	_ZNSt13__future_base13_State_baseV217_M_complete_asyncEv, .Lfunc_end21-_ZNSt13__future_base13_State_baseV217_M_complete_asyncEv
	.cfi_endproc

	.section	.text._ZNSt13__future_base7_ResultIiE10_M_destroyEv,"axG",@progbits,_ZNSt13__future_base7_ResultIiE10_M_destroyEv,comdat
	.weak	_ZNSt13__future_base7_ResultIiE10_M_destroyEv
	.p2align	4, 0x90
	.type	_ZNSt13__future_base7_ResultIiE10_M_destroyEv,@function
_ZNSt13__future_base7_ResultIiE10_M_destroyEv: # @_ZNSt13__future_base7_ResultIiE10_M_destroyEv
	.cfi_startproc
# BB#0:
	testq	%rdi, %rdi
	je	.LBB22_1
# BB#2:
	movq	(%rdi), %rax
	jmpq	*16(%rax)               # TAILCALL
.LBB22_1:
	retq
.Lfunc_end22:
	.size	_ZNSt13__future_base7_ResultIiE10_M_destroyEv, .Lfunc_end22-_ZNSt13__future_base7_ResultIiE10_M_destroyEv
	.cfi_endproc

	.section	.text._ZNSt13__future_base7_ResultIiED2Ev,"axG",@progbits,_ZNSt13__future_base7_ResultIiED2Ev,comdat
	.weak	_ZNSt13__future_base7_ResultIiED2Ev
	.p2align	4, 0x90
	.type	_ZNSt13__future_base7_ResultIiED2Ev,@function
_ZNSt13__future_base7_ResultIiED2Ev:    # @_ZNSt13__future_base7_ResultIiED2Ev
	.cfi_startproc
# BB#0:
	movq	$_ZTVNSt13__future_base7_ResultIiEE+16, (%rdi)
	jmp	_ZNSt13__future_base12_Result_baseD2Ev # TAILCALL
.Lfunc_end23:
	.size	_ZNSt13__future_base7_ResultIiED2Ev, .Lfunc_end23-_ZNSt13__future_base7_ResultIiED2Ev
	.cfi_endproc

	.section	.text._ZNSt13__future_base7_ResultIiED0Ev,"axG",@progbits,_ZNSt13__future_base7_ResultIiED0Ev,comdat
	.weak	_ZNSt13__future_base7_ResultIiED0Ev
	.p2align	4, 0x90
	.type	_ZNSt13__future_base7_ResultIiED0Ev,@function
_ZNSt13__future_base7_ResultIiED0Ev:    # @_ZNSt13__future_base7_ResultIiED0Ev
	.cfi_startproc
# BB#0:
	pushq	%rbx
.Ltmp128:
	.cfi_def_cfa_offset 16
.Ltmp129:
	.cfi_offset %rbx, -16
	movq	%rdi, %rbx
	movq	$_ZTVNSt13__future_base7_ResultIiEE+16, (%rbx)
	callq	_ZNSt13__future_base12_Result_baseD2Ev
	movq	%rbx, %rdi
	popq	%rbx
	jmp	_ZdlPv                  # TAILCALL
.Lfunc_end24:
	.size	_ZNSt13__future_base7_ResultIiED0Ev, .Lfunc_end24-_ZNSt13__future_base7_ResultIiED0Ev
	.cfi_endproc

	.section	.text._ZNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEED0Ev,"axG",@progbits,_ZNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEED0Ev,comdat
	.weak	_ZNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEED0Ev
	.p2align	4, 0x90
	.type	_ZNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEED0Ev,@function
_ZNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEED0Ev: # @_ZNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEED0Ev
	.cfi_startproc
# BB#0:
	pushq	%rbx
.Ltmp130:
	.cfi_def_cfa_offset 16
.Ltmp131:
	.cfi_offset %rbx, -16
	movq	%rdi, %rbx
	callq	_ZNSt6thread6_StateD2Ev
	movq	%rbx, %rdi
	popq	%rbx
	jmp	_ZdlPv                  # TAILCALL
.Lfunc_end25:
	.size	_ZNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEED0Ev, .Lfunc_end25-_ZNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEED0Ev
	.cfi_endproc

	.section	.text._ZNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEE6_M_runEv,"axG",@progbits,_ZNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEE6_M_runEv,comdat
	.weak	_ZNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEE6_M_runEv
	.p2align	4, 0x90
	.type	_ZNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEE6_M_runEv,@function
_ZNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEE6_M_runEv: # @_ZNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEE6_M_runEv
	.cfi_startproc
# BB#0:
	addq	$8, %rdi
	jmp	_ZZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEC1EOS5_ENKUlvE_clEv # TAILCALL
.Lfunc_end26:
	.size	_ZNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEE6_M_runEv, .Lfunc_end26-_ZNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEE6_M_runEv
	.cfi_endproc

	.section	.text._ZZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEC1EOS5_ENKUlvE_clEv,"axG",@progbits,_ZZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEC1EOS5_ENKUlvE_clEv,comdat
	.weak	_ZZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEC1EOS5_ENKUlvE_clEv
	.p2align	4, 0x90
	.type	_ZZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEC1EOS5_ENKUlvE_clEv,@function
_ZZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEC1EOS5_ENKUlvE_clEv: # @_ZZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEC1EOS5_ENKUlvE_clEv
.Lfunc_begin10:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception10
# BB#0:
	pushq	%r15
.Ltmp162:
	.cfi_def_cfa_offset 16
	pushq	%r14
.Ltmp163:
	.cfi_def_cfa_offset 24
	pushq	%r12
.Ltmp164:
	.cfi_def_cfa_offset 32
	pushq	%rbx
.Ltmp165:
	.cfi_def_cfa_offset 40
	subq	$88, %rsp
.Ltmp166:
	.cfi_def_cfa_offset 128
.Ltmp167:
	.cfi_offset %rbx, -40
.Ltmp168:
	.cfi_offset %r12, -32
.Ltmp169:
	.cfi_offset %r14, -24
.Ltmp170:
	.cfi_offset %r15, -16
	movq	(%rdi), %rbx
	leaq	48(%rbx), %r12
	leaq	56(%rbx), %rax
	movq	%r12, 16(%rsp)
	movq	%rax, 24(%rsp)
	movq	$_ZNSt17_Function_handlerIFSt10unique_ptrINSt13__future_base12_Result_baseENS2_8_DeleterEEvENS1_12_Task_setterIS0_INS1_7_ResultIiEES3_ESt12_Bind_simpleIFPFivEvEEiEEE9_M_invokeERKSt9_Any_data, 40(%rsp)
	movq	$_ZNSt14_Function_base13_Base_managerINSt13__future_base12_Task_setterISt10unique_ptrINS1_7_ResultIiEENS1_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEEE10_M_managerERSt9_Any_dataRKSG_St18_Manager_operation, 32(%rsp)
	movb	$0, 7(%rsp)
	leaq	7(%rsp), %rax
	movq	%rax, 48(%rsp)
	leaq	16(%rsp), %rax
	movq	%rax, 56(%rsp)
	movq	%rbx, 64(%rsp)
	movl	$_ZNSt13__future_base13_State_baseV29_M_do_setEPSt8functionIFSt10unique_ptrINS_12_Result_baseENS3_8_DeleterEEvEEPb, %eax
	movd	%rax, %xmm0
	movups	%xmm0, 72(%rsp)
	movq	_ZSt15__once_callable@GOTTPOFF(%rip), %rax
	leaq	48(%rsp), %rcx
	movq	%rcx, %fs:(%rax)
	movq	_ZSt11__once_call@GOTTPOFF(%rip), %rax
	movq	$_ZSt16__once_call_implISt12_Bind_simpleIFSt7_Mem_fnIMNSt13__future_base13_State_baseV2EFvPSt8functionIFSt10unique_ptrINS2_12_Result_baseENS6_8_DeleterEEvEEPbEEPS3_SB_SC_EEEvv, %fs:(%rax)
	movl	$-1, %eax
	movl	$__pthread_key_create, %ecx
	testq	%rcx, %rcx
	je	.LBB27_3
# BB#1:
	leaq	24(%rbx), %rdi
.Ltmp132:
	movl	$__once_proxy, %esi
	callq	pthread_once
.Ltmp133:
# BB#2:
	testl	%eax, %eax
	jne	.LBB27_3
# BB#5:
	cmpb	$0, 7(%rsp)
	je	.LBB27_11
# BB#6:
	movl	$1, %eax
	xchgl	%eax, 16(%rbx)
	testl	%eax, %eax
	jns	.LBB27_8
# BB#7:
	leaq	16(%rbx), %rdi
.Ltmp136:
	callq	_ZNSt28__atomic_futex_unsigned_base19_M_futex_notify_allEPj
.Ltmp137:
.LBB27_8:
	movq	32(%rsp), %rax
	testq	%rax, %rax
	je	.LBB27_10
# BB#9:
.Ltmp138:
	leaq	16(%rsp), %rdi
	movl	$3, %edx
	movq	%rdi, %rsi
	callq	*%rax
.Ltmp139:
.LBB27_10:
	addq	$88, %rsp
	popq	%rbx
	popq	%r12
	popq	%r14
	popq	%r15
	retq
.LBB27_3:
.Ltmp134:
	movl	%eax, %edi
	callq	_ZSt20__throw_system_errori
.Ltmp135:
# BB#4:
.LBB27_11:
.Ltmp141:
	movl	$2, %edi
	callq	_ZSt20__throw_future_errori
.Ltmp142:
# BB#12:
.LBB27_13:
.Ltmp140:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.LBB27_14:
.Ltmp143:
	movq	%rdx, %r15
	movq	%rax, %r14
	movq	32(%rsp), %rax
	testq	%rax, %rax
	je	.LBB27_16
# BB#15:
.Ltmp144:
	leaq	16(%rsp), %rdi
	movl	$3, %edx
	movq	%rdi, %rsi
	callq	*%rax
.Ltmp145:
.LBB27_16:
	cmpl	$2, %r15d
	jne	.LBB27_28
# BB#17:
	movq	%r14, %rdi
	callq	__cxa_begin_catch
	movq	(%r12), %rax
	testq	%rax, %rax
	je	.LBB27_22
# BB#18:
	movq	$0, 48(%rbx)
	movq	%rax, 8(%rsp)
.Ltmp147:
	leaq	8(%rsp), %rsi
	movq	%rbx, %rdi
	callq	_ZNSt13__future_base13_State_baseV216_M_break_promiseESt10unique_ptrINS_12_Result_baseENS2_8_DeleterEE
.Ltmp148:
# BB#19:
	movq	8(%rsp), %rdi
	testq	%rdi, %rdi
	je	.LBB27_21
# BB#20:
	movq	(%rdi), %rax
	movq	(%rax), %rax
.Ltmp153:
	callq	*%rax
.Ltmp154:
.LBB27_21:
	movq	$0, 8(%rsp)
.LBB27_22:
.Ltmp156:
	callq	__cxa_rethrow
.Ltmp157:
# BB#30:
.LBB27_32:
.Ltmp155:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.LBB27_23:
.Ltmp149:
	movq	%rax, %r14
	movq	8(%rsp), %rdi
	testq	%rdi, %rdi
	je	.LBB27_25
# BB#24:
	movq	(%rdi), %rax
	movq	(%rax), %rax
.Ltmp150:
	callq	*%rax
.Ltmp151:
.LBB27_25:
	movq	$0, 8(%rsp)
	jmp	.LBB27_27
.LBB27_33:
.Ltmp152:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.LBB27_26:
.Ltmp158:
	movq	%rax, %r14
.LBB27_27:
.Ltmp159:
	callq	__cxa_end_catch
.Ltmp160:
.LBB27_28:
	movq	%r14, %rdi
	callq	_Unwind_Resume
.LBB27_29:
.Ltmp161:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.LBB27_31:
.Ltmp146:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.Lfunc_end27:
	.size	_ZZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEC1EOS5_ENKUlvE_clEv, .Lfunc_end27-_ZZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEC1EOS5_ENKUlvE_clEv
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table27:
.Lexception10:
	.byte	255                     # @LPStart Encoding = omit
	.byte	3                       # @TType Encoding = udata4
	.ascii	"\240\001"              # @TType base offset
	.byte	3                       # Call site Encoding = udata4
	.ascii	"\217\001"              # Call site table length
	.long	.Ltmp132-.Lfunc_begin10 # >> Call Site 1 <<
	.long	.Ltmp137-.Ltmp132       #   Call between .Ltmp132 and .Ltmp137
	.long	.Ltmp143-.Lfunc_begin10 #     jumps to .Ltmp143
	.byte	3                       #   On action: 2
	.long	.Ltmp138-.Lfunc_begin10 # >> Call Site 2 <<
	.long	.Ltmp139-.Ltmp138       #   Call between .Ltmp138 and .Ltmp139
	.long	.Ltmp140-.Lfunc_begin10 #     jumps to .Ltmp140
	.byte	5                       #   On action: 3
	.long	.Ltmp134-.Lfunc_begin10 # >> Call Site 3 <<
	.long	.Ltmp142-.Ltmp134       #   Call between .Ltmp134 and .Ltmp142
	.long	.Ltmp143-.Lfunc_begin10 #     jumps to .Ltmp143
	.byte	3                       #   On action: 2
	.long	.Ltmp144-.Lfunc_begin10 # >> Call Site 4 <<
	.long	.Ltmp145-.Ltmp144       #   Call between .Ltmp144 and .Ltmp145
	.long	.Ltmp146-.Lfunc_begin10 #     jumps to .Ltmp146
	.byte	5                       #   On action: 3
	.long	.Ltmp145-.Lfunc_begin10 # >> Call Site 5 <<
	.long	.Ltmp147-.Ltmp145       #   Call between .Ltmp145 and .Ltmp147
	.long	0                       #     has no landing pad
	.byte	0                       #   On action: cleanup
	.long	.Ltmp147-.Lfunc_begin10 # >> Call Site 6 <<
	.long	.Ltmp148-.Ltmp147       #   Call between .Ltmp147 and .Ltmp148
	.long	.Ltmp149-.Lfunc_begin10 #     jumps to .Ltmp149
	.byte	0                       #   On action: cleanup
	.long	.Ltmp153-.Lfunc_begin10 # >> Call Site 7 <<
	.long	.Ltmp154-.Ltmp153       #   Call between .Ltmp153 and .Ltmp154
	.long	.Ltmp155-.Lfunc_begin10 #     jumps to .Ltmp155
	.byte	5                       #   On action: 3
	.long	.Ltmp156-.Lfunc_begin10 # >> Call Site 8 <<
	.long	.Ltmp157-.Ltmp156       #   Call between .Ltmp156 and .Ltmp157
	.long	.Ltmp158-.Lfunc_begin10 #     jumps to .Ltmp158
	.byte	0                       #   On action: cleanup
	.long	.Ltmp150-.Lfunc_begin10 # >> Call Site 9 <<
	.long	.Ltmp151-.Ltmp150       #   Call between .Ltmp150 and .Ltmp151
	.long	.Ltmp152-.Lfunc_begin10 #     jumps to .Ltmp152
	.byte	5                       #   On action: 3
	.long	.Ltmp159-.Lfunc_begin10 # >> Call Site 10 <<
	.long	.Ltmp160-.Ltmp159       #   Call between .Ltmp159 and .Ltmp160
	.long	.Ltmp161-.Lfunc_begin10 #     jumps to .Ltmp161
	.byte	5                       #   On action: 3
	.long	.Ltmp160-.Lfunc_begin10 # >> Call Site 11 <<
	.long	.Lfunc_end27-.Ltmp160   #   Call between .Ltmp160 and .Lfunc_end27
	.long	0                       #     has no landing pad
	.byte	0                       #   On action: cleanup
	.byte	0                       # >> Action Record 1 <<
                                        #   Cleanup
	.byte	0                       #   No further actions
	.byte	2                       # >> Action Record 2 <<
                                        #   Catch TypeInfo 2
	.byte	125                     #   Continue to action 1
	.byte	1                       # >> Action Record 3 <<
                                        #   Catch TypeInfo 1
	.byte	0                       #   No further actions
                                        # >> Catch TypeInfos <<
	.long	_ZTIN10__cxxabiv115__forced_unwindE # TypeInfo 2
	.long	0                       # TypeInfo 1
	.p2align	2

	.section	.text._ZNSt13__future_base13_State_baseV216_M_break_promiseESt10unique_ptrINS_12_Result_baseENS2_8_DeleterEE,"axG",@progbits,_ZNSt13__future_base13_State_baseV216_M_break_promiseESt10unique_ptrINS_12_Result_baseENS2_8_DeleterEE,comdat
	.weak	_ZNSt13__future_base13_State_baseV216_M_break_promiseESt10unique_ptrINS_12_Result_baseENS2_8_DeleterEE
	.p2align	4, 0x90
	.type	_ZNSt13__future_base13_State_baseV216_M_break_promiseESt10unique_ptrINS_12_Result_baseENS2_8_DeleterEE,@function
_ZNSt13__future_base13_State_baseV216_M_break_promiseESt10unique_ptrINS_12_Result_baseENS2_8_DeleterEE: # @_ZNSt13__future_base13_State_baseV216_M_break_promiseESt10unique_ptrINS_12_Result_baseENS2_8_DeleterEE
	.cfi_startproc
# BB#0:
	pushq	%rbp
.Ltmp171:
	.cfi_def_cfa_offset 16
	pushq	%r15
.Ltmp172:
	.cfi_def_cfa_offset 24
	pushq	%r14
.Ltmp173:
	.cfi_def_cfa_offset 32
	pushq	%r13
.Ltmp174:
	.cfi_def_cfa_offset 40
	pushq	%r12
.Ltmp175:
	.cfi_def_cfa_offset 48
	pushq	%rbx
.Ltmp176:
	.cfi_def_cfa_offset 56
	subq	$56, %rsp
.Ltmp177:
	.cfi_def_cfa_offset 112
.Ltmp178:
	.cfi_offset %rbx, -56
.Ltmp179:
	.cfi_offset %r12, -48
.Ltmp180:
	.cfi_offset %r13, -40
.Ltmp181:
	.cfi_offset %r14, -32
.Ltmp182:
	.cfi_offset %r15, -24
.Ltmp183:
	.cfi_offset %rbp, -16
	movq	%rsi, %r15
	movq	%rdi, %r14
	cmpq	$0, (%r15)
	je	.LBB28_3
# BB#1:
	callq	_ZSt15future_categoryv
	movq	(%r15), %rbx
	addq	$8, %rbx
	leaq	24(%rsp), %r12
	movl	$4, %esi
	movq	%r12, %rdi
	movq	%rax, %rdx
	callq	_ZNSt12future_errorC2ESt10error_code
	leaq	8(%rsp), %r13
	movq	%r13, %rdi
	movq	%r12, %rsi
	callq	_ZSt18make_exception_ptrISt12future_errorENSt15__exception_ptr13exception_ptrET_
	movq	8(%rsp), %rax
	movq	%rax, 16(%rsp)
	movq	$0, 8(%rsp)
	leaq	16(%rsp), %rbp
	movq	%rbp, %rdi
	movq	%rbx, %rsi
	callq	_ZNSt15__exception_ptr13exception_ptr4swapERS0_
	movq	%rbp, %rdi
	callq	_ZNSt15__exception_ptr13exception_ptrD1Ev
	movq	%r13, %rdi
	callq	_ZNSt15__exception_ptr13exception_ptrD1Ev
	movq	%r12, %rdi
	callq	_ZNSt12future_errorD1Ev
	movq	8(%r14), %rax
	movq	(%r15), %rcx
	movq	%rcx, 8(%r14)
	movq	%rax, (%r15)
	movl	$1, %eax
	xchgl	%eax, 16(%r14)
	testl	%eax, %eax
	jns	.LBB28_3
# BB#2:
	addq	$16, %r14
	movq	%r14, %rdi
	callq	_ZNSt28__atomic_futex_unsigned_base19_M_futex_notify_allEPj
.LBB28_3:
	addq	$56, %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	retq
.Lfunc_end28:
	.size	_ZNSt13__future_base13_State_baseV216_M_break_promiseESt10unique_ptrINS_12_Result_baseENS2_8_DeleterEE, .Lfunc_end28-_ZNSt13__future_base13_State_baseV216_M_break_promiseESt10unique_ptrINS_12_Result_baseENS2_8_DeleterEE
	.cfi_endproc

	.section	.text._ZNSt13__future_base13_State_baseV29_M_do_setEPSt8functionIFSt10unique_ptrINS_12_Result_baseENS3_8_DeleterEEvEEPb,"axG",@progbits,_ZNSt13__future_base13_State_baseV29_M_do_setEPSt8functionIFSt10unique_ptrINS_12_Result_baseENS3_8_DeleterEEvEEPb,comdat
	.weak	_ZNSt13__future_base13_State_baseV29_M_do_setEPSt8functionIFSt10unique_ptrINS_12_Result_baseENS3_8_DeleterEEvEEPb
	.p2align	4, 0x90
	.type	_ZNSt13__future_base13_State_baseV29_M_do_setEPSt8functionIFSt10unique_ptrINS_12_Result_baseENS3_8_DeleterEEvEEPb,@function
_ZNSt13__future_base13_State_baseV29_M_do_setEPSt8functionIFSt10unique_ptrINS_12_Result_baseENS3_8_DeleterEEvEEPb: # @_ZNSt13__future_base13_State_baseV29_M_do_setEPSt8functionIFSt10unique_ptrINS_12_Result_baseENS3_8_DeleterEEvEEPb
.Lfunc_begin11:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception11
# BB#0:
	pushq	%r14
.Ltmp187:
	.cfi_def_cfa_offset 16
	pushq	%rbx
.Ltmp188:
	.cfi_def_cfa_offset 24
	pushq	%rax
.Ltmp189:
	.cfi_def_cfa_offset 32
.Ltmp190:
	.cfi_offset %rbx, -24
.Ltmp191:
	.cfi_offset %r14, -16
	movq	%rdx, %r14
	movq	%rdi, %rbx
	cmpq	$0, 16(%rsi)
	je	.LBB29_4
# BB#1:
	leaq	(%rsp), %rdi
	callq	*24(%rsi)
	movb	$1, (%r14)
	movq	8(%rbx), %rdi
	movq	(%rsp), %rax
	movq	%rax, 8(%rbx)
	movq	%rdi, (%rsp)
	testq	%rdi, %rdi
	je	.LBB29_3
# BB#2:
	movq	(%rdi), %rax
	movq	(%rax), %rax
.Ltmp184:
	callq	*%rax
.Ltmp185:
.LBB29_3:
	addq	$8, %rsp
	popq	%rbx
	popq	%r14
	retq
.LBB29_4:
	callq	_ZSt25__throw_bad_function_callv
.LBB29_5:
.Ltmp186:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.Lfunc_end29:
	.size	_ZNSt13__future_base13_State_baseV29_M_do_setEPSt8functionIFSt10unique_ptrINS_12_Result_baseENS3_8_DeleterEEvEEPb, .Lfunc_end29-_ZNSt13__future_base13_State_baseV29_M_do_setEPSt8functionIFSt10unique_ptrINS_12_Result_baseENS3_8_DeleterEEvEEPb
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table29:
.Lexception11:
	.byte	255                     # @LPStart Encoding = omit
	.byte	3                       # @TType Encoding = udata4
	.asciz	"\257\200"              # @TType base offset
	.byte	3                       # Call site Encoding = udata4
	.byte	39                      # Call site table length
	.long	.Lfunc_begin11-.Lfunc_begin11 # >> Call Site 1 <<
	.long	.Ltmp184-.Lfunc_begin11 #   Call between .Lfunc_begin11 and .Ltmp184
	.long	0                       #     has no landing pad
	.byte	0                       #   On action: cleanup
	.long	.Ltmp184-.Lfunc_begin11 # >> Call Site 2 <<
	.long	.Ltmp185-.Ltmp184       #   Call between .Ltmp184 and .Ltmp185
	.long	.Ltmp186-.Lfunc_begin11 #     jumps to .Ltmp186
	.byte	1                       #   On action: 1
	.long	.Ltmp185-.Lfunc_begin11 # >> Call Site 3 <<
	.long	.Lfunc_end29-.Ltmp185   #   Call between .Ltmp185 and .Lfunc_end29
	.long	0                       #     has no landing pad
	.byte	0                       #   On action: cleanup
	.byte	1                       # >> Action Record 1 <<
                                        #   Catch TypeInfo 1
	.byte	0                       #   No further actions
                                        # >> Catch TypeInfos <<
	.long	0                       # TypeInfo 1
	.p2align	2

	.section	.text._ZSt16__once_call_implISt12_Bind_simpleIFSt7_Mem_fnIMNSt13__future_base13_State_baseV2EFvPSt8functionIFSt10unique_ptrINS2_12_Result_baseENS6_8_DeleterEEvEEPbEEPS3_SB_SC_EEEvv,"axG",@progbits,_ZSt16__once_call_implISt12_Bind_simpleIFSt7_Mem_fnIMNSt13__future_base13_State_baseV2EFvPSt8functionIFSt10unique_ptrINS2_12_Result_baseENS6_8_DeleterEEvEEPbEEPS3_SB_SC_EEEvv,comdat
	.weak	_ZSt16__once_call_implISt12_Bind_simpleIFSt7_Mem_fnIMNSt13__future_base13_State_baseV2EFvPSt8functionIFSt10unique_ptrINS2_12_Result_baseENS6_8_DeleterEEvEEPbEEPS3_SB_SC_EEEvv
	.p2align	4, 0x90
	.type	_ZSt16__once_call_implISt12_Bind_simpleIFSt7_Mem_fnIMNSt13__future_base13_State_baseV2EFvPSt8functionIFSt10unique_ptrINS2_12_Result_baseENS6_8_DeleterEEvEEPbEEPS3_SB_SC_EEEvv,@function
_ZSt16__once_call_implISt12_Bind_simpleIFSt7_Mem_fnIMNSt13__future_base13_State_baseV2EFvPSt8functionIFSt10unique_ptrINS2_12_Result_baseENS6_8_DeleterEEvEEPbEEPS3_SB_SC_EEEvv: # @_ZSt16__once_call_implISt12_Bind_simpleIFSt7_Mem_fnIMNSt13__future_base13_State_baseV2EFvPSt8functionIFSt10unique_ptrINS2_12_Result_baseENS6_8_DeleterEEvEEPbEEPS3_SB_SC_EEEvv
	.cfi_startproc
# BB#0:
	movq	_ZSt15__once_callable@GOTTPOFF(%rip), %rax
	movq	%fs:(%rax), %rcx
	movq	16(%rcx), %rdi
	movq	24(%rcx), %rax
	addq	32(%rcx), %rdi
	testb	$1, %al
	je	.LBB30_2
# BB#1:
	movq	(%rdi), %rdx
	movq	-1(%rdx,%rax), %rax
.LBB30_2:
	movq	(%rcx), %rdx
	movq	8(%rcx), %rsi
	jmpq	*%rax                   # TAILCALL
.Lfunc_end30:
	.size	_ZSt16__once_call_implISt12_Bind_simpleIFSt7_Mem_fnIMNSt13__future_base13_State_baseV2EFvPSt8functionIFSt10unique_ptrINS2_12_Result_baseENS6_8_DeleterEEvEEPbEEPS3_SB_SC_EEEvv, .Lfunc_end30-_ZSt16__once_call_implISt12_Bind_simpleIFSt7_Mem_fnIMNSt13__future_base13_State_baseV2EFvPSt8functionIFSt10unique_ptrINS2_12_Result_baseENS6_8_DeleterEEvEEPbEEPS3_SB_SC_EEEvv
	.cfi_endproc

	.section	.text._ZNSt17_Function_handlerIFSt10unique_ptrINSt13__future_base12_Result_baseENS2_8_DeleterEEvENS1_12_Task_setterIS0_INS1_7_ResultIiEES3_ESt12_Bind_simpleIFPFivEvEEiEEE9_M_invokeERKSt9_Any_data,"axG",@progbits,_ZNSt17_Function_handlerIFSt10unique_ptrINSt13__future_base12_Result_baseENS2_8_DeleterEEvENS1_12_Task_setterIS0_INS1_7_ResultIiEES3_ESt12_Bind_simpleIFPFivEvEEiEEE9_M_invokeERKSt9_Any_data,comdat
	.weak	_ZNSt17_Function_handlerIFSt10unique_ptrINSt13__future_base12_Result_baseENS2_8_DeleterEEvENS1_12_Task_setterIS0_INS1_7_ResultIiEES3_ESt12_Bind_simpleIFPFivEvEEiEEE9_M_invokeERKSt9_Any_data
	.p2align	4, 0x90
	.type	_ZNSt17_Function_handlerIFSt10unique_ptrINSt13__future_base12_Result_baseENS2_8_DeleterEEvENS1_12_Task_setterIS0_INS1_7_ResultIiEES3_ESt12_Bind_simpleIFPFivEvEEiEEE9_M_invokeERKSt9_Any_data,@function
_ZNSt17_Function_handlerIFSt10unique_ptrINSt13__future_base12_Result_baseENS2_8_DeleterEEvENS1_12_Task_setterIS0_INS1_7_ResultIiEES3_ESt12_Bind_simpleIFPFivEvEEiEEE9_M_invokeERKSt9_Any_data: # @_ZNSt17_Function_handlerIFSt10unique_ptrINSt13__future_base12_Result_baseENS2_8_DeleterEEvENS1_12_Task_setterIS0_INS1_7_ResultIiEES3_ESt12_Bind_simpleIFPFivEvEEiEEE9_M_invokeERKSt9_Any_data
	.cfi_startproc
# BB#0:
	pushq	%rbx
.Ltmp192:
	.cfi_def_cfa_offset 16
	subq	$16, %rsp
.Ltmp193:
	.cfi_def_cfa_offset 32
.Ltmp194:
	.cfi_offset %rbx, -16
	movq	%rdi, %rbx
	leaq	8(%rsp), %rdi
	callq	_ZNKSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEclEv
	movq	8(%rsp), %rax
	movq	%rax, (%rbx)
	movq	%rbx, %rax
	addq	$16, %rsp
	popq	%rbx
	retq
.Lfunc_end31:
	.size	_ZNSt17_Function_handlerIFSt10unique_ptrINSt13__future_base12_Result_baseENS2_8_DeleterEEvENS1_12_Task_setterIS0_INS1_7_ResultIiEES3_ESt12_Bind_simpleIFPFivEvEEiEEE9_M_invokeERKSt9_Any_data, .Lfunc_end31-_ZNSt17_Function_handlerIFSt10unique_ptrINSt13__future_base12_Result_baseENS2_8_DeleterEEvENS1_12_Task_setterIS0_INS1_7_ResultIiEES3_ESt12_Bind_simpleIFPFivEvEEiEEE9_M_invokeERKSt9_Any_data
	.cfi_endproc

	.section	.text._ZNSt14_Function_base13_Base_managerINSt13__future_base12_Task_setterISt10unique_ptrINS1_7_ResultIiEENS1_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEEE10_M_managerERSt9_Any_dataRKSG_St18_Manager_operation,"axG",@progbits,_ZNSt14_Function_base13_Base_managerINSt13__future_base12_Task_setterISt10unique_ptrINS1_7_ResultIiEENS1_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEEE10_M_managerERSt9_Any_dataRKSG_St18_Manager_operation,comdat
	.weak	_ZNSt14_Function_base13_Base_managerINSt13__future_base12_Task_setterISt10unique_ptrINS1_7_ResultIiEENS1_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEEE10_M_managerERSt9_Any_dataRKSG_St18_Manager_operation
	.p2align	4, 0x90
	.type	_ZNSt14_Function_base13_Base_managerINSt13__future_base12_Task_setterISt10unique_ptrINS1_7_ResultIiEENS1_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEEE10_M_managerERSt9_Any_dataRKSG_St18_Manager_operation,@function
_ZNSt14_Function_base13_Base_managerINSt13__future_base12_Task_setterISt10unique_ptrINS1_7_ResultIiEENS1_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEEE10_M_managerERSt9_Any_dataRKSG_St18_Manager_operation: # @_ZNSt14_Function_base13_Base_managerINSt13__future_base12_Task_setterISt10unique_ptrINS1_7_ResultIiEENS1_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEEE10_M_managerERSt9_Any_dataRKSG_St18_Manager_operation
	.cfi_startproc
# BB#0:
	cmpl	$2, %edx
	je	.LBB32_5
# BB#1:
	cmpl	$1, %edx
	je	.LBB32_4
# BB#2:
	testl	%edx, %edx
	jne	.LBB32_6
# BB#3:
	movq	$_ZTINSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEE, (%rdi)
	xorl	%eax, %eax
	retq
.LBB32_5:
	movups	(%rsi), %xmm0
	movups	%xmm0, (%rdi)
.LBB32_6:
	xorl	%eax, %eax
	retq
.LBB32_4:
	movq	%rsi, (%rdi)
	xorl	%eax, %eax
	retq
.Lfunc_end32:
	.size	_ZNSt14_Function_base13_Base_managerINSt13__future_base12_Task_setterISt10unique_ptrINS1_7_ResultIiEENS1_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEEE10_M_managerERSt9_Any_dataRKSG_St18_Manager_operation, .Lfunc_end32-_ZNSt14_Function_base13_Base_managerINSt13__future_base12_Task_setterISt10unique_ptrINS1_7_ResultIiEENS1_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEEE10_M_managerERSt9_Any_dataRKSG_St18_Manager_operation
	.cfi_endproc

	.section	.text._ZNKSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEclEv,"axG",@progbits,_ZNKSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEclEv,comdat
	.weak	_ZNKSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEclEv
	.p2align	4, 0x90
	.type	_ZNKSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEclEv,@function
_ZNKSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEclEv: # @_ZNKSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEclEv
.Lfunc_begin12:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception12
# BB#0:
	pushq	%rbp
.Ltmp204:
	.cfi_def_cfa_offset 16
	pushq	%r15
.Ltmp205:
	.cfi_def_cfa_offset 24
	pushq	%r14
.Ltmp206:
	.cfi_def_cfa_offset 32
	pushq	%r13
.Ltmp207:
	.cfi_def_cfa_offset 40
	pushq	%r12
.Ltmp208:
	.cfi_def_cfa_offset 48
	pushq	%rbx
.Ltmp209:
	.cfi_def_cfa_offset 56
	subq	$24, %rsp
.Ltmp210:
	.cfi_def_cfa_offset 80
.Ltmp211:
	.cfi_offset %rbx, -56
.Ltmp212:
	.cfi_offset %r12, -48
.Ltmp213:
	.cfi_offset %r13, -40
.Ltmp214:
	.cfi_offset %r14, -32
.Ltmp215:
	.cfi_offset %r15, -24
.Ltmp216:
	.cfi_offset %rbp, -16
	movq	%rsi, %rbp
	movq	%rdi, %r14
	movq	(%rbp), %rax
	movq	8(%rbp), %rcx
	movq	(%rax), %rbx
	movq	(%rcx), %rax
.Ltmp195:
	callq	*%rax
.Ltmp196:
# BB#1:
	movl	%eax, 16(%rbx)
	movb	$1, 20(%rbx)
.LBB33_5:
	movq	(%rbp), %rax
	movq	(%rax), %rcx
	movq	$0, (%rax)
	movq	%rcx, (%r14)
	movq	%r14, %rax
	addq	$24, %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	retq
.LBB33_2:
.Ltmp197:
	movq	%rdx, %r15
	movq	%rax, %rdi
	callq	__cxa_begin_catch
	cmpl	$2, %r15d
	je	.LBB33_3
# BB#4:
	movq	(%rbp), %rax
	movq	(%rax), %r15
	addq	$8, %r15
	leaq	8(%rsp), %r12
	movq	%r12, %rdi
	callq	_ZSt17current_exceptionv
	movq	8(%rsp), %rax
	movq	%rax, 16(%rsp)
	movq	$0, 8(%rsp)
	leaq	16(%rsp), %r13
	movq	%r13, %rdi
	movq	%r15, %rsi
	callq	_ZNSt15__exception_ptr13exception_ptr4swapERS0_
	movq	%r13, %rdi
	callq	_ZNSt15__exception_ptr13exception_ptrD1Ev
	movq	%r12, %rdi
	callq	_ZNSt15__exception_ptr13exception_ptrD1Ev
	callq	__cxa_end_catch
	jmp	.LBB33_5
.LBB33_3:
.Ltmp198:
	callq	__cxa_rethrow
.Ltmp199:
# BB#9:
.LBB33_6:
.Ltmp200:
	movq	%rax, %rbx
.Ltmp201:
	callq	__cxa_end_catch
.Ltmp202:
# BB#7:
	movq	%rbx, %rdi
	callq	_Unwind_Resume
.LBB33_8:
.Ltmp203:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.Lfunc_end33:
	.size	_ZNKSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEclEv, .Lfunc_end33-_ZNKSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEclEv
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table33:
.Lexception12:
	.byte	255                     # @LPStart Encoding = omit
	.byte	3                       # @TType Encoding = udata4
	.asciz	"\317\200"              # @TType base offset
	.byte	3                       # Call site Encoding = udata4
	.byte	65                      # Call site table length
	.long	.Ltmp195-.Lfunc_begin12 # >> Call Site 1 <<
	.long	.Ltmp196-.Ltmp195       #   Call between .Ltmp195 and .Ltmp196
	.long	.Ltmp197-.Lfunc_begin12 #     jumps to .Ltmp197
	.byte	3                       #   On action: 2
	.long	.Ltmp196-.Lfunc_begin12 # >> Call Site 2 <<
	.long	.Ltmp198-.Ltmp196       #   Call between .Ltmp196 and .Ltmp198
	.long	0                       #     has no landing pad
	.byte	0                       #   On action: cleanup
	.long	.Ltmp198-.Lfunc_begin12 # >> Call Site 3 <<
	.long	.Ltmp199-.Ltmp198       #   Call between .Ltmp198 and .Ltmp199
	.long	.Ltmp200-.Lfunc_begin12 #     jumps to .Ltmp200
	.byte	0                       #   On action: cleanup
	.long	.Ltmp201-.Lfunc_begin12 # >> Call Site 4 <<
	.long	.Ltmp202-.Ltmp201       #   Call between .Ltmp201 and .Ltmp202
	.long	.Ltmp203-.Lfunc_begin12 #     jumps to .Ltmp203
	.byte	1                       #   On action: 1
	.long	.Ltmp202-.Lfunc_begin12 # >> Call Site 5 <<
	.long	.Lfunc_end33-.Ltmp202   #   Call between .Ltmp202 and .Lfunc_end33
	.long	0                       #     has no landing pad
	.byte	0                       #   On action: cleanup
	.byte	1                       # >> Action Record 1 <<
                                        #   Catch TypeInfo 1
	.byte	0                       #   No further actions
	.byte	2                       # >> Action Record 2 <<
                                        #   Catch TypeInfo 2
	.byte	125                     #   Continue to action 1
                                        # >> Catch TypeInfos <<
	.long	_ZTIN10__cxxabiv115__forced_unwindE # TypeInfo 2
	.long	0                       # TypeInfo 1
	.p2align	2

	.section	.text._ZSt18make_exception_ptrISt12future_errorENSt15__exception_ptr13exception_ptrET_,"axG",@progbits,_ZSt18make_exception_ptrISt12future_errorENSt15__exception_ptr13exception_ptrET_,comdat
	.weak	_ZSt18make_exception_ptrISt12future_errorENSt15__exception_ptr13exception_ptrET_
	.p2align	4, 0x90
	.type	_ZSt18make_exception_ptrISt12future_errorENSt15__exception_ptr13exception_ptrET_,@function
_ZSt18make_exception_ptrISt12future_errorENSt15__exception_ptr13exception_ptrET_: # @_ZSt18make_exception_ptrISt12future_errorENSt15__exception_ptr13exception_ptrET_
.Lfunc_begin13:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception13
# BB#0:
	pushq	%r15
.Ltmp223:
	.cfi_def_cfa_offset 16
	pushq	%r14
.Ltmp224:
	.cfi_def_cfa_offset 24
	pushq	%rbx
.Ltmp225:
	.cfi_def_cfa_offset 32
.Ltmp226:
	.cfi_offset %rbx, -32
.Ltmp227:
	.cfi_offset %r14, -24
.Ltmp228:
	.cfi_offset %r15, -16
	movq	%rsi, %r15
	movq	%rdi, %r14
	movl	$32, %edi
	callq	__cxa_allocate_exception
	movq	%rax, %rbx
	movq	%rbx, %rdi
	movq	%r15, %rsi
	callq	_ZNSt11logic_errorC2ERKS_
	movq	$_ZTVSt12future_error+16, (%rbx)
	movups	16(%r15), %xmm0
	movups	%xmm0, 16(%rbx)
.Ltmp217:
	movl	$_ZTISt12future_error, %esi
	movl	$_ZNSt12future_errorD1Ev, %edx
	movq	%rbx, %rdi
	callq	__cxa_throw
.Ltmp218:
# BB#4:
.LBB34_1:
.Ltmp219:
	movq	%rax, %rdi
	callq	__cxa_begin_catch
	movq	%r14, %rdi
	callq	_ZSt17current_exceptionv
.Ltmp220:
	callq	__cxa_end_catch
.Ltmp221:
# BB#2:
	movq	%r14, %rax
	popq	%rbx
	popq	%r14
	popq	%r15
	retq
.LBB34_3:
.Ltmp222:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.Lfunc_end34:
	.size	_ZSt18make_exception_ptrISt12future_errorENSt15__exception_ptr13exception_ptrET_, .Lfunc_end34-_ZSt18make_exception_ptrISt12future_errorENSt15__exception_ptr13exception_ptrET_
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table34:
.Lexception13:
	.byte	255                     # @LPStart Encoding = omit
	.byte	3                       # @TType Encoding = udata4
	.asciz	"\274"                  # @TType base offset
	.byte	3                       # Call site Encoding = udata4
	.byte	52                      # Call site table length
	.long	.Lfunc_begin13-.Lfunc_begin13 # >> Call Site 1 <<
	.long	.Ltmp217-.Lfunc_begin13 #   Call between .Lfunc_begin13 and .Ltmp217
	.long	0                       #     has no landing pad
	.byte	0                       #   On action: cleanup
	.long	.Ltmp217-.Lfunc_begin13 # >> Call Site 2 <<
	.long	.Ltmp218-.Ltmp217       #   Call between .Ltmp217 and .Ltmp218
	.long	.Ltmp219-.Lfunc_begin13 #     jumps to .Ltmp219
	.byte	1                       #   On action: 1
	.long	.Ltmp218-.Lfunc_begin13 # >> Call Site 3 <<
	.long	.Ltmp220-.Ltmp218       #   Call between .Ltmp218 and .Ltmp220
	.long	0                       #     has no landing pad
	.byte	0                       #   On action: cleanup
	.long	.Ltmp220-.Lfunc_begin13 # >> Call Site 4 <<
	.long	.Ltmp221-.Ltmp220       #   Call between .Ltmp220 and .Ltmp221
	.long	.Ltmp222-.Lfunc_begin13 #     jumps to .Ltmp222
	.byte	1                       #   On action: 1
	.byte	1                       # >> Action Record 1 <<
                                        #   Catch TypeInfo 1
	.byte	0                       #   No further actions
                                        # >> Catch TypeInfos <<
	.long	0                       # TypeInfo 1
	.p2align	2

	.section	.text._ZNSt12future_errorC2ESt10error_code,"axG",@progbits,_ZNSt12future_errorC2ESt10error_code,comdat
	.weak	_ZNSt12future_errorC2ESt10error_code
	.p2align	4, 0x90
	.type	_ZNSt12future_errorC2ESt10error_code,@function
_ZNSt12future_errorC2ESt10error_code:   # @_ZNSt12future_errorC2ESt10error_code
.Lfunc_begin14:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception14
# BB#0:
	pushq	%rbp
.Ltmp235:
	.cfi_def_cfa_offset 16
	pushq	%r15
.Ltmp236:
	.cfi_def_cfa_offset 24
	pushq	%r14
.Ltmp237:
	.cfi_def_cfa_offset 32
	pushq	%rbx
.Ltmp238:
	.cfi_def_cfa_offset 40
	subq	$72, %rsp
.Ltmp239:
	.cfi_def_cfa_offset 112
.Ltmp240:
	.cfi_offset %rbx, -40
.Ltmp241:
	.cfi_offset %r14, -32
.Ltmp242:
	.cfi_offset %r15, -24
.Ltmp243:
	.cfi_offset %rbp, -16
	movq	%rdx, %r14
	movl	%esi, %r15d
	movq	%rdi, %rbx
	movq	(%r14), %rax
	leaq	40(%rsp), %rbp
	movq	%rbp, %rdi
	movq	%r14, %rsi
	movl	%r15d, %edx
	callq	*32(%rax)
.Ltmp229:
	xorl	%esi, %esi
	xorl	%edx, %edx
	movl	$.L.str, %ecx
	movl	$19, %r8d
	movq	%rbp, %rdi
	callq	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_replaceEmmPKcm
.Ltmp230:
# BB#1:
	leaq	24(%rsp), %rbp
	movq	%rbp, 8(%rsp)
	movq	(%rax), %rdx
	movq	%rax, %rcx
	addq	$16, %rcx
	cmpq	%rcx, %rdx
	je	.LBB35_2
# BB#3:
	movq	%rdx, 8(%rsp)
	movq	16(%rax), %rdx
	movq	%rdx, 24(%rsp)
	jmp	.LBB35_4
.LBB35_2:
	movups	(%rdx), %xmm0
	movups	%xmm0, (%rbp)
.LBB35_4:
	movq	8(%rax), %rdx
	movq	%rdx, 16(%rsp)
	movq	%rcx, (%rax)
	movq	$0, 8(%rax)
	movb	$0, 16(%rax)
.Ltmp232:
	leaq	8(%rsp), %rsi
	movq	%rbx, %rdi
	callq	_ZNSt11logic_errorC2ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
.Ltmp233:
# BB#5:
	movq	8(%rsp), %rdi
	cmpq	%rbp, %rdi
	je	.LBB35_7
# BB#6:
	callq	_ZdlPv
.LBB35_7:
	movq	40(%rsp), %rdi
	leaq	56(%rsp), %rax
	cmpq	%rax, %rdi
	je	.LBB35_9
# BB#8:
	callq	_ZdlPv
.LBB35_9:
	movq	$_ZTVSt12future_error+16, (%rbx)
	movl	%r15d, 16(%rbx)
	movq	%r14, 24(%rbx)
	addq	$72, %rsp
	popq	%rbx
	popq	%r14
	popq	%r15
	popq	%rbp
	retq
.LBB35_11:
.Ltmp234:
	movq	%rax, %rbx
	movq	8(%rsp), %rdi
	cmpq	%rbp, %rdi
	je	.LBB35_13
# BB#12:
	callq	_ZdlPv
	jmp	.LBB35_13
.LBB35_10:
.Ltmp231:
	movq	%rax, %rbx
.LBB35_13:
	movq	40(%rsp), %rdi
	leaq	56(%rsp), %rax
	cmpq	%rax, %rdi
	je	.LBB35_15
# BB#14:
	callq	_ZdlPv
.LBB35_15:
	movq	%rbx, %rdi
	callq	_Unwind_Resume
.Lfunc_end35:
	.size	_ZNSt12future_errorC2ESt10error_code, .Lfunc_end35-_ZNSt12future_errorC2ESt10error_code
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table35:
.Lexception14:
	.byte	255                     # @LPStart Encoding = omit
	.byte	3                       # @TType Encoding = udata4
	.asciz	"\266\200\200"          # @TType base offset
	.byte	3                       # Call site Encoding = udata4
	.byte	52                      # Call site table length
	.long	.Lfunc_begin14-.Lfunc_begin14 # >> Call Site 1 <<
	.long	.Ltmp229-.Lfunc_begin14 #   Call between .Lfunc_begin14 and .Ltmp229
	.long	0                       #     has no landing pad
	.byte	0                       #   On action: cleanup
	.long	.Ltmp229-.Lfunc_begin14 # >> Call Site 2 <<
	.long	.Ltmp230-.Ltmp229       #   Call between .Ltmp229 and .Ltmp230
	.long	.Ltmp231-.Lfunc_begin14 #     jumps to .Ltmp231
	.byte	0                       #   On action: cleanup
	.long	.Ltmp232-.Lfunc_begin14 # >> Call Site 3 <<
	.long	.Ltmp233-.Ltmp232       #   Call between .Ltmp232 and .Ltmp233
	.long	.Ltmp234-.Lfunc_begin14 #     jumps to .Ltmp234
	.byte	0                       #   On action: cleanup
	.long	.Ltmp233-.Lfunc_begin14 # >> Call Site 4 <<
	.long	.Lfunc_end35-.Ltmp233   #   Call between .Ltmp233 and .Lfunc_end35
	.long	0                       #     has no landing pad
	.byte	0                       #   On action: cleanup
	.p2align	2

	.section	.text._ZSt16__once_call_implISt12_Bind_simpleIFSt7_Mem_fnIMSt6threadFvvEEPS2_EEEvv,"axG",@progbits,_ZSt16__once_call_implISt12_Bind_simpleIFSt7_Mem_fnIMSt6threadFvvEEPS2_EEEvv,comdat
	.weak	_ZSt16__once_call_implISt12_Bind_simpleIFSt7_Mem_fnIMSt6threadFvvEEPS2_EEEvv
	.p2align	4, 0x90
	.type	_ZSt16__once_call_implISt12_Bind_simpleIFSt7_Mem_fnIMSt6threadFvvEEPS2_EEEvv,@function
_ZSt16__once_call_implISt12_Bind_simpleIFSt7_Mem_fnIMSt6threadFvvEEPS2_EEEvv: # @_ZSt16__once_call_implISt12_Bind_simpleIFSt7_Mem_fnIMSt6threadFvvEEPS2_EEEvv
	.cfi_startproc
# BB#0:
	movq	_ZSt15__once_callable@GOTTPOFF(%rip), %rax
	movq	%fs:(%rax), %rcx
	movq	(%rcx), %rdi
	movq	8(%rcx), %rax
	addq	16(%rcx), %rdi
	testb	$1, %al
	je	.LBB36_2
# BB#1:
	movq	(%rdi), %rcx
	movq	-1(%rcx,%rax), %rax
.LBB36_2:
	jmpq	*%rax                   # TAILCALL
.Lfunc_end36:
	.size	_ZSt16__once_call_implISt12_Bind_simpleIFSt7_Mem_fnIMSt6threadFvvEEPS2_EEEvv, .Lfunc_end36-_ZSt16__once_call_implISt12_Bind_simpleIFSt7_Mem_fnIMSt6threadFvvEEPS2_EEEvv
	.cfi_endproc

	.section	.text._ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED2Ev,"axG",@progbits,_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED2Ev,comdat
	.weak	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED2Ev
	.p2align	4, 0x90
	.type	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED2Ev,@function
_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED2Ev: # @_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED2Ev
	.cfi_startproc
# BB#0:
	retq
.Lfunc_end37:
	.size	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED2Ev, .Lfunc_end37-_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED2Ev
	.cfi_endproc

	.section	.text._ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED0Ev,"axG",@progbits,_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED0Ev,comdat
	.weak	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED0Ev
	.p2align	4, 0x90
	.type	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED0Ev,@function
_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED0Ev: # @_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED0Ev
	.cfi_startproc
# BB#0:
	jmp	_ZdlPv                  # TAILCALL
.Lfunc_end38:
	.size	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED0Ev, .Lfunc_end38-_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED0Ev
	.cfi_endproc

	.section	.text._ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv,"axG",@progbits,_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv,comdat
	.weak	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv
	.p2align	4, 0x90
	.type	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv,@function
_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv: # @_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv
.Lfunc_begin15:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception15
# BB#0:
	pushq	%rbx
.Ltmp250:
	.cfi_def_cfa_offset 16
.Ltmp251:
	.cfi_offset %rbx, -16
	movq	%rdi, %rbx
	movq	$_ZTVNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEE+16, 16(%rbx)
	movq	48(%rbx), %rdi
	testq	%rdi, %rdi
	je	.LBB39_2
# BB#1:
	movq	(%rdi), %rax
	movq	(%rax), %rax
.Ltmp244:
	callq	*%rax
.Ltmp245:
.LBB39_2:
	movq	$0, 48(%rbx)
	movq	$_ZTVNSt13__future_base13_State_baseV2E+16, 16(%rbx)
	movq	24(%rbx), %rdi
	testq	%rdi, %rdi
	je	.LBB39_4
# BB#3:
	movq	(%rdi), %rax
	movq	(%rax), %rax
.Ltmp247:
	callq	*%rax
.Ltmp248:
.LBB39_4:
	movq	$0, 24(%rbx)
	popq	%rbx
	retq
.LBB39_6:
.Ltmp249:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.LBB39_5:
.Ltmp246:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.Lfunc_end39:
	.size	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv, .Lfunc_end39-_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table39:
.Lexception15:
	.byte	255                     # @LPStart Encoding = omit
	.byte	3                       # @TType Encoding = udata4
	.asciz	"\242\200\200"          # @TType base offset
	.byte	3                       # Call site Encoding = udata4
	.byte	26                      # Call site table length
	.long	.Ltmp244-.Lfunc_begin15 # >> Call Site 1 <<
	.long	.Ltmp245-.Ltmp244       #   Call between .Ltmp244 and .Ltmp245
	.long	.Ltmp246-.Lfunc_begin15 #     jumps to .Ltmp246
	.byte	1                       #   On action: 1
	.long	.Ltmp247-.Lfunc_begin15 # >> Call Site 2 <<
	.long	.Ltmp248-.Ltmp247       #   Call between .Ltmp247 and .Ltmp248
	.long	.Ltmp249-.Lfunc_begin15 #     jumps to .Ltmp249
	.byte	1                       #   On action: 1
	.byte	1                       # >> Action Record 1 <<
                                        #   Catch TypeInfo 1
	.byte	0                       #   No further actions
                                        # >> Catch TypeInfos <<
	.long	0                       # TypeInfo 1
	.p2align	2

	.section	.text._ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv,"axG",@progbits,_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv,comdat
	.weak	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv
	.p2align	4, 0x90
	.type	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv,@function
_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv: # @_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv
	.cfi_startproc
# BB#0:
	testq	%rdi, %rdi
	je	.LBB40_1
# BB#2:
	jmp	_ZdlPv                  # TAILCALL
.LBB40_1:
	retq
.Lfunc_end40:
	.size	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv, .Lfunc_end40-_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv
	.cfi_endproc

	.section	.text._ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info,"axG",@progbits,_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info,comdat
	.weak	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info
	.p2align	4, 0x90
	.type	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info,@function
_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info: # @_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info
	.cfi_startproc
# BB#0:
	pushq	%rbx
.Ltmp252:
	.cfi_def_cfa_offset 16
.Ltmp253:
	.cfi_offset %rbx, -16
	movq	%rdi, %rbx
	movq	8(%rsi), %rdi
	movl	$_ZTSSt19_Sp_make_shared_tag, %eax
	cmpq	%rax, %rdi
	je	.LBB41_3
# BB#1:
	xorl	%eax, %eax
	cmpb	$42, (%rdi)
	je	.LBB41_4
# BB#2:
	movl	$_ZTSSt19_Sp_make_shared_tag, %esi
	callq	strcmp
	movl	%eax, %ecx
	xorl	%eax, %eax
	testl	%ecx, %ecx
	jne	.LBB41_4
.LBB41_3:
	addq	$16, %rbx
	movq	%rbx, %rax
.LBB41_4:
	popq	%rbx
	retq
.Lfunc_end41:
	.size	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info, .Lfunc_end41-_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info
	.cfi_endproc

	.section	.text._ZNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiED2Ev,"axG",@progbits,_ZNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiED2Ev,comdat
	.weak	_ZNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiED2Ev
	.p2align	4, 0x90
	.type	_ZNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiED2Ev,@function
_ZNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiED2Ev: # @_ZNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiED2Ev
.Lfunc_begin16:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception16
# BB#0:
	pushq	%rbx
.Ltmp260:
	.cfi_def_cfa_offset 16
.Ltmp261:
	.cfi_offset %rbx, -16
	movq	%rdi, %rbx
	movq	$_ZTVNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEE+16, (%rbx)
	movq	32(%rbx), %rdi
	testq	%rdi, %rdi
	je	.LBB42_2
# BB#1:
	movq	(%rdi), %rax
	movq	(%rax), %rax
.Ltmp254:
	callq	*%rax
.Ltmp255:
.LBB42_2:
	movq	$0, 32(%rbx)
	movq	$_ZTVNSt13__future_base13_State_baseV2E+16, (%rbx)
	movq	8(%rbx), %rdi
	testq	%rdi, %rdi
	je	.LBB42_4
# BB#3:
	movq	(%rdi), %rax
	movq	(%rax), %rax
.Ltmp257:
	callq	*%rax
.Ltmp258:
.LBB42_4:
	movq	$0, 8(%rbx)
	popq	%rbx
	retq
.LBB42_6:
.Ltmp259:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.LBB42_5:
.Ltmp256:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.Lfunc_end42:
	.size	_ZNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiED2Ev, .Lfunc_end42-_ZNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiED2Ev
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table42:
.Lexception16:
	.byte	255                     # @LPStart Encoding = omit
	.byte	3                       # @TType Encoding = udata4
	.asciz	"\242\200\200"          # @TType base offset
	.byte	3                       # Call site Encoding = udata4
	.byte	26                      # Call site table length
	.long	.Ltmp254-.Lfunc_begin16 # >> Call Site 1 <<
	.long	.Ltmp255-.Ltmp254       #   Call between .Ltmp254 and .Ltmp255
	.long	.Ltmp256-.Lfunc_begin16 #     jumps to .Ltmp256
	.byte	1                       #   On action: 1
	.long	.Ltmp257-.Lfunc_begin16 # >> Call Site 2 <<
	.long	.Ltmp258-.Ltmp257       #   Call between .Ltmp257 and .Ltmp258
	.long	.Ltmp259-.Lfunc_begin16 #     jumps to .Ltmp259
	.byte	1                       #   On action: 1
	.byte	1                       # >> Action Record 1 <<
                                        #   Catch TypeInfo 1
	.byte	0                       #   No further actions
                                        # >> Catch TypeInfos <<
	.long	0                       # TypeInfo 1
	.p2align	2

	.section	.text._ZNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiED0Ev,"axG",@progbits,_ZNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiED0Ev,comdat
	.weak	_ZNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiED0Ev
	.p2align	4, 0x90
	.type	_ZNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiED0Ev,@function
_ZNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiED0Ev: # @_ZNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiED0Ev
.Lfunc_begin17:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception17
# BB#0:
	pushq	%rbx
.Ltmp268:
	.cfi_def_cfa_offset 16
.Ltmp269:
	.cfi_offset %rbx, -16
	movq	%rdi, %rbx
	movq	$_ZTVNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEE+16, (%rbx)
	movq	32(%rbx), %rdi
	testq	%rdi, %rdi
	je	.LBB43_2
# BB#1:
	movq	(%rdi), %rax
	movq	(%rax), %rax
.Ltmp262:
	callq	*%rax
.Ltmp263:
.LBB43_2:
	movq	$0, 32(%rbx)
	movq	$_ZTVNSt13__future_base13_State_baseV2E+16, (%rbx)
	movq	8(%rbx), %rdi
	testq	%rdi, %rdi
	je	.LBB43_4
# BB#3:
	movq	(%rdi), %rax
	movq	(%rax), %rax
.Ltmp265:
	callq	*%rax
.Ltmp266:
.LBB43_4:
	movq	%rbx, %rdi
	popq	%rbx
	jmp	_ZdlPv                  # TAILCALL
.LBB43_6:
.Ltmp267:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.LBB43_5:
.Ltmp264:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.Lfunc_end43:
	.size	_ZNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiED0Ev, .Lfunc_end43-_ZNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiED0Ev
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table43:
.Lexception17:
	.byte	255                     # @LPStart Encoding = omit
	.byte	3                       # @TType Encoding = udata4
	.asciz	"\242\200\200"          # @TType base offset
	.byte	3                       # Call site Encoding = udata4
	.byte	26                      # Call site table length
	.long	.Ltmp262-.Lfunc_begin17 # >> Call Site 1 <<
	.long	.Ltmp263-.Ltmp262       #   Call between .Ltmp262 and .Ltmp263
	.long	.Ltmp264-.Lfunc_begin17 #     jumps to .Ltmp264
	.byte	1                       #   On action: 1
	.long	.Ltmp265-.Lfunc_begin17 # >> Call Site 2 <<
	.long	.Ltmp266-.Ltmp265       #   Call between .Ltmp265 and .Ltmp266
	.long	.Ltmp267-.Lfunc_begin17 #     jumps to .Ltmp267
	.byte	1                       #   On action: 1
	.byte	1                       # >> Action Record 1 <<
                                        #   Catch TypeInfo 1
	.byte	0                       #   No further actions
                                        # >> Catch TypeInfos <<
	.long	0                       # TypeInfo 1
	.p2align	2

	.section	.text._ZNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiE17_M_complete_asyncEv,"axG",@progbits,_ZNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiE17_M_complete_asyncEv,comdat
	.weak	_ZNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiE17_M_complete_asyncEv
	.p2align	4, 0x90
	.type	_ZNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiE17_M_complete_asyncEv,@function
_ZNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiE17_M_complete_asyncEv: # @_ZNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiE17_M_complete_asyncEv
.Lfunc_begin18:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception18
# BB#0:
	pushq	%rbx
.Ltmp283:
	.cfi_def_cfa_offset 16
	subq	$80, %rsp
.Ltmp284:
	.cfi_def_cfa_offset 96
.Ltmp285:
	.cfi_offset %rbx, -16
	movq	%rdi, %rbx
	leaq	32(%rbx), %rax
	leaq	40(%rbx), %rcx
	movq	%rax, 8(%rsp)
	movq	%rcx, 16(%rsp)
	movq	$_ZNSt17_Function_handlerIFSt10unique_ptrINSt13__future_base12_Result_baseENS2_8_DeleterEEvENS1_12_Task_setterIS0_INS1_7_ResultIiEES3_ESt12_Bind_simpleIFPFivEvEEiEEE9_M_invokeERKSt9_Any_data, 32(%rsp)
	movq	$_ZNSt14_Function_base13_Base_managerINSt13__future_base12_Task_setterISt10unique_ptrINS1_7_ResultIiEENS1_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEEE10_M_managerERSt9_Any_dataRKSG_St18_Manager_operation, 24(%rsp)
	movb	$0, 7(%rsp)
	leaq	7(%rsp), %rax
	movq	%rax, 40(%rsp)
	leaq	8(%rsp), %rax
	movq	%rax, 48(%rsp)
	movq	%rbx, 56(%rsp)
	movl	$_ZNSt13__future_base13_State_baseV29_M_do_setEPSt8functionIFSt10unique_ptrINS_12_Result_baseENS3_8_DeleterEEvEEPb, %eax
	movd	%rax, %xmm0
	movups	%xmm0, 64(%rsp)
	movq	_ZSt15__once_callable@GOTTPOFF(%rip), %rax
	leaq	40(%rsp), %rcx
	movq	%rcx, %fs:(%rax)
	movq	_ZSt11__once_call@GOTTPOFF(%rip), %rax
	movq	$_ZSt16__once_call_implISt12_Bind_simpleIFSt7_Mem_fnIMNSt13__future_base13_State_baseV2EFvPSt8functionIFSt10unique_ptrINS2_12_Result_baseENS6_8_DeleterEEvEEPbEEPS3_SB_SC_EEEvv, %fs:(%rax)
	movl	$-1, %eax
	movl	$__pthread_key_create, %ecx
	testq	%rcx, %rcx
	je	.LBB44_3
# BB#1:
	leaq	24(%rbx), %rdi
.Ltmp270:
	movl	$__once_proxy, %esi
	callq	pthread_once
.Ltmp271:
# BB#2:
	testl	%eax, %eax
	jne	.LBB44_3
# BB#5:
	cmpb	$0, 7(%rsp)
	je	.LBB44_8
# BB#6:
	movl	$1, %eax
	xchgl	%eax, 16(%rbx)
	testl	%eax, %eax
	jns	.LBB44_8
# BB#7:
	addq	$16, %rbx
.Ltmp274:
	movq	%rbx, %rdi
	callq	_ZNSt28__atomic_futex_unsigned_base19_M_futex_notify_allEPj
.Ltmp275:
.LBB44_8:
	movq	24(%rsp), %rax
	testq	%rax, %rax
	je	.LBB44_10
# BB#9:
.Ltmp280:
	leaq	8(%rsp), %rdi
	movl	$3, %edx
	movq	%rdi, %rsi
	callq	*%rax
.Ltmp281:
.LBB44_10:
	addq	$80, %rsp
	popq	%rbx
	retq
.LBB44_3:
.Ltmp272:
	movl	%eax, %edi
	callq	_ZSt20__throw_system_errori
.Ltmp273:
# BB#4:
.LBB44_14:
.Ltmp282:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.LBB44_11:
.Ltmp276:
	movq	%rax, %rbx
	movq	24(%rsp), %rax
	testq	%rax, %rax
	je	.LBB44_13
# BB#12:
.Ltmp277:
	leaq	8(%rsp), %rdi
	movl	$3, %edx
	movq	%rdi, %rsi
	callq	*%rax
.Ltmp278:
.LBB44_13:
	movq	%rbx, %rdi
	callq	_Unwind_Resume
.LBB44_15:
.Ltmp279:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.Lfunc_end44:
	.size	_ZNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiE17_M_complete_asyncEv, .Lfunc_end44-_ZNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiE17_M_complete_asyncEv
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table44:
.Lexception18:
	.byte	255                     # @LPStart Encoding = omit
	.byte	3                       # @TType Encoding = udata4
	.byte	73                      # @TType base offset
	.byte	3                       # Call site Encoding = udata4
	.byte	65                      # Call site table length
	.long	.Ltmp270-.Lfunc_begin18 # >> Call Site 1 <<
	.long	.Ltmp275-.Ltmp270       #   Call between .Ltmp270 and .Ltmp275
	.long	.Ltmp276-.Lfunc_begin18 #     jumps to .Ltmp276
	.byte	0                       #   On action: cleanup
	.long	.Ltmp280-.Lfunc_begin18 # >> Call Site 2 <<
	.long	.Ltmp281-.Ltmp280       #   Call between .Ltmp280 and .Ltmp281
	.long	.Ltmp282-.Lfunc_begin18 #     jumps to .Ltmp282
	.byte	1                       #   On action: 1
	.long	.Ltmp272-.Lfunc_begin18 # >> Call Site 3 <<
	.long	.Ltmp273-.Ltmp272       #   Call between .Ltmp272 and .Ltmp273
	.long	.Ltmp276-.Lfunc_begin18 #     jumps to .Ltmp276
	.byte	0                       #   On action: cleanup
	.long	.Ltmp277-.Lfunc_begin18 # >> Call Site 4 <<
	.long	.Ltmp278-.Ltmp277       #   Call between .Ltmp277 and .Ltmp278
	.long	.Ltmp279-.Lfunc_begin18 #     jumps to .Ltmp279
	.byte	1                       #   On action: 1
	.long	.Ltmp278-.Lfunc_begin18 # >> Call Site 5 <<
	.long	.Lfunc_end44-.Ltmp278   #   Call between .Ltmp278 and .Lfunc_end44
	.long	0                       #     has no landing pad
	.byte	0                       #   On action: cleanup
	.byte	1                       # >> Action Record 1 <<
                                        #   Catch TypeInfo 1
	.byte	0                       #   No further actions
                                        # >> Catch TypeInfos <<
	.long	0                       # TypeInfo 1
	.p2align	2

	.section	.text._ZNKSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiE21_M_is_deferred_futureEv,"axG",@progbits,_ZNKSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiE21_M_is_deferred_futureEv,comdat
	.weak	_ZNKSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiE21_M_is_deferred_futureEv
	.p2align	4, 0x90
	.type	_ZNKSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiE21_M_is_deferred_futureEv,@function
_ZNKSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiE21_M_is_deferred_futureEv: # @_ZNKSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiE21_M_is_deferred_futureEv
	.cfi_startproc
# BB#0:
	movb	$1, %al
	retq
.Lfunc_end45:
	.size	_ZNKSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiE21_M_is_deferred_futureEv, .Lfunc_end45-_ZNKSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiE21_M_is_deferred_futureEv
	.cfi_endproc

	.section	.text._ZNKSt14__basic_futureIiE13_M_get_resultEv,"axG",@progbits,_ZNKSt14__basic_futureIiE13_M_get_resultEv,comdat
	.weak	_ZNKSt14__basic_futureIiE13_M_get_resultEv
	.p2align	4, 0x90
	.type	_ZNKSt14__basic_futureIiE13_M_get_resultEv,@function
_ZNKSt14__basic_futureIiE13_M_get_resultEv: # @_ZNKSt14__basic_futureIiE13_M_get_resultEv
.Lfunc_begin19:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception19
# BB#0:
	pushq	%rbp
.Ltmp289:
	.cfi_def_cfa_offset 16
	pushq	%r15
.Ltmp290:
	.cfi_def_cfa_offset 24
	pushq	%r14
.Ltmp291:
	.cfi_def_cfa_offset 32
	pushq	%rbx
.Ltmp292:
	.cfi_def_cfa_offset 40
	subq	$24, %rsp
.Ltmp293:
	.cfi_def_cfa_offset 64
.Ltmp294:
	.cfi_offset %rbx, -40
.Ltmp295:
	.cfi_offset %r14, -32
.Ltmp296:
	.cfi_offset %r15, -24
.Ltmp297:
	.cfi_offset %rbp, -16
	movq	(%rdi), %r14
	testq	%r14, %r14
	je	.LBB46_10
# BB#1:
	movq	(%r14), %rax
	movq	%r14, %rdi
	callq	*16(%rax)
	movl	16(%r14), %edx
	andl	$2147483647, %edx       # imm = 0x7FFFFFFF
	cmpl	$1, %edx
	je	.LBB46_5
# BB#2:
	leaq	16(%r14), %rbx
	.p2align	4, 0x90
.LBB46_3:                               # =>This Inner Loop Header: Depth=1
	lock		orl	$-2147483648, (%rbx) # imm = 0x80000000
	orl	$-2147483648, %edx      # imm = 0x80000000
	xorl	%ecx, %ecx
	xorl	%r8d, %r8d
	xorl	%r9d, %r9d
	movq	%rbx, %rdi
	movq	%rbx, %rsi
	callq	_ZNSt28__atomic_futex_unsigned_base19_M_futex_wait_untilEPjjbNSt6chrono8durationIlSt5ratioILl1ELl1EEEENS2_IlS3_ILl1ELl1000000000EEEE
	movl	(%rbx), %edx
	testb	%al, %al
	je	.LBB46_5
# BB#4:                                 #   in Loop: Header=BB46_3 Depth=1
	andl	$2147483647, %edx       # imm = 0x7FFFFFFF
	cmpl	$1, %edx
	jne	.LBB46_3
.LBB46_5:
	movq	8(%r14), %rbx
	leaq	8(%rbx), %r14
	movq	$0, 16(%rsp)
	leaq	16(%rsp), %r15
	movq	%r14, %rdi
	movq	%r15, %rsi
	callq	_ZNSt15__exception_ptreqERKNS_13exception_ptrES2_
	movl	%eax, %ebp
	movq	%r15, %rdi
	callq	_ZNSt15__exception_ptr13exception_ptrD1Ev
	testb	%bpl, %bpl
	je	.LBB46_6
# BB#9:
	movq	%rbx, %rax
	addq	$24, %rsp
	popq	%rbx
	popq	%r14
	popq	%r15
	popq	%rbp
	retq
.LBB46_10:
	movl	$3, %edi
	callq	_ZSt20__throw_future_errori
.LBB46_6:
	leaq	8(%rsp), %rbx
	movq	%rbx, %rdi
	movq	%r14, %rsi
	callq	_ZNSt15__exception_ptr13exception_ptrC1ERKS0_
.Ltmp286:
	movq	%rbx, %rdi
	callq	_ZSt17rethrow_exceptionNSt15__exception_ptr13exception_ptrE
.Ltmp287:
# BB#7:
.LBB46_8:
.Ltmp288:
	movq	%rax, %rbx
	leaq	8(%rsp), %rdi
	callq	_ZNSt15__exception_ptr13exception_ptrD1Ev
	movq	%rbx, %rdi
	callq	_Unwind_Resume
.Lfunc_end46:
	.size	_ZNKSt14__basic_futureIiE13_M_get_resultEv, .Lfunc_end46-_ZNKSt14__basic_futureIiE13_M_get_resultEv
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table46:
.Lexception19:
	.byte	255                     # @LPStart Encoding = omit
	.byte	3                       # @TType Encoding = udata4
	.byte	41                      # @TType base offset
	.byte	3                       # Call site Encoding = udata4
	.byte	39                      # Call site table length
	.long	.Lfunc_begin19-.Lfunc_begin19 # >> Call Site 1 <<
	.long	.Ltmp286-.Lfunc_begin19 #   Call between .Lfunc_begin19 and .Ltmp286
	.long	0                       #     has no landing pad
	.byte	0                       #   On action: cleanup
	.long	.Ltmp286-.Lfunc_begin19 # >> Call Site 2 <<
	.long	.Ltmp287-.Ltmp286       #   Call between .Ltmp286 and .Ltmp287
	.long	.Ltmp288-.Lfunc_begin19 #     jumps to .Ltmp288
	.byte	0                       #   On action: cleanup
	.long	.Ltmp287-.Lfunc_begin19 # >> Call Site 3 <<
	.long	.Lfunc_end46-.Ltmp287   #   Call between .Ltmp287 and .Lfunc_end46
	.long	0                       #     has no landing pad
	.byte	0                       #   On action: cleanup
	.p2align	2

	.type	_ZTSSt19_Sp_make_shared_tag,@object # @_ZTSSt19_Sp_make_shared_tag
	.section	.rodata._ZTSSt19_Sp_make_shared_tag,"aG",@progbits,_ZTSSt19_Sp_make_shared_tag,comdat
	.weak	_ZTSSt19_Sp_make_shared_tag
	.p2align	4
_ZTSSt19_Sp_make_shared_tag:
	.asciz	"St19_Sp_make_shared_tag"
	.size	_ZTSSt19_Sp_make_shared_tag, 24

	.type	_ZTISt19_Sp_make_shared_tag,@object # @_ZTISt19_Sp_make_shared_tag
	.section	.rodata._ZTISt19_Sp_make_shared_tag,"aG",@progbits,_ZTISt19_Sp_make_shared_tag,comdat
	.weak	_ZTISt19_Sp_make_shared_tag
	.p2align	3
_ZTISt19_Sp_make_shared_tag:
	.quad	_ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	_ZTSSt19_Sp_make_shared_tag
	.size	_ZTISt19_Sp_make_shared_tag, 16

	.type	_ZTVSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE,@object # @_ZTVSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE
	.section	.rodata._ZTVSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE,"aG",@progbits,_ZTVSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE,comdat
	.weak	_ZTVSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE
	.p2align	3
_ZTVSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE:
	.quad	0
	.quad	_ZTISt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE
	.quad	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED2Ev
	.quad	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED0Ev
	.quad	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv
	.quad	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv
	.quad	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info
	.size	_ZTVSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE, 56

	.type	_ZTSSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE,@object # @_ZTSSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE
	.section	.rodata._ZTSSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE,"aG",@progbits,_ZTSSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE,comdat
	.weak	_ZTSSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE
	.p2align	4
_ZTSSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE:
	.asciz	"St23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE"
	.size	_ZTSSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE, 133

	.type	_ZTSSt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE,@object # @_ZTSSt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE
	.section	.rodata._ZTSSt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE,"aG",@progbits,_ZTSSt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE,comdat
	.weak	_ZTSSt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE
	.p2align	4
_ZTSSt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE:
	.asciz	"St16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE"
	.size	_ZTSSt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE, 52

	.type	_ZTSSt11_Mutex_baseILN9__gnu_cxx12_Lock_policyE2EE,@object # @_ZTSSt11_Mutex_baseILN9__gnu_cxx12_Lock_policyE2EE
	.section	.rodata._ZTSSt11_Mutex_baseILN9__gnu_cxx12_Lock_policyE2EE,"aG",@progbits,_ZTSSt11_Mutex_baseILN9__gnu_cxx12_Lock_policyE2EE,comdat
	.weak	_ZTSSt11_Mutex_baseILN9__gnu_cxx12_Lock_policyE2EE
	.p2align	4
_ZTSSt11_Mutex_baseILN9__gnu_cxx12_Lock_policyE2EE:
	.asciz	"St11_Mutex_baseILN9__gnu_cxx12_Lock_policyE2EE"
	.size	_ZTSSt11_Mutex_baseILN9__gnu_cxx12_Lock_policyE2EE, 47

	.type	_ZTISt11_Mutex_baseILN9__gnu_cxx12_Lock_policyE2EE,@object # @_ZTISt11_Mutex_baseILN9__gnu_cxx12_Lock_policyE2EE
	.section	.rodata._ZTISt11_Mutex_baseILN9__gnu_cxx12_Lock_policyE2EE,"aG",@progbits,_ZTISt11_Mutex_baseILN9__gnu_cxx12_Lock_policyE2EE,comdat
	.weak	_ZTISt11_Mutex_baseILN9__gnu_cxx12_Lock_policyE2EE
	.p2align	3
_ZTISt11_Mutex_baseILN9__gnu_cxx12_Lock_policyE2EE:
	.quad	_ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	_ZTSSt11_Mutex_baseILN9__gnu_cxx12_Lock_policyE2EE
	.size	_ZTISt11_Mutex_baseILN9__gnu_cxx12_Lock_policyE2EE, 16

	.type	_ZTISt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE,@object # @_ZTISt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE
	.section	.rodata._ZTISt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE,"aG",@progbits,_ZTISt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE,comdat
	.weak	_ZTISt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE
	.p2align	4
_ZTISt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE:
	.quad	_ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	_ZTSSt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE
	.quad	_ZTISt11_Mutex_baseILN9__gnu_cxx12_Lock_policyE2EE
	.size	_ZTISt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE, 24

	.type	_ZTISt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE,@object # @_ZTISt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE
	.section	.rodata._ZTISt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE,"aG",@progbits,_ZTISt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE,comdat
	.weak	_ZTISt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE
	.p2align	4
_ZTISt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE:
	.quad	_ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	_ZTSSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE
	.quad	_ZTISt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE
	.size	_ZTISt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE, 24

	.type	_ZTVNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEE,@object # @_ZTVNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEE
	.section	.rodata._ZTVNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEE,"aG",@progbits,_ZTVNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEE,comdat
	.weak	_ZTVNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEE
	.p2align	3
_ZTVNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEE:
	.quad	0
	.quad	_ZTINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEE
	.quad	_ZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiED2Ev
	.quad	_ZNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiED0Ev
	.quad	_ZNSt13__future_base21_Async_state_commonV217_M_complete_asyncEv
	.quad	_ZNKSt13__future_base13_State_baseV221_M_is_deferred_futureEv
	.size	_ZTVNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEE, 48

	.type	_ZTSNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEE,@object # @_ZTSNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEE
	.section	.rodata._ZTSNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEE,"aG",@progbits,_ZTSNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEE,comdat
	.weak	_ZTSNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEE
	.p2align	4
_ZTSNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEE:
	.asciz	"NSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEE"
	.size	_ZTSNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEE, 68

	.type	_ZTSNSt13__future_base21_Async_state_commonV2E,@object # @_ZTSNSt13__future_base21_Async_state_commonV2E
	.section	.rodata._ZTSNSt13__future_base21_Async_state_commonV2E,"aG",@progbits,_ZTSNSt13__future_base21_Async_state_commonV2E,comdat
	.weak	_ZTSNSt13__future_base21_Async_state_commonV2E
	.p2align	4
_ZTSNSt13__future_base21_Async_state_commonV2E:
	.asciz	"NSt13__future_base21_Async_state_commonV2E"
	.size	_ZTSNSt13__future_base21_Async_state_commonV2E, 43

	.type	_ZTSNSt13__future_base13_State_baseV2E,@object # @_ZTSNSt13__future_base13_State_baseV2E
	.section	.rodata._ZTSNSt13__future_base13_State_baseV2E,"aG",@progbits,_ZTSNSt13__future_base13_State_baseV2E,comdat
	.weak	_ZTSNSt13__future_base13_State_baseV2E
	.p2align	4
_ZTSNSt13__future_base13_State_baseV2E:
	.asciz	"NSt13__future_base13_State_baseV2E"
	.size	_ZTSNSt13__future_base13_State_baseV2E, 35

	.type	_ZTINSt13__future_base13_State_baseV2E,@object # @_ZTINSt13__future_base13_State_baseV2E
	.section	.rodata._ZTINSt13__future_base13_State_baseV2E,"aG",@progbits,_ZTINSt13__future_base13_State_baseV2E,comdat
	.weak	_ZTINSt13__future_base13_State_baseV2E
	.p2align	3
_ZTINSt13__future_base13_State_baseV2E:
	.quad	_ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	_ZTSNSt13__future_base13_State_baseV2E
	.size	_ZTINSt13__future_base13_State_baseV2E, 16

	.type	_ZTINSt13__future_base21_Async_state_commonV2E,@object # @_ZTINSt13__future_base21_Async_state_commonV2E
	.section	.rodata._ZTINSt13__future_base21_Async_state_commonV2E,"aG",@progbits,_ZTINSt13__future_base21_Async_state_commonV2E,comdat
	.weak	_ZTINSt13__future_base21_Async_state_commonV2E
	.p2align	4
_ZTINSt13__future_base21_Async_state_commonV2E:
	.quad	_ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	_ZTSNSt13__future_base21_Async_state_commonV2E
	.quad	_ZTINSt13__future_base13_State_baseV2E
	.size	_ZTINSt13__future_base21_Async_state_commonV2E, 24

	.type	_ZTINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEE,@object # @_ZTINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEE
	.section	.rodata._ZTINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEE,"aG",@progbits,_ZTINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEE,comdat
	.weak	_ZTINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEE
	.p2align	4
_ZTINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEE:
	.quad	_ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	_ZTSNSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEE
	.quad	_ZTINSt13__future_base21_Async_state_commonV2E
	.size	_ZTINSt13__future_base17_Async_state_implISt12_Bind_simpleIFPFivEvEEiEE, 24

	.type	_ZTVNSt13__future_base21_Async_state_commonV2E,@object # @_ZTVNSt13__future_base21_Async_state_commonV2E
	.section	.rodata._ZTVNSt13__future_base21_Async_state_commonV2E,"aG",@progbits,_ZTVNSt13__future_base21_Async_state_commonV2E,comdat
	.weak	_ZTVNSt13__future_base21_Async_state_commonV2E
	.p2align	3
_ZTVNSt13__future_base21_Async_state_commonV2E:
	.quad	0
	.quad	_ZTINSt13__future_base21_Async_state_commonV2E
	.quad	_ZNSt13__future_base21_Async_state_commonV2D2Ev
	.quad	_ZNSt13__future_base21_Async_state_commonV2D0Ev
	.quad	_ZNSt13__future_base21_Async_state_commonV217_M_complete_asyncEv
	.quad	_ZNKSt13__future_base13_State_baseV221_M_is_deferred_futureEv
	.size	_ZTVNSt13__future_base21_Async_state_commonV2E, 48

	.type	_ZTVNSt13__future_base13_State_baseV2E,@object # @_ZTVNSt13__future_base13_State_baseV2E
	.section	.rodata._ZTVNSt13__future_base13_State_baseV2E,"aG",@progbits,_ZTVNSt13__future_base13_State_baseV2E,comdat
	.weak	_ZTVNSt13__future_base13_State_baseV2E
	.p2align	3
_ZTVNSt13__future_base13_State_baseV2E:
	.quad	0
	.quad	_ZTINSt13__future_base13_State_baseV2E
	.quad	_ZNSt13__future_base13_State_baseV2D2Ev
	.quad	_ZNSt13__future_base13_State_baseV2D0Ev
	.quad	_ZNSt13__future_base13_State_baseV217_M_complete_asyncEv
	.quad	_ZNKSt13__future_base13_State_baseV221_M_is_deferred_futureEv
	.size	_ZTVNSt13__future_base13_State_baseV2E, 48

	.type	_ZTVNSt13__future_base7_ResultIiEE,@object # @_ZTVNSt13__future_base7_ResultIiEE
	.section	.rodata._ZTVNSt13__future_base7_ResultIiEE,"aG",@progbits,_ZTVNSt13__future_base7_ResultIiEE,comdat
	.weak	_ZTVNSt13__future_base7_ResultIiEE
	.p2align	3
_ZTVNSt13__future_base7_ResultIiEE:
	.quad	0
	.quad	_ZTINSt13__future_base7_ResultIiEE
	.quad	_ZNSt13__future_base7_ResultIiE10_M_destroyEv
	.quad	_ZNSt13__future_base7_ResultIiED2Ev
	.quad	_ZNSt13__future_base7_ResultIiED0Ev
	.size	_ZTVNSt13__future_base7_ResultIiEE, 40

	.type	_ZTSNSt13__future_base7_ResultIiEE,@object # @_ZTSNSt13__future_base7_ResultIiEE
	.section	.rodata._ZTSNSt13__future_base7_ResultIiEE,"aG",@progbits,_ZTSNSt13__future_base7_ResultIiEE,comdat
	.weak	_ZTSNSt13__future_base7_ResultIiEE
	.p2align	4
_ZTSNSt13__future_base7_ResultIiEE:
	.asciz	"NSt13__future_base7_ResultIiEE"
	.size	_ZTSNSt13__future_base7_ResultIiEE, 31

	.type	_ZTINSt13__future_base7_ResultIiEE,@object # @_ZTINSt13__future_base7_ResultIiEE
	.section	.rodata._ZTINSt13__future_base7_ResultIiEE,"aG",@progbits,_ZTINSt13__future_base7_ResultIiEE,comdat
	.weak	_ZTINSt13__future_base7_ResultIiEE
	.p2align	4
_ZTINSt13__future_base7_ResultIiEE:
	.quad	_ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	_ZTSNSt13__future_base7_ResultIiEE
	.quad	_ZTINSt13__future_base12_Result_baseE
	.size	_ZTINSt13__future_base7_ResultIiEE, 24

	.type	_ZTVNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEEE,@object # @_ZTVNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEEE
	.section	.rodata._ZTVNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEEE,"aG",@progbits,_ZTVNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEEE,comdat
	.weak	_ZTVNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEEE
	.p2align	3
_ZTVNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEEE:
	.quad	0
	.quad	_ZTINSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEEE
	.quad	_ZNSt6thread6_StateD2Ev
	.quad	_ZNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEED0Ev
	.quad	_ZNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEE6_M_runEv
	.size	_ZTVNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEEE, 40

	.type	_ZTSNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEEE,@object # @_ZTSNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEEE
	.section	.rodata._ZTSNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEEE,"aG",@progbits,_ZTSNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEEE,comdat
	.weak	_ZTSNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEEE
	.p2align	4
_ZTSNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEEE:
	.asciz	"NSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEEE"
	.size	_ZTSNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEEE, 115

	.type	_ZTINSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEEE,@object # @_ZTINSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEEE
	.section	.rodata._ZTINSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEEE,"aG",@progbits,_ZTINSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEEE,comdat
	.weak	_ZTINSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEEE
	.p2align	4
_ZTINSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEEE:
	.quad	_ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	_ZTSNSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEEE
	.quad	_ZTINSt6thread6_StateE
	.size	_ZTINSt6thread11_State_implISt12_Bind_simpleIFZNSt13__future_base17_Async_state_implIS1_IFPFivEvEEiEC1EOS7_EUlvE_vEEEE, 24

	.type	_ZTSNSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEE,@object # @_ZTSNSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEE
	.section	.rodata._ZTSNSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEE,"aG",@progbits,_ZTSNSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEE,comdat
	.weak	_ZTSNSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEE
	.p2align	4
_ZTSNSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEE:
	.asciz	"NSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEE"
	.size	_ZTSNSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEE, 121

	.type	_ZTINSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEE,@object # @_ZTINSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEE
	.section	.rodata._ZTINSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEE,"aG",@progbits,_ZTINSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEE,comdat
	.weak	_ZTINSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEE
	.p2align	3
_ZTINSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEE:
	.quad	_ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	_ZTSNSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEE
	.size	_ZTINSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEESt12_Bind_simpleIFPFivEvEEiEE, 16

	.type	.L.str,@object          # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"std::future_error: "
	.size	.L.str, 20

	.type	_ZTVSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE,@object # @_ZTVSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE
	.section	.rodata._ZTVSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE,"aG",@progbits,_ZTVSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE,comdat
	.weak	_ZTVSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE
	.p2align	3
_ZTVSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE:
	.quad	0
	.quad	_ZTISt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE
	.quad	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED2Ev
	.quad	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EED0Ev
	.quad	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv
	.quad	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv
	.quad	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info
	.size	_ZTVSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE, 56

	.type	_ZTSSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE,@object # @_ZTSSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE
	.section	.rodata._ZTSSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE,"aG",@progbits,_ZTSSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE,comdat
	.weak	_ZTSSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE
	.p2align	4
_ZTSSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE:
	.asciz	"St23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE"
	.size	_ZTSSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE, 131

	.type	_ZTISt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE,@object # @_ZTISt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE
	.section	.rodata._ZTISt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE,"aG",@progbits,_ZTISt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE,comdat
	.weak	_ZTISt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE
	.p2align	4
_ZTISt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE:
	.quad	_ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	_ZTSSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE
	.quad	_ZTISt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE
	.size	_ZTISt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEESaIS7_ELN9__gnu_cxx12_Lock_policyE2EE, 24

	.type	_ZTVNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEE,@object # @_ZTVNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEE
	.section	.rodata._ZTVNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEE,"aG",@progbits,_ZTVNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEE,comdat
	.weak	_ZTVNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEE
	.p2align	3
_ZTVNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEE:
	.quad	0
	.quad	_ZTINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEE
	.quad	_ZNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiED2Ev
	.quad	_ZNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiED0Ev
	.quad	_ZNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiE17_M_complete_asyncEv
	.quad	_ZNKSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiE21_M_is_deferred_futureEv
	.size	_ZTVNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEE, 48

	.type	_ZTSNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEE,@object # @_ZTSNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEE
	.section	.rodata._ZTSNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEE,"aG",@progbits,_ZTSNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEE,comdat
	.weak	_ZTSNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEE
	.p2align	4
_ZTSNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEE:
	.asciz	"NSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEE"
	.size	_ZTSNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEE, 66

	.type	_ZTINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEE,@object # @_ZTINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEE
	.section	.rodata._ZTINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEE,"aG",@progbits,_ZTINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEE,comdat
	.weak	_ZTINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEE
	.p2align	4
_ZTINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEE:
	.quad	_ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	_ZTSNSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEE
	.quad	_ZTINSt13__future_base13_State_baseV2E
	.size	_ZTINSt13__future_base15_Deferred_stateISt12_Bind_simpleIFPFivEvEEiEE, 24

	.weak	__pthread_key_create
	.weak	pthread_once

	.ident	"clang version 3.9.0 (tags/RELEASE_390/final)"
	.section	".note.GNU-stack","",@progbits
