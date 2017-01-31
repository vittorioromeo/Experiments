	.file	"fut2.cpp"
	.text
	.p2align 4,,15
	.globl	_ZN3vrm4core4impl22is_strong_typedef_implEv
	.type	_ZN3vrm4core4impl22is_strong_typedef_implEv, @function
_ZN3vrm4core4impl22is_strong_typedef_implEv:
.LFB2788:
	.cfi_startproc
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE2788:
	.size	_ZN3vrm4core4impl22is_strong_typedef_implEv, .-_ZN3vrm4core4impl22is_strong_typedef_implEv
	.p2align 4,,15
	.globl	_ZN4ecst4impl35decrement_cv_counter_and_notify_oneERSt5mutexRSt18condition_variableRm
	.type	_ZN4ecst4impl35decrement_cv_counter_and_notify_oneERSt5mutexRSt18condition_variableRm, @function
_ZN4ecst4impl35decrement_cv_counter_and_notify_oneERSt5mutexRSt18condition_variableRm:
.LFB5074:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA5074
	movl	$_ZL28__gthrw___pthread_key_createPjPFvPvE, %eax
	testq	%rax, %rax
	je	.L4
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	movq	%rsi, %r12
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movq	%rdi, %rbp
	movq	%rdx, %rbx
	call	_ZL26__gthrw_pthread_mutex_lockP15pthread_mutex_t
	testl	%eax, %eax
	jne	.L9
	subq	$1, (%rbx)
	movq	%r12, %rdi
	call	_ZNSt18condition_variable10notify_oneEv
	popq	%rbx
	.cfi_restore 3
	.cfi_def_cfa_offset 24
	movq	%rbp, %rdi
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_restore 12
	.cfi_def_cfa_offset 8
	jmp	_ZL28__gthrw_pthread_mutex_unlockP15pthread_mutex_t
	.p2align 4,,10
	.p2align 3
.L4:
	subq	$1, (%rdx)
	movq	%rsi, %rdi
	jmp	_ZNSt18condition_variable10notify_oneEv
.L9:
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	.cfi_offset 6, -24
	.cfi_offset 12, -16
	movl	%eax, %edi
	call	_ZSt20__throw_system_errori
	.cfi_endproc
.LFE5074:
	.globl	__gxx_personality_v0
	.section	.gcc_except_table,"a",@progbits
.LLSDA5074:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE5074-.LLSDACSB5074
.LLSDACSB5074:
.LLSDACSE5074:
	.text
	.size	_ZN4ecst4impl35decrement_cv_counter_and_notify_oneERSt5mutexRSt18condition_variableRm, .-_ZN4ecst4impl35decrement_cv_counter_and_notify_oneERSt5mutexRSt18condition_variableRm
	.p2align 4,,15
	.globl	_ZN4ecst4impl35decrement_cv_counter_and_notify_allERSt5mutexRSt18condition_variableRm
	.type	_ZN4ecst4impl35decrement_cv_counter_and_notify_allERSt5mutexRSt18condition_variableRm, @function
_ZN4ecst4impl35decrement_cv_counter_and_notify_allERSt5mutexRSt18condition_variableRm:
.LFB5078:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA5078
	movl	$_ZL28__gthrw___pthread_key_createPjPFvPvE, %eax
	testq	%rax, %rax
	je	.L11
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	movq	%rsi, %r12
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movq	%rdi, %rbp
	movq	%rdx, %rbx
	call	_ZL26__gthrw_pthread_mutex_lockP15pthread_mutex_t
	testl	%eax, %eax
	jne	.L16
	subq	$1, (%rbx)
	movq	%r12, %rdi
	call	_ZNSt18condition_variable10notify_allEv
	popq	%rbx
	.cfi_restore 3
	.cfi_def_cfa_offset 24
	movq	%rbp, %rdi
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_restore 12
	.cfi_def_cfa_offset 8
	jmp	_ZL28__gthrw_pthread_mutex_unlockP15pthread_mutex_t
	.p2align 4,,10
	.p2align 3
.L11:
	subq	$1, (%rdx)
	movq	%rsi, %rdi
	jmp	_ZNSt18condition_variable10notify_allEv
.L16:
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	.cfi_offset 6, -24
	.cfi_offset 12, -16
	movl	%eax, %edi
	call	_ZSt20__throw_system_errori
	.cfi_endproc
.LFE5078:
	.section	.gcc_except_table
.LLSDA5078:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE5078-.LLSDACSB5078
.LLSDACSB5078:
.LLSDACSE5078:
	.text
	.size	_ZN4ecst4impl35decrement_cv_counter_and_notify_allERSt5mutexRSt18condition_variableRm, .-_ZN4ecst4impl35decrement_cv_counter_and_notify_allERSt5mutexRSt18condition_variableRm
	.align 2
	.p2align 4,,15
	.globl	_ZN4ecst5latchC2Em
	.type	_ZN4ecst5latchC2Em, @function
_ZN4ecst5latchC2Em:
.LFB5092:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rsi, %rbp
	movq	%rdi, %rbx
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	call	_ZNSt18condition_variableC1Ev
	pxor	%xmm0, %xmm0
	movq	%rbp, 88(%rbx)
	movq	$0, 80(%rbx)
	movups	%xmm0, 48(%rbx)
	movups	%xmm0, 64(%rbx)
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE5092:
	.size	_ZN4ecst5latchC2Em, .-_ZN4ecst5latchC2Em
	.globl	_ZN4ecst5latchC1Em
	.set	_ZN4ecst5latchC1Em,_ZN4ecst5latchC2Em
	.align 2
	.p2align 4,,15
	.globl	_ZN4ecst5latch24decrement_and_notify_oneEv
	.type	_ZN4ecst5latch24decrement_and_notify_oneEv, @function
_ZN4ecst5latch24decrement_and_notify_oneEv:
.LFB5094:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA5094
	movl	$_ZL28__gthrw___pthread_key_createPjPFvPvE, %eax
	testq	%rax, %rax
	je	.L20
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	leaq	48(%rdi), %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	%rbp, %rdi
	call	_ZL26__gthrw_pthread_mutex_lockP15pthread_mutex_t
	testl	%eax, %eax
	jne	.L25
	subq	$1, 88(%rbx)
	movq	%rbx, %rdi
	call	_ZNSt18condition_variable10notify_oneEv
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	movq	%rbp, %rdi
	popq	%rbx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	jmp	_ZL28__gthrw_pthread_mutex_unlockP15pthread_mutex_t
	.p2align 4,,10
	.p2align 3
.L20:
	subq	$1, 88(%rdi)
	jmp	_ZNSt18condition_variable10notify_oneEv
.L25:
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -24
	.cfi_offset 6, -16
	movl	%eax, %edi
	call	_ZSt20__throw_system_errori
	.cfi_endproc
.LFE5094:
	.section	.gcc_except_table
.LLSDA5094:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE5094-.LLSDACSB5094
.LLSDACSB5094:
.LLSDACSE5094:
	.text
	.size	_ZN4ecst5latch24decrement_and_notify_oneEv, .-_ZN4ecst5latch24decrement_and_notify_oneEv
	.align 2
	.p2align 4,,15
	.globl	_ZN4ecst5latch24decrement_and_notify_allEv
	.type	_ZN4ecst5latch24decrement_and_notify_allEv, @function
_ZN4ecst5latch24decrement_and_notify_allEv:
.LFB5095:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA5095
	movl	$_ZL28__gthrw___pthread_key_createPjPFvPvE, %eax
	testq	%rax, %rax
	je	.L27
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	leaq	48(%rdi), %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	%rbp, %rdi
	call	_ZL26__gthrw_pthread_mutex_lockP15pthread_mutex_t
	testl	%eax, %eax
	jne	.L32
	subq	$1, 88(%rbx)
	movq	%rbx, %rdi
	call	_ZNSt18condition_variable10notify_allEv
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	movq	%rbp, %rdi
	popq	%rbx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	jmp	_ZL28__gthrw_pthread_mutex_unlockP15pthread_mutex_t
	.p2align 4,,10
	.p2align 3
.L27:
	subq	$1, 88(%rdi)
	jmp	_ZNSt18condition_variable10notify_allEv
.L32:
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -24
	.cfi_offset 6, -16
	movl	%eax, %edi
	call	_ZSt20__throw_system_errori
	.cfi_endproc
.LFE5095:
	.section	.gcc_except_table
.LLSDA5095:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE5095-.LLSDACSB5095
.LLSDACSB5095:
.LLSDACSE5095:
	.text
	.size	_ZN4ecst5latch24decrement_and_notify_allEv, .-_ZN4ecst5latch24decrement_and_notify_allEv
	.p2align 4,,15
	.globl	_Z3op0v
	.type	_Z3op0v, @function
_Z3op0v:
.LFB6690:
	.cfi_startproc
	movl	state(%rip), %eax
	addl	$1, %eax
	movl	%eax, state(%rip)
	ret
	.cfi_endproc
.LFE6690:
	.size	_Z3op0v, .-_Z3op0v
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB6691:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA6691
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movl	$_ZL28__gthrw___pthread_key_createPjPFvPvE, %ebp
	subq	$120, %rsp
	.cfi_def_cfa_offset 144
	leaq	16(%rsp), %rdi
	call	_ZNSt18condition_variableC1Ev
	movl	state(%rip), %eax
	pxor	%xmm0, %xmm0
	movq	$0, 96(%rsp)
	movq	$1, 104(%rsp)
	addl	$1, %eax
	movl	%eax, state(%rip)
	movl	state(%rip), %eax
	movaps	%xmm0, 64(%rsp)
	addl	$1, %eax
	movaps	%xmm0, 80(%rsp)
	movl	%eax, state(%rip)
	movl	state(%rip), %eax
	addl	$1, %eax
	movl	%eax, state(%rip)
	movl	state(%rip), %eax
	addl	$1, %eax
	testq	%rbp, %rbp
	movl	%eax, state(%rip)
	je	.L35
	leaq	64(%rsp), %rbx
	movq	%rbx, %rdi
	call	_ZL26__gthrw_pthread_mutex_lockP15pthread_mutex_t
	testl	%eax, %eax
	jne	.L51
	leaq	16(%rsp), %rdi
	subq	$1, 104(%rsp)
	call	_ZNSt18condition_variable10notify_oneEv
	movq	%rbx, %rdi
	call	_ZL28__gthrw_pthread_mutex_unlockP15pthread_mutex_t
	movq	%rbx, %rdi
	movq	%rbx, (%rsp)
	movb	$0, 8(%rsp)
	call	_ZL26__gthrw_pthread_mutex_lockP15pthread_mutex_t
	testl	%eax, %eax
	jne	.L52
.L41:
	cmpq	$0, 104(%rsp)
	movb	$1, 8(%rsp)
	je	.L40
	.p2align 4,,10
	.p2align 3
.L37:
	leaq	16(%rsp), %rdi
	movq	%rsp, %rsi
	call	_ZNSt18condition_variable4waitERSt11unique_lockISt5mutexE
	cmpq	$0, 104(%rsp)
	jne	.L37
	cmpb	$0, 8(%rsp)
	jne	.L40
.L39:
	leaq	16(%rsp), %rdi
	call	_ZNSt18condition_variableD1Ev
	addq	$120, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
.L35:
	.cfi_restore_state
	leaq	16(%rsp), %rdi
	movq	$_ZL28__gthrw___pthread_key_createPjPFvPvE, 104(%rsp)
	call	_ZNSt18condition_variable10notify_oneEv
	leaq	64(%rsp), %rax
	movq	%rax, (%rsp)
	jmp	.L41
.L40:
	movq	(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L39
	testq	%rbp, %rbp
	je	.L39
	call	_ZL28__gthrw_pthread_mutex_unlockP15pthread_mutex_t
	jmp	.L39
.L51:
	movl	%eax, %edi
	call	_ZSt20__throw_system_errori
.L52:
	movl	%eax, %edi
	call	_ZSt20__throw_system_errori
	.cfi_endproc
.LFE6691:
	.section	.gcc_except_table
.LLSDA6691:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE6691-.LLSDACSB6691
.LLSDACSB6691:
.LLSDACSE6691:
	.section	.text.startup
	.size	main, .-main
	.globl	my_pool
	.bss
	.type	my_pool, @object
	.size	my_pool, 1
my_pool:
	.zero	1
	.globl	state
	.align 4
	.type	state, @object
	.size	state, 4
state:
	.zero	4
	.weakref	_ZL28__gthrw___pthread_key_createPjPFvPvE,__pthread_key_create
	.weakref	_ZL28__gthrw_pthread_mutex_unlockP15pthread_mutex_t,pthread_mutex_unlock
	.weakref	_ZL26__gthrw_pthread_mutex_lockP15pthread_mutex_t,pthread_mutex_lock
	.ident	"GCC: (GNU) 7.0.0 20161204 (experimental)"
	.section	.note.GNU-stack,"",@progbits
