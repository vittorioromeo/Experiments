	.file	"x.cpp"
	.text
	.type	_ZNSt14_Function_base13_Base_managerIZ4mainEUliE_E10_M_managerERSt9_Any_dataRKS3_St18_Manager_operation, @function
_ZNSt14_Function_base13_Base_managerIZ4mainEUliE_E10_M_managerERSt9_Any_dataRKS3_St18_Manager_operation:
.LFB2929:
	.cfi_startproc
	cmpl	$1, %edx
	je	.L3
	cmpl	$1, %edx
	jb	.L4
	cmpl	$2, %edx
	je	.L5
.L2:
	movl	$0, %eax
	ret
.L4:
	leaq	_ZTIZ4mainEUliE_(%rip), %rax
	movq	%rax, (%rdi)
	jmp	.L2
.L3:
	movq	%rsi, (%rdi)
	jmp	.L2
.L5:
	testq	%rdi, %rdi
	je	.L2
	movq	(%rsi), %rax
	movq	%rax, (%rdi)
	jmp	.L2
	.cfi_endproc
.LFE2929:
	.size	_ZNSt14_Function_base13_Base_managerIZ4mainEUliE_E10_M_managerERSt9_Any_dataRKS3_St18_Manager_operation, .-_ZNSt14_Function_base13_Base_managerIZ4mainEUliE_E10_M_managerERSt9_Any_dataRKS3_St18_Manager_operation
	.section	.text._ZNKSt8functionIFiiEEclEi,"axG",@progbits,_ZNKSt8functionIFiiEEclEi,comdat
	.align 2
	.weak	_ZNKSt8functionIFiiEEclEi
	.type	_ZNKSt8functionIFiiEEclEi, @function
_ZNKSt8functionIFiiEEclEi:
.LFB2829:
	.cfi_startproc
	leaq	-4152(%rsp), %rsp
	orq	$0, (%rsp)
	leaq	4128(%rsp), %rsp
	.cfi_def_cfa_offset 32
	movl	%esi, 12(%rsp)
	cmpq	$0, 16(%rdi)
	je	.L9
	leaq	12(%rsp), %rsi
	call	*24(%rdi)
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L9:
	.cfi_restore_state
	call	_ZSt25__throw_bad_function_callv@PLT
	.cfi_endproc
.LFE2829:
	.size	_ZNKSt8functionIFiiEEclEi, .-_ZNKSt8functionIFiiEEclEi
	.text
	.type	_ZNSt17_Function_handlerIFiiEZ4mainEUliE_E9_M_invokeERKSt9_Any_dataOi, @function
_ZNSt17_Function_handlerIFiiEZ4mainEUliE_E9_M_invokeERKSt9_Any_dataOi:
.LFB2928:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	leaq	-4128(%rsp), %rsp
	orq	$0, (%rsp)
	leaq	4128(%rsp), %rsp
	movl	(%rsi), %ebx
	movl	$1, %eax
	testl	%ebx, %ebx
	jne	.L14
.L10:
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L14:
	.cfi_restore_state
	leal	-1(%rbx), %esi
	movq	(%rdi), %rdi
	call	_ZNKSt8functionIFiiEEclEi
	imull	%ebx, %eax
	jmp	.L10
	.cfi_endproc
.LFE2928:
	.size	_ZNSt17_Function_handlerIFiiEZ4mainEUliE_E9_M_invokeERKSt9_Any_dataOi, .-_ZNSt17_Function_handlerIFiiEZ4mainEUliE_E9_M_invokeERKSt9_Any_dataOi
	.globl	main
	.type	main, @function
main:
.LFB2622:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA2622
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	leaq	-4192(%rsp), %rsp
	orq	$0, (%rsp)
	leaq	4128(%rsp), %rsp
	.cfi_def_cfa_offset 80
	movq	%fs:40, %rax
	movq	%rax, 56(%rsp)
	xorl	%eax, %eax
	leaq	16(%rsp), %rdi
	movq	%rdi, 16(%rsp)
	leaq	_ZNSt17_Function_handlerIFiiEZ4mainEUliE_E9_M_invokeERKSt9_Any_dataOi(%rip), %rax
	movq	%rax, 40(%rsp)
	leaq	_ZNSt14_Function_base13_Base_managerIZ4mainEUliE_E10_M_managerERSt9_Any_dataRKS3_St18_Manager_operation(%rip), %rax
	movq	%rax, 32(%rsp)
	movl	$6, %esi
.LEHB0:
	call	_ZNKSt8functionIFiiEEclEi
.LEHE0:
	movl	%eax, 12(%rsp)
	movq	32(%rsp), %rax
	testq	%rax, %rax
	je	.L16
	leaq	16(%rsp), %rdi
	movl	$3, %edx
	movq	%rdi, %rsi
	call	*%rax
.L16:
	movl	$0, %eax
	movq	56(%rsp), %rcx
	xorq	%fs:40, %rcx
	jne	.L23
	addq	$64, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
.L20:
	.cfi_restore_state
	movq	%rax, %rbx
	movq	32(%rsp), %rax
	testq	%rax, %rax
	je	.L18
	leaq	16(%rsp), %rdi
	movl	$3, %edx
	movq	%rdi, %rsi
	call	*%rax
.L18:
	movq	%rbx, %rdi
.LEHB1:
	call	_Unwind_Resume@PLT
.LEHE1:
.L23:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE2622:
	.globl	__gxx_personality_v0
	.section	.gcc_except_table,"a",@progbits
.LLSDA2622:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE2622-.LLSDACSB2622
.LLSDACSB2622:
	.uleb128 .LEHB0-.LFB2622
	.uleb128 .LEHE0-.LEHB0
	.uleb128 .L20-.LFB2622
	.uleb128 0
	.uleb128 .LEHB1-.LFB2622
	.uleb128 .LEHE1-.LEHB1
	.uleb128 0
	.uleb128 0
.LLSDACSE2622:
	.text
	.size	main, .-main
	.section	.data.rel.ro,"aw",@progbits
	.align 8
	.type	_ZTIZ4mainEUliE_, @object
	.size	_ZTIZ4mainEUliE_, 16
_ZTIZ4mainEUliE_:
	.quad	_ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	_ZTSZ4mainEUliE_
	.section	.rodata
	.align 8
	.type	_ZTSZ4mainEUliE_, @object
	.size	_ZTSZ4mainEUliE_, 14
_ZTSZ4mainEUliE_:
	.string	"*Z4mainEUliE_"
	.hidden	DW.ref.__gxx_personality_v0
	.weak	DW.ref.__gxx_personality_v0
	.section	.data.DW.ref.__gxx_personality_v0,"awG",@progbits,DW.ref.__gxx_personality_v0,comdat
	.align 8
	.type	DW.ref.__gxx_personality_v0, @object
	.size	DW.ref.__gxx_personality_v0, 8
DW.ref.__gxx_personality_v0:
	.quad	__gxx_personality_v0
	.ident	"GCC: (GNU) 6.2.1 20160830"
	.section	.note.GNU-stack,"",@progbits
