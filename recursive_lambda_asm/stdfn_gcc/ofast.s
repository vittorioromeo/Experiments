	.file	"x.cpp"
	.text
	.p2align 4,,15
	.type	_ZNSt14_Function_base13_Base_managerIZ4mainEUliE_E10_M_managerERSt9_Any_dataRKS3_St18_Manager_operation, @function
_ZNSt14_Function_base13_Base_managerIZ4mainEUliE_E10_M_managerERSt9_Any_dataRKS3_St18_Manager_operation:
.LFB2929:
	.cfi_startproc
	cmpl	$1, %edx
	je	.L3
	jb	.L4
	cmpl	$2, %edx
	jne	.L2
	movq	(%rsi), %rax
	movq	%rax, (%rdi)
.L2:
	xorl	%eax, %eax
	ret
	.p2align 4,,10
	.p2align 3
.L4:
	leaq	_ZTIZ4mainEUliE_(%rip), %rax
	movq	%rax, (%rdi)
	xorl	%eax, %eax
	ret
	.p2align 4,,10
	.p2align 3
.L3:
	movq	%rsi, (%rdi)
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE2929:
	.size	_ZNSt14_Function_base13_Base_managerIZ4mainEUliE_E10_M_managerERSt9_Any_dataRKS3_St18_Manager_operation, .-_ZNSt14_Function_base13_Base_managerIZ4mainEUliE_E10_M_managerERSt9_Any_dataRKS3_St18_Manager_operation
	.p2align 4,,15
	.type	_ZNSt17_Function_handlerIFiiEZ4mainEUliE_E9_M_invokeERKSt9_Any_dataOi, @function
_ZNSt17_Function_handlerIFiiEZ4mainEUliE_E9_M_invokeERKSt9_Any_dataOi:
.LFB2928:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	subq	$4144, %rsp
	orq	$0, (%rsp)
	addq	$4128, %rsp
	.cfi_def_cfa_offset 32
	movl	(%rsi), %ebx
	movq	%fs:40, %rax
	movq	%rax, 8(%rsp)
	xorl	%eax, %eax
	testl	%ebx, %ebx
	je	.L11
	movq	(%rdi), %rax
	leal	-1(%rbx), %edx
	movl	%edx, 4(%rsp)
	cmpq	$0, 16(%rax)
	je	.L13
	leaq	4(%rsp), %rsi
	movq	%rax, %rdi
	call	*24(%rax)
	imull	%ebx, %eax
.L7:
	movq	8(%rsp), %rcx
	xorq	%fs:40, %rcx
	jne	.L14
	addq	$16, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L11:
	.cfi_restore_state
	movl	$1, %eax
	jmp	.L7
.L14:
	call	__stack_chk_fail@PLT
.L13:
	call	_ZSt25__throw_bad_function_callv@PLT
	.cfi_endproc
.LFE2928:
	.size	_ZNSt17_Function_handlerIFiiEZ4mainEUliE_E9_M_invokeERKSt9_Any_dataOi, .-_ZNSt17_Function_handlerIFiiEZ4mainEUliE_E9_M_invokeERKSt9_Any_dataOi
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB2622:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA2622
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$4200, %rsp
	orq	$0, (%rsp)
	addq	$4128, %rsp
	.cfi_def_cfa_offset 96
	movq	%fs:40, %rax
	movq	%rax, 56(%rsp)
	xorl	%eax, %eax
	leaq	_ZNSt17_Function_handlerIFiiEZ4mainEUliE_E9_M_invokeERKSt9_Any_dataOi(%rip), %rax
	leaq	16(%rsp), %rbx
	leaq	12(%rsp), %rsi
	movl	$5, 12(%rsp)
	movq	%rax, 40(%rsp)
	leaq	_ZNSt14_Function_base13_Base_managerIZ4mainEUliE_E10_M_managerERSt9_Any_dataRKS3_St18_Manager_operation(%rip), %rax
	movq	%rbx, %rdi
	movq	%rbx, 16(%rsp)
	movq	%rax, 32(%rsp)
.LEHB0:
	call	_ZNSt17_Function_handlerIFiiEZ4mainEUliE_E9_M_invokeERKSt9_Any_dataOi
.LEHE0:
	leal	(%rax,%rax,2), %eax
	addl	%eax, %eax
	movl	%eax, 8(%rsp)
	movq	32(%rsp), %rax
	testq	%rax, %rax
	je	.L16
	movl	$3, %edx
	movq	%rbx, %rsi
	movq	%rbx, %rdi
	call	*%rax
.L16:
	xorl	%eax, %eax
	movq	56(%rsp), %rcx
	xorq	%fs:40, %rcx
	jne	.L30
	addq	$72, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
.L30:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
.L20:
	movq	%rax, %rbp
	movq	32(%rsp), %rax
	testq	%rax, %rax
	je	.L18
	movl	$3, %edx
	movq	%rbx, %rsi
	movq	%rbx, %rdi
	call	*%rax
.L18:
	movq	%rbp, %rdi
.LEHB1:
	call	_Unwind_Resume@PLT
.LEHE1:
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
	.section	.text.startup
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
