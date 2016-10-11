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
.Ltmp3:
	.cfi_def_cfa_offset 16
	subq	$48, %rsp
.Ltmp4:
	.cfi_def_cfa_offset 64
.Ltmp5:
	.cfi_offset %rbx, -16
	movq	%fs:40, %rax
	movq	%rax, 40(%rsp)
	leaq	8(%rsp), %rbx
	movq	%rbx, %rdi
	movq	%rbx, %rsi
	callq	_ZNSt8functionIFiiEEC2IZ4mainE3$_0vvEET_
.Ltmp0:
	movl	$6, %esi
	movq	%rbx, %rdi
	callq	_ZNKSt8functionIFiiEEclEi@PLT
.Ltmp1:
# BB#1:
	movl	%eax, 4(%rsp)
	leaq	8(%rsp), %rdi
	callq	_ZNSt14_Function_baseD2Ev@PLT
	movq	%fs:40, %rax
	cmpq	40(%rsp), %rax
	jne	.LBB0_4
# BB#2:                                 # %SP_return
	xorl	%eax, %eax
	addq	$48, %rsp
	popq	%rbx
	retq
.LBB0_3:
.Ltmp2:
	movq	%rax, %rbx
	leaq	8(%rsp), %rdi
	callq	_ZNSt14_Function_baseD2Ev@PLT
	movq	%rbx, %rdi
	callq	_Unwind_Resume@PLT
.LBB0_4:                                # %CallStackCheckFailBlk
	callq	__stack_chk_fail@PLT
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.align	4
GCC_except_table0:
.Lexception0:
	.byte	255                     # @LPStart Encoding = omit
	.byte	155                     # @TType Encoding = indirect pcrel sdata4
	.asciz	"\234"                  # @TType base offset
	.byte	3                       # Call site Encoding = udata4
	.byte	26                      # Call site table length
	.long	.Ltmp0-.Lfunc_begin0    # >> Call Site 1 <<
	.long	.Ltmp1-.Ltmp0           #   Call between .Ltmp0 and .Ltmp1
	.long	.Ltmp2-.Lfunc_begin0    #     jumps to .Ltmp2
	.byte	0                       #   On action: cleanup
	.long	.Ltmp1-.Lfunc_begin0    # >> Call Site 2 <<
	.long	.Lfunc_end0-.Ltmp1      #   Call between .Ltmp1 and .Lfunc_end0
	.long	0                       #     has no landing pad
	.byte	0                       #   On action: cleanup
	.align	4

	.text
	.align	16, 0x90
	.type	_ZNSt8functionIFiiEEC2IZ4mainE3$_0vvEET_,@function
_ZNSt8functionIFiiEEC2IZ4mainE3$_0vvEET_: # @"_ZNSt8functionIFiiEEC2IZ4mainE3$_0vvEET_"
	.cfi_startproc
# BB#0:
	pushq	%rbx
.Ltmp6:
	.cfi_def_cfa_offset 16
	subq	$16, %rsp
.Ltmp7:
	.cfi_def_cfa_offset 32
.Ltmp8:
	.cfi_offset %rbx, -16
	movq	%rdi, %rbx
	movq	%fs:40, %rax
	movq	%rax, 8(%rsp)
	movq	%rsi, (%rsp)
	callq	_ZNSt14_Function_baseC2Ev@PLT
	leaq	(%rsp), %rdi
	callq	_ZSt4moveIRZ4mainE3$_0EONSt16remove_referenceIT_E4typeEOS3_
	movq	%rbx, %rdi
	movq	%rax, %rsi
	callq	_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E15_M_init_functorERSt9_Any_dataOS1_
	leaq	_ZNSt17_Function_handlerIFiiEZ4mainE3$_0E9_M_invokeERKSt9_Any_dataOi(%rip), %rax
	movq	%rax, 24(%rbx)
	leaq	_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E10_M_managerERSt9_Any_dataRKS3_St18_Manager_operation(%rip), %rax
	movq	%rax, 16(%rbx)
	movq	%fs:40, %rax
	cmpq	8(%rsp), %rax
	jne	.LBB1_2
# BB#1:                                 # %SP_return
	addq	$16, %rsp
	popq	%rbx
	retq
.LBB1_2:                                # %CallStackCheckFailBlk
	callq	__stack_chk_fail@PLT
.Lfunc_end1:
	.size	_ZNSt8functionIFiiEEC2IZ4mainE3$_0vvEET_, .Lfunc_end1-_ZNSt8functionIFiiEEC2IZ4mainE3$_0vvEET_
	.cfi_endproc

	.section	.text._ZNKSt8functionIFiiEEclEi,"axG",@progbits,_ZNKSt8functionIFiiEEclEi,comdat
	.weak	_ZNKSt8functionIFiiEEclEi
	.align	16, 0x90
	.type	_ZNKSt8functionIFiiEEclEi,@function
_ZNKSt8functionIFiiEEclEi:              # @_ZNKSt8functionIFiiEEclEi
	.cfi_startproc
# BB#0:
	pushq	%r14
.Ltmp9:
	.cfi_def_cfa_offset 16
	pushq	%rbx
.Ltmp10:
	.cfi_def_cfa_offset 24
	subq	$24, %rsp
.Ltmp11:
	.cfi_def_cfa_offset 48
.Ltmp12:
	.cfi_offset %rbx, -24
.Ltmp13:
	.cfi_offset %r14, -16
	movq	%rdi, %rbx
	movq	%fs:40, %rax
	movq	%rax, 16(%rsp)
	movl	%esi, 12(%rsp)
	callq	_ZNKSt14_Function_base8_M_emptyEv@PLT
	testb	%al, %al
	jne	.LBB2_3
# BB#1:
	movq	24(%rbx), %r14
	leaq	12(%rsp), %rdi
	callq	_ZSt7forwardIiEOT_RNSt16remove_referenceIS0_E4typeE@PLT
	movq	%rbx, %rdi
	movq	%rax, %rsi
	callq	*%r14
	movq	%fs:40, %rcx
	cmpq	16(%rsp), %rcx
	jne	.LBB2_2
# BB#4:                                 # %SP_return
	addq	$24, %rsp
	popq	%rbx
	popq	%r14
	retq
.LBB2_3:
	callq	_ZSt25__throw_bad_function_callv@PLT
.LBB2_2:                                # %CallStackCheckFailBlk
	callq	__stack_chk_fail@PLT
.Lfunc_end2:
	.size	_ZNKSt8functionIFiiEEclEi, .Lfunc_end2-_ZNKSt8functionIFiiEEclEi
	.cfi_endproc

	.section	.text._ZNSt14_Function_baseD2Ev,"axG",@progbits,_ZNSt14_Function_baseD2Ev,comdat
	.weak	_ZNSt14_Function_baseD2Ev
	.align	16, 0x90
	.type	_ZNSt14_Function_baseD2Ev,@function
_ZNSt14_Function_baseD2Ev:              # @_ZNSt14_Function_baseD2Ev
.Lfunc_begin1:
	.cfi_startproc
	.cfi_personality 155, DW.ref.__gxx_personality_v0
	.cfi_lsda 27, .Lexception1
# BB#0:
	pushq	%rax
.Ltmp17:
	.cfi_def_cfa_offset 16
	movq	16(%rdi), %rax
	testq	%rax, %rax
	je	.LBB3_2
# BB#1:
.Ltmp14:
	movl	$3, %edx
	movq	%rdi, %rsi
	callq	*%rax
.Ltmp15:
.LBB3_2:
	popq	%rax
	retq
.LBB3_3:
.Ltmp16:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.Lfunc_end3:
	.size	_ZNSt14_Function_baseD2Ev, .Lfunc_end3-_ZNSt14_Function_baseD2Ev
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.align	4
GCC_except_table3:
.Lexception1:
	.byte	255                     # @LPStart Encoding = omit
	.byte	155                     # @TType Encoding = indirect pcrel sdata4
	.byte	21                      # @TType base offset
	.byte	3                       # Call site Encoding = udata4
	.byte	13                      # Call site table length
	.long	.Ltmp14-.Lfunc_begin1   # >> Call Site 1 <<
	.long	.Ltmp15-.Ltmp14         #   Call between .Ltmp14 and .Ltmp15
	.long	.Ltmp16-.Lfunc_begin1   #     jumps to .Ltmp16
	.byte	1                       #   On action: 1
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
.Lfunc_end4:
	.size	__clang_call_terminate, .Lfunc_end4-__clang_call_terminate

	.section	.text._ZNKSt14_Function_base8_M_emptyEv,"axG",@progbits,_ZNKSt14_Function_base8_M_emptyEv,comdat
	.weak	_ZNKSt14_Function_base8_M_emptyEv
	.align	16, 0x90
	.type	_ZNKSt14_Function_base8_M_emptyEv,@function
_ZNKSt14_Function_base8_M_emptyEv:      # @_ZNKSt14_Function_base8_M_emptyEv
	.cfi_startproc
# BB#0:
	cmpq	$0, 16(%rdi)
	sete	%al
	retq
.Lfunc_end5:
	.size	_ZNKSt14_Function_base8_M_emptyEv, .Lfunc_end5-_ZNKSt14_Function_base8_M_emptyEv
	.cfi_endproc

	.section	.text._ZSt7forwardIiEOT_RNSt16remove_referenceIS0_E4typeE,"axG",@progbits,_ZSt7forwardIiEOT_RNSt16remove_referenceIS0_E4typeE,comdat
	.weak	_ZSt7forwardIiEOT_RNSt16remove_referenceIS0_E4typeE
	.align	16, 0x90
	.type	_ZSt7forwardIiEOT_RNSt16remove_referenceIS0_E4typeE,@function
_ZSt7forwardIiEOT_RNSt16remove_referenceIS0_E4typeE: # @_ZSt7forwardIiEOT_RNSt16remove_referenceIS0_E4typeE
	.cfi_startproc
# BB#0:
	movq	%rdi, %rax
	retq
.Lfunc_end6:
	.size	_ZSt7forwardIiEOT_RNSt16remove_referenceIS0_E4typeE, .Lfunc_end6-_ZSt7forwardIiEOT_RNSt16remove_referenceIS0_E4typeE
	.cfi_endproc

	.section	.text._ZNSt14_Function_baseC2Ev,"axG",@progbits,_ZNSt14_Function_baseC2Ev,comdat
	.weak	_ZNSt14_Function_baseC2Ev
	.align	16, 0x90
	.type	_ZNSt14_Function_baseC2Ev,@function
_ZNSt14_Function_baseC2Ev:              # @_ZNSt14_Function_baseC2Ev
	.cfi_startproc
# BB#0:
	movq	$0, 16(%rdi)
	retq
.Lfunc_end7:
	.size	_ZNSt14_Function_baseC2Ev, .Lfunc_end7-_ZNSt14_Function_baseC2Ev
	.cfi_endproc

	.text
	.align	16, 0x90
	.type	_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E15_M_init_functorERSt9_Any_dataOS1_,@function
_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E15_M_init_functorERSt9_Any_dataOS1_: # @"_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E15_M_init_functorERSt9_Any_dataOS1_"
	.cfi_startproc
# BB#0:
	pushq	%rbx
.Ltmp18:
	.cfi_def_cfa_offset 16
.Ltmp19:
	.cfi_offset %rbx, -16
	movq	%rdi, %rbx
	movq	%rsi, %rdi
	callq	_ZSt4moveIRZ4mainE3$_0EONSt16remove_referenceIT_E4typeEOS3_
	movq	%rbx, %rdi
	movq	%rax, %rsi
	popq	%rbx
	jmp	_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E15_M_init_functorERSt9_Any_dataOS1_St17integral_constantIbLb1EE # TAILCALL
.Lfunc_end8:
	.size	_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E15_M_init_functorERSt9_Any_dataOS1_, .Lfunc_end8-_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E15_M_init_functorERSt9_Any_dataOS1_
	.cfi_endproc

	.align	16, 0x90
	.type	_ZSt4moveIRZ4mainE3$_0EONSt16remove_referenceIT_E4typeEOS3_,@function
_ZSt4moveIRZ4mainE3$_0EONSt16remove_referenceIT_E4typeEOS3_: # @"_ZSt4moveIRZ4mainE3$_0EONSt16remove_referenceIT_E4typeEOS3_"
	.cfi_startproc
# BB#0:
	movq	%rdi, %rax
	retq
.Lfunc_end9:
	.size	_ZSt4moveIRZ4mainE3$_0EONSt16remove_referenceIT_E4typeEOS3_, .Lfunc_end9-_ZSt4moveIRZ4mainE3$_0EONSt16remove_referenceIT_E4typeEOS3_
	.cfi_endproc

	.align	16, 0x90
	.type	_ZNSt17_Function_handlerIFiiEZ4mainE3$_0E9_M_invokeERKSt9_Any_dataOi,@function
_ZNSt17_Function_handlerIFiiEZ4mainE3$_0E9_M_invokeERKSt9_Any_dataOi: # @"_ZNSt17_Function_handlerIFiiEZ4mainE3$_0E9_M_invokeERKSt9_Any_dataOi"
	.cfi_startproc
# BB#0:
	pushq	%r14
.Ltmp20:
	.cfi_def_cfa_offset 16
	pushq	%rbx
.Ltmp21:
	.cfi_def_cfa_offset 24
	pushq	%rax
.Ltmp22:
	.cfi_def_cfa_offset 32
.Ltmp23:
	.cfi_offset %rbx, -24
.Ltmp24:
	.cfi_offset %r14, -16
	movq	%rsi, %r14
	callq	_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E14_M_get_pointerERKSt9_Any_data
	movq	%rax, %rbx
	movq	%r14, %rdi
	callq	_ZSt7forwardIiEOT_RNSt16remove_referenceIS0_E4typeE@PLT
	movl	(%rax), %esi
	movq	%rbx, %rdi
	addq	$8, %rsp
	popq	%rbx
	popq	%r14
	jmp	_ZZ4mainENK3$_0clEi     # TAILCALL
.Lfunc_end10:
	.size	_ZNSt17_Function_handlerIFiiEZ4mainE3$_0E9_M_invokeERKSt9_Any_dataOi, .Lfunc_end10-_ZNSt17_Function_handlerIFiiEZ4mainE3$_0E9_M_invokeERKSt9_Any_dataOi
	.cfi_endproc

	.align	16, 0x90
	.type	_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E10_M_managerERSt9_Any_dataRKS3_St18_Manager_operation,@function
_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E10_M_managerERSt9_Any_dataRKS3_St18_Manager_operation: # @"_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E10_M_managerERSt9_Any_dataRKS3_St18_Manager_operation"
	.cfi_startproc
# BB#0:
	pushq	%r14
.Ltmp25:
	.cfi_def_cfa_offset 16
	pushq	%rbx
.Ltmp26:
	.cfi_def_cfa_offset 24
	pushq	%rax
.Ltmp27:
	.cfi_def_cfa_offset 32
.Ltmp28:
	.cfi_offset %rbx, -24
.Ltmp29:
	.cfi_offset %r14, -16
	movq	%rdi, %rbx
	cmpl	$2, %edx
	je	.LBB11_5
# BB#1:
	cmpl	$1, %edx
	je	.LBB11_4
# BB#2:
	testl	%edx, %edx
	jne	.LBB11_6
# BB#3:
	movq	%rbx, %rdi
	callq	_ZNSt9_Any_data9_M_accessIPKSt9type_infoEERT_v@PLT
	leaq	_ZTIZ4mainE3$_0(%rip), %rcx
	movq	%rcx, (%rax)
	jmp	.LBB11_6
.LBB11_5:
	movq	%rbx, %rdi
	callq	_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E8_M_cloneERSt9_Any_dataRKS3_St17integral_constantIbLb1EE
	jmp	.LBB11_6
.LBB11_4:
	movq	%rsi, %rdi
	callq	_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E14_M_get_pointerERKSt9_Any_data
	movq	%rax, %r14
	movq	%rbx, %rdi
	callq	_ZNSt9_Any_data9_M_accessIPZ4mainE3$_0EERT_v
	movq	%r14, (%rax)
.LBB11_6:
	xorl	%eax, %eax
	addq	$8, %rsp
	popq	%rbx
	popq	%r14
	retq
.Lfunc_end11:
	.size	_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E10_M_managerERSt9_Any_dataRKS3_St18_Manager_operation, .Lfunc_end11-_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E10_M_managerERSt9_Any_dataRKS3_St18_Manager_operation
	.cfi_endproc

	.align	16, 0x90
	.type	_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E15_M_init_functorERSt9_Any_dataOS1_St17integral_constantIbLb1EE,@function
_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E15_M_init_functorERSt9_Any_dataOS1_St17integral_constantIbLb1EE: # @"_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E15_M_init_functorERSt9_Any_dataOS1_St17integral_constantIbLb1EE"
	.cfi_startproc
# BB#0:
	pushq	%r14
.Ltmp30:
	.cfi_def_cfa_offset 16
	pushq	%rbx
.Ltmp31:
	.cfi_def_cfa_offset 24
	pushq	%rax
.Ltmp32:
	.cfi_def_cfa_offset 32
.Ltmp33:
	.cfi_offset %rbx, -24
.Ltmp34:
	.cfi_offset %r14, -16
	movq	%rsi, %r14
	callq	_ZNSt9_Any_data9_M_accessEv@PLT
	movq	%rax, %rbx
	movq	%r14, %rdi
	callq	_ZSt4moveIRZ4mainE3$_0EONSt16remove_referenceIT_E4typeEOS3_
	movq	(%rax), %rax
	movq	%rax, (%rbx)
	addq	$8, %rsp
	popq	%rbx
	popq	%r14
	retq
.Lfunc_end12:
	.size	_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E15_M_init_functorERSt9_Any_dataOS1_St17integral_constantIbLb1EE, .Lfunc_end12-_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E15_M_init_functorERSt9_Any_dataOS1_St17integral_constantIbLb1EE
	.cfi_endproc

	.section	.text._ZNSt9_Any_data9_M_accessEv,"axG",@progbits,_ZNSt9_Any_data9_M_accessEv,comdat
	.weak	_ZNSt9_Any_data9_M_accessEv
	.align	16, 0x90
	.type	_ZNSt9_Any_data9_M_accessEv,@function
_ZNSt9_Any_data9_M_accessEv:            # @_ZNSt9_Any_data9_M_accessEv
	.cfi_startproc
# BB#0:
	movq	%rdi, %rax
	retq
.Lfunc_end13:
	.size	_ZNSt9_Any_data9_M_accessEv, .Lfunc_end13-_ZNSt9_Any_data9_M_accessEv
	.cfi_endproc

	.text
	.align	16, 0x90
	.type	_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E14_M_get_pointerERKSt9_Any_data,@function
_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E14_M_get_pointerERKSt9_Any_data: # @"_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E14_M_get_pointerERKSt9_Any_data"
	.cfi_startproc
# BB#0:
	pushq	%rax
.Ltmp35:
	.cfi_def_cfa_offset 16
	callq	_ZNKSt9_Any_data9_M_accessIZ4mainE3$_0EERKT_v
	movq	%rax, %rdi
	callq	_ZSt11__addressofIKZ4mainE3$_0EPT_RS2_
	popq	%rcx
	retq
.Lfunc_end14:
	.size	_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E14_M_get_pointerERKSt9_Any_data, .Lfunc_end14-_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E14_M_get_pointerERKSt9_Any_data
	.cfi_endproc

	.align	16, 0x90
	.type	_ZZ4mainENK3$_0clEi,@function
_ZZ4mainENK3$_0clEi:                    # @"_ZZ4mainENK3$_0clEi"
	.cfi_startproc
# BB#0:
	pushq	%rbx
.Ltmp36:
	.cfi_def_cfa_offset 16
.Ltmp37:
	.cfi_offset %rbx, -16
	movl	%esi, %ebx
	movl	$1, %eax
	testl	%ebx, %ebx
	je	.LBB15_2
# BB#1:
	movq	(%rdi), %rdi
	leal	-1(%rbx), %esi
	callq	_ZNKSt8functionIFiiEEclEi@PLT
	imull	%ebx, %eax
.LBB15_2:
	popq	%rbx
	retq
.Lfunc_end15:
	.size	_ZZ4mainENK3$_0clEi, .Lfunc_end15-_ZZ4mainENK3$_0clEi
	.cfi_endproc

	.align	16, 0x90
	.type	_ZSt11__addressofIKZ4mainE3$_0EPT_RS2_,@function
_ZSt11__addressofIKZ4mainE3$_0EPT_RS2_: # @"_ZSt11__addressofIKZ4mainE3$_0EPT_RS2_"
	.cfi_startproc
# BB#0:
	movq	%rdi, %rax
	retq
.Lfunc_end16:
	.size	_ZSt11__addressofIKZ4mainE3$_0EPT_RS2_, .Lfunc_end16-_ZSt11__addressofIKZ4mainE3$_0EPT_RS2_
	.cfi_endproc

	.align	16, 0x90
	.type	_ZNKSt9_Any_data9_M_accessIZ4mainE3$_0EERKT_v,@function
_ZNKSt9_Any_data9_M_accessIZ4mainE3$_0EERKT_v: # @"_ZNKSt9_Any_data9_M_accessIZ4mainE3$_0EERKT_v"
	.cfi_startproc
# BB#0:
	pushq	%rax
.Ltmp38:
	.cfi_def_cfa_offset 16
	callq	_ZNKSt9_Any_data9_M_accessEv@PLT
	popq	%rcx
	retq
.Lfunc_end17:
	.size	_ZNKSt9_Any_data9_M_accessIZ4mainE3$_0EERKT_v, .Lfunc_end17-_ZNKSt9_Any_data9_M_accessIZ4mainE3$_0EERKT_v
	.cfi_endproc

	.section	.text._ZNKSt9_Any_data9_M_accessEv,"axG",@progbits,_ZNKSt9_Any_data9_M_accessEv,comdat
	.weak	_ZNKSt9_Any_data9_M_accessEv
	.align	16, 0x90
	.type	_ZNKSt9_Any_data9_M_accessEv,@function
_ZNKSt9_Any_data9_M_accessEv:           # @_ZNKSt9_Any_data9_M_accessEv
	.cfi_startproc
# BB#0:
	movq	%rdi, %rax
	retq
.Lfunc_end18:
	.size	_ZNKSt9_Any_data9_M_accessEv, .Lfunc_end18-_ZNKSt9_Any_data9_M_accessEv
	.cfi_endproc

	.section	.text._ZNSt9_Any_data9_M_accessIPKSt9type_infoEERT_v,"axG",@progbits,_ZNSt9_Any_data9_M_accessIPKSt9type_infoEERT_v,comdat
	.weak	_ZNSt9_Any_data9_M_accessIPKSt9type_infoEERT_v
	.align	16, 0x90
	.type	_ZNSt9_Any_data9_M_accessIPKSt9type_infoEERT_v,@function
_ZNSt9_Any_data9_M_accessIPKSt9type_infoEERT_v: # @_ZNSt9_Any_data9_M_accessIPKSt9type_infoEERT_v
	.cfi_startproc
# BB#0:
	pushq	%rax
.Ltmp39:
	.cfi_def_cfa_offset 16
	callq	_ZNSt9_Any_data9_M_accessEv@PLT
	popq	%rcx
	retq
.Lfunc_end19:
	.size	_ZNSt9_Any_data9_M_accessIPKSt9type_infoEERT_v, .Lfunc_end19-_ZNSt9_Any_data9_M_accessIPKSt9type_infoEERT_v
	.cfi_endproc

	.text
	.align	16, 0x90
	.type	_ZNSt9_Any_data9_M_accessIPZ4mainE3$_0EERT_v,@function
_ZNSt9_Any_data9_M_accessIPZ4mainE3$_0EERT_v: # @"_ZNSt9_Any_data9_M_accessIPZ4mainE3$_0EERT_v"
	.cfi_startproc
# BB#0:
	pushq	%rax
.Ltmp40:
	.cfi_def_cfa_offset 16
	callq	_ZNSt9_Any_data9_M_accessEv@PLT
	popq	%rcx
	retq
.Lfunc_end20:
	.size	_ZNSt9_Any_data9_M_accessIPZ4mainE3$_0EERT_v, .Lfunc_end20-_ZNSt9_Any_data9_M_accessIPZ4mainE3$_0EERT_v
	.cfi_endproc

	.align	16, 0x90
	.type	_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E8_M_cloneERSt9_Any_dataRKS3_St17integral_constantIbLb1EE,@function
_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E8_M_cloneERSt9_Any_dataRKS3_St17integral_constantIbLb1EE: # @"_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E8_M_cloneERSt9_Any_dataRKS3_St17integral_constantIbLb1EE"
	.cfi_startproc
# BB#0:
	pushq	%r14
.Ltmp41:
	.cfi_def_cfa_offset 16
	pushq	%rbx
.Ltmp42:
	.cfi_def_cfa_offset 24
	pushq	%rax
.Ltmp43:
	.cfi_def_cfa_offset 32
.Ltmp44:
	.cfi_offset %rbx, -24
.Ltmp45:
	.cfi_offset %r14, -16
	movq	%rsi, %r14
	callq	_ZNSt9_Any_data9_M_accessEv@PLT
	movq	%rax, %rbx
	movq	%r14, %rdi
	callq	_ZNKSt9_Any_data9_M_accessIZ4mainE3$_0EERKT_v
	movq	(%rax), %rax
	movq	%rax, (%rbx)
	addq	$8, %rsp
	popq	%rbx
	popq	%r14
	retq
.Lfunc_end21:
	.size	_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E8_M_cloneERSt9_Any_dataRKS3_St17integral_constantIbLb1EE, .Lfunc_end21-_ZNSt14_Function_base13_Base_managerIZ4mainE3$_0E8_M_cloneERSt9_Any_dataRKS3_St17integral_constantIbLb1EE
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
