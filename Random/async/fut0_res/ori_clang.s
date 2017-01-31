	.text
	.file	"./fut0.cpp"
	.globl	_ZN3vrm4core4impl22is_strong_typedef_implEv
	.p2align	4, 0x90
	.type	_ZN3vrm4core4impl22is_strong_typedef_implEv,@function
_ZN3vrm4core4impl22is_strong_typedef_implEv: # @_ZN3vrm4core4impl22is_strong_typedef_implEv
	.cfi_startproc
# BB#0:
	retq
.Lfunc_end0:
	.size	_ZN3vrm4core4impl22is_strong_typedef_implEv, .Lfunc_end0-_ZN3vrm4core4impl22is_strong_typedef_implEv
	.cfi_endproc

	.globl	_ZN4ecst4impl35decrement_cv_counter_and_notify_oneERSt5mutexRSt18condition_variableRm
	.p2align	4, 0x90
	.type	_ZN4ecst4impl35decrement_cv_counter_and_notify_oneERSt5mutexRSt18condition_variableRm,@function
_ZN4ecst4impl35decrement_cv_counter_and_notify_oneERSt5mutexRSt18condition_variableRm: # @_ZN4ecst4impl35decrement_cv_counter_and_notify_oneERSt5mutexRSt18condition_variableRm
.Lfunc_begin0:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception0
# BB#0:
	pushq	%r15
.Ltmp3:
	.cfi_def_cfa_offset 16
	pushq	%r14
.Ltmp4:
	.cfi_def_cfa_offset 24
	pushq	%rbx
.Ltmp5:
	.cfi_def_cfa_offset 32
.Ltmp6:
	.cfi_offset %rbx, -32
.Ltmp7:
	.cfi_offset %r14, -24
.Ltmp8:
	.cfi_offset %r15, -16
	movq	%rdx, %rbx
	movq	%rsi, %r15
	movq	%rdi, %r14
	movl	$__pthread_key_create, %eax
	testq	%rax, %rax
	je	.LBB1_6
# BB#1:
	movq	%r14, %rdi
	callq	pthread_mutex_lock
	testl	%eax, %eax
	jne	.LBB1_2
# BB#4:
	decq	(%rbx)
	movq	%r15, %rdi
	callq	_ZNSt18condition_variable10notify_oneEv
	movl	$__pthread_key_create, %eax
	testq	%rax, %rax
	je	.LBB1_8
# BB#5:
	movq	%r14, %rdi
	popq	%rbx
	popq	%r14
	popq	%r15
	jmp	pthread_mutex_unlock    # TAILCALL
.LBB1_6:
	decq	(%rbx)
	movq	%r15, %rdi
	popq	%rbx
	popq	%r14
	popq	%r15
	jmp	_ZNSt18condition_variable10notify_oneEv # TAILCALL
.LBB1_8:
	popq	%rbx
	popq	%r14
	popq	%r15
	retq
.LBB1_2:
.Ltmp0:
	movl	%eax, %edi
	callq	_ZSt20__throw_system_errori
.Ltmp1:
# BB#3:
.LBB1_7:
.Ltmp2:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.Lfunc_end1:
	.size	_ZN4ecst4impl35decrement_cv_counter_and_notify_oneERSt5mutexRSt18condition_variableRm, .Lfunc_end1-_ZN4ecst4impl35decrement_cv_counter_and_notify_oneERSt5mutexRSt18condition_variableRm
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table1:
.Lexception0:
	.byte	255                     # @LPStart Encoding = omit
	.byte	3                       # @TType Encoding = udata4
	.byte	21                      # @TType base offset
	.byte	3                       # Call site Encoding = udata4
	.byte	13                      # Call site table length
	.long	.Ltmp0-.Lfunc_begin0    # >> Call Site 1 <<
	.long	.Ltmp1-.Ltmp0           #   Call between .Ltmp0 and .Ltmp1
	.long	.Ltmp2-.Lfunc_begin0    #     jumps to .Ltmp2
	.byte	1                       #   On action: 1
	.byte	1                       # >> Action Record 1 <<
                                        #   Catch TypeInfo 1
	.byte	0                       #   No further actions
                                        # >> Catch TypeInfos <<
	.long	0                       # TypeInfo 1
	.p2align	2

	.text
	.globl	_ZN4ecst4impl35decrement_cv_counter_and_notify_allERSt5mutexRSt18condition_variableRm
	.p2align	4, 0x90
	.type	_ZN4ecst4impl35decrement_cv_counter_and_notify_allERSt5mutexRSt18condition_variableRm,@function
_ZN4ecst4impl35decrement_cv_counter_and_notify_allERSt5mutexRSt18condition_variableRm: # @_ZN4ecst4impl35decrement_cv_counter_and_notify_allERSt5mutexRSt18condition_variableRm
.Lfunc_begin1:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception1
# BB#0:
	pushq	%r15
.Ltmp12:
	.cfi_def_cfa_offset 16
	pushq	%r14
.Ltmp13:
	.cfi_def_cfa_offset 24
	pushq	%rbx
.Ltmp14:
	.cfi_def_cfa_offset 32
.Ltmp15:
	.cfi_offset %rbx, -32
.Ltmp16:
	.cfi_offset %r14, -24
.Ltmp17:
	.cfi_offset %r15, -16
	movq	%rdx, %rbx
	movq	%rsi, %r15
	movq	%rdi, %r14
	movl	$__pthread_key_create, %eax
	testq	%rax, %rax
	je	.LBB2_6
# BB#1:
	movq	%r14, %rdi
	callq	pthread_mutex_lock
	testl	%eax, %eax
	jne	.LBB2_2
# BB#4:
	decq	(%rbx)
	movq	%r15, %rdi
	callq	_ZNSt18condition_variable10notify_allEv
	movl	$__pthread_key_create, %eax
	testq	%rax, %rax
	je	.LBB2_8
# BB#5:
	movq	%r14, %rdi
	popq	%rbx
	popq	%r14
	popq	%r15
	jmp	pthread_mutex_unlock    # TAILCALL
.LBB2_6:
	decq	(%rbx)
	movq	%r15, %rdi
	popq	%rbx
	popq	%r14
	popq	%r15
	jmp	_ZNSt18condition_variable10notify_allEv # TAILCALL
.LBB2_8:
	popq	%rbx
	popq	%r14
	popq	%r15
	retq
.LBB2_2:
.Ltmp9:
	movl	%eax, %edi
	callq	_ZSt20__throw_system_errori
.Ltmp10:
# BB#3:
.LBB2_7:
.Ltmp11:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.Lfunc_end2:
	.size	_ZN4ecst4impl35decrement_cv_counter_and_notify_allERSt5mutexRSt18condition_variableRm, .Lfunc_end2-_ZN4ecst4impl35decrement_cv_counter_and_notify_allERSt5mutexRSt18condition_variableRm
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table2:
.Lexception1:
	.byte	255                     # @LPStart Encoding = omit
	.byte	3                       # @TType Encoding = udata4
	.byte	21                      # @TType base offset
	.byte	3                       # Call site Encoding = udata4
	.byte	13                      # Call site table length
	.long	.Ltmp9-.Lfunc_begin1    # >> Call Site 1 <<
	.long	.Ltmp10-.Ltmp9          #   Call between .Ltmp9 and .Ltmp10
	.long	.Ltmp11-.Lfunc_begin1   #     jumps to .Ltmp11
	.byte	1                       #   On action: 1
	.byte	1                       # >> Action Record 1 <<
                                        #   Catch TypeInfo 1
	.byte	0                       #   No further actions
                                        # >> Catch TypeInfos <<
	.long	0                       # TypeInfo 1
	.p2align	2

	.text
	.globl	_ZN4ecst5latchC2Em
	.p2align	4, 0x90
	.type	_ZN4ecst5latchC2Em,@function
_ZN4ecst5latchC2Em:                     # @_ZN4ecst5latchC2Em
	.cfi_startproc
# BB#0:
	pushq	%r14
.Ltmp18:
	.cfi_def_cfa_offset 16
	pushq	%rbx
.Ltmp19:
	.cfi_def_cfa_offset 24
	pushq	%rax
.Ltmp20:
	.cfi_def_cfa_offset 32
.Ltmp21:
	.cfi_offset %rbx, -24
.Ltmp22:
	.cfi_offset %r14, -16
	movq	%rsi, %r14
	movq	%rdi, %rbx
	callq	_ZNSt18condition_variableC1Ev
	xorps	%xmm0, %xmm0
	movups	%xmm0, 64(%rbx)
	movups	%xmm0, 48(%rbx)
	movq	$0, 80(%rbx)
	movq	%r14, 88(%rbx)
	addq	$8, %rsp
	popq	%rbx
	popq	%r14
	retq
.Lfunc_end3:
	.size	_ZN4ecst5latchC2Em, .Lfunc_end3-_ZN4ecst5latchC2Em
	.cfi_endproc

	.globl	_ZN4ecst5latch24decrement_and_notify_oneEv
	.p2align	4, 0x90
	.type	_ZN4ecst5latch24decrement_and_notify_oneEv,@function
_ZN4ecst5latch24decrement_and_notify_oneEv: # @_ZN4ecst5latch24decrement_and_notify_oneEv
.Lfunc_begin2:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception2
# BB#0:
	pushq	%r14
.Ltmp26:
	.cfi_def_cfa_offset 16
	pushq	%rbx
.Ltmp27:
	.cfi_def_cfa_offset 24
	pushq	%rax
.Ltmp28:
	.cfi_def_cfa_offset 32
.Ltmp29:
	.cfi_offset %rbx, -24
.Ltmp30:
	.cfi_offset %r14, -16
	movq	%rdi, %rbx
	movl	$__pthread_key_create, %eax
	testq	%rax, %rax
	je	.LBB4_6
# BB#1:
	leaq	48(%rbx), %r14
	movq	%r14, %rdi
	callq	pthread_mutex_lock
	testl	%eax, %eax
	jne	.LBB4_2
# BB#4:
	decq	88(%rbx)
	movq	%rbx, %rdi
	callq	_ZNSt18condition_variable10notify_oneEv
	movl	$__pthread_key_create, %eax
	testq	%rax, %rax
	je	.LBB4_8
# BB#5:
	movq	%r14, %rdi
	addq	$8, %rsp
	popq	%rbx
	popq	%r14
	jmp	pthread_mutex_unlock    # TAILCALL
.LBB4_6:
	decq	88(%rbx)
	movq	%rbx, %rdi
	addq	$8, %rsp
	popq	%rbx
	popq	%r14
	jmp	_ZNSt18condition_variable10notify_oneEv # TAILCALL
.LBB4_8:
	addq	$8, %rsp
	popq	%rbx
	popq	%r14
	retq
.LBB4_2:
.Ltmp23:
	movl	%eax, %edi
	callq	_ZSt20__throw_system_errori
.Ltmp24:
# BB#3:
.LBB4_7:
.Ltmp25:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.Lfunc_end4:
	.size	_ZN4ecst5latch24decrement_and_notify_oneEv, .Lfunc_end4-_ZN4ecst5latch24decrement_and_notify_oneEv
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table4:
.Lexception2:
	.byte	255                     # @LPStart Encoding = omit
	.byte	3                       # @TType Encoding = udata4
	.byte	21                      # @TType base offset
	.byte	3                       # Call site Encoding = udata4
	.byte	13                      # Call site table length
	.long	.Ltmp23-.Lfunc_begin2   # >> Call Site 1 <<
	.long	.Ltmp24-.Ltmp23         #   Call between .Ltmp23 and .Ltmp24
	.long	.Ltmp25-.Lfunc_begin2   #     jumps to .Ltmp25
	.byte	1                       #   On action: 1
	.byte	1                       # >> Action Record 1 <<
                                        #   Catch TypeInfo 1
	.byte	0                       #   No further actions
                                        # >> Catch TypeInfos <<
	.long	0                       # TypeInfo 1
	.p2align	2

	.text
	.globl	_ZN4ecst5latch24decrement_and_notify_allEv
	.p2align	4, 0x90
	.type	_ZN4ecst5latch24decrement_and_notify_allEv,@function
_ZN4ecst5latch24decrement_and_notify_allEv: # @_ZN4ecst5latch24decrement_and_notify_allEv
.Lfunc_begin3:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception3
# BB#0:
	pushq	%r14
.Ltmp34:
	.cfi_def_cfa_offset 16
	pushq	%rbx
.Ltmp35:
	.cfi_def_cfa_offset 24
	pushq	%rax
.Ltmp36:
	.cfi_def_cfa_offset 32
.Ltmp37:
	.cfi_offset %rbx, -24
.Ltmp38:
	.cfi_offset %r14, -16
	movq	%rdi, %rbx
	movl	$__pthread_key_create, %eax
	testq	%rax, %rax
	je	.LBB5_6
# BB#1:
	leaq	48(%rbx), %r14
	movq	%r14, %rdi
	callq	pthread_mutex_lock
	testl	%eax, %eax
	jne	.LBB5_2
# BB#4:
	decq	88(%rbx)
	movq	%rbx, %rdi
	callq	_ZNSt18condition_variable10notify_allEv
	movl	$__pthread_key_create, %eax
	testq	%rax, %rax
	je	.LBB5_8
# BB#5:
	movq	%r14, %rdi
	addq	$8, %rsp
	popq	%rbx
	popq	%r14
	jmp	pthread_mutex_unlock    # TAILCALL
.LBB5_6:
	decq	88(%rbx)
	movq	%rbx, %rdi
	addq	$8, %rsp
	popq	%rbx
	popq	%r14
	jmp	_ZNSt18condition_variable10notify_allEv # TAILCALL
.LBB5_8:
	addq	$8, %rsp
	popq	%rbx
	popq	%r14
	retq
.LBB5_2:
.Ltmp31:
	movl	%eax, %edi
	callq	_ZSt20__throw_system_errori
.Ltmp32:
# BB#3:
.LBB5_7:
.Ltmp33:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.Lfunc_end5:
	.size	_ZN4ecst5latch24decrement_and_notify_allEv, .Lfunc_end5-_ZN4ecst5latch24decrement_and_notify_allEv
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table5:
.Lexception3:
	.byte	255                     # @LPStart Encoding = omit
	.byte	3                       # @TType Encoding = udata4
	.byte	21                      # @TType base offset
	.byte	3                       # Call site Encoding = udata4
	.byte	13                      # Call site table length
	.long	.Ltmp31-.Lfunc_begin3   # >> Call Site 1 <<
	.long	.Ltmp32-.Ltmp31         #   Call between .Ltmp31 and .Ltmp32
	.long	.Ltmp33-.Lfunc_begin3   #     jumps to .Ltmp33
	.byte	1                       #   On action: 1
	.byte	1                       # >> Action Record 1 <<
                                        #   Catch TypeInfo 1
	.byte	0                       #   No further actions
                                        # >> Catch TypeInfos <<
	.long	0                       # TypeInfo 1
	.p2align	2

	.text
	.globl	_Z9get_valuev
	.p2align	4, 0x90
	.type	_Z9get_valuev,@function
_Z9get_valuev:                          # @_Z9get_valuev
	.cfi_startproc
# BB#0:
	xorl	%eax, %eax
	retq
.Lfunc_end6:
	.size	_Z9get_valuev, .Lfunc_end6-_Z9get_valuev
	.cfi_endproc

	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
.Lfunc_begin4:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception4
# BB#0:
	pushq	%r14
.Ltmp45:
	.cfi_def_cfa_offset 16
	pushq	%rbx
.Ltmp46:
	.cfi_def_cfa_offset 24
	subq	$120, %rsp
.Ltmp47:
	.cfi_def_cfa_offset 144
.Ltmp48:
	.cfi_offset %rbx, -24
.Ltmp49:
	.cfi_offset %r14, -16
	leaq	24(%rsp), %rdi
	callq	_ZNSt18condition_variableC1Ev
	xorps	%xmm0, %xmm0
	movups	%xmm0, 88(%rsp)
	movups	%xmm0, 72(%rsp)
	movq	$0, 104(%rsp)
	movq	$1, 112(%rsp)
	movl	$__pthread_key_create, %eax
	testq	%rax, %rax
	je	.LBB7_7
# BB#1:
	leaq	72(%rsp), %rbx
	movq	%rbx, %rdi
	callq	pthread_mutex_lock
	testl	%eax, %eax
	jne	.LBB7_2
# BB#4:
	decq	112(%rsp)
	leaq	24(%rsp), %rdi
	callq	_ZNSt18condition_variable10notify_allEv
	movl	$__pthread_key_create, %eax
	testq	%rax, %rax
	je	.LBB7_8
# BB#5:
	movq	%rbx, %rdi
	callq	pthread_mutex_unlock
	jmp	.LBB7_8
.LBB7_7:
	movq	$0, 112(%rsp)
	leaq	24(%rsp), %rdi
	callq	_ZNSt18condition_variable10notify_allEv
.LBB7_8:
	leaq	72(%rsp), %rbx
	movq	%rbx, 8(%rsp)
	movb	$0, 16(%rsp)
	movl	$__pthread_key_create, %eax
	testq	%rax, %rax
	je	.LBB7_12
# BB#9:
	movq	%rbx, %rdi
	callq	pthread_mutex_lock
	testl	%eax, %eax
	jne	.LBB7_10
.LBB7_12:
	movb	$1, 16(%rsp)
	cmpq	$0, 112(%rsp)
	je	.LBB7_17
# BB#13:
	leaq	24(%rsp), %r14
	leaq	8(%rsp), %rbx
	.p2align	4, 0x90
.LBB7_14:                               # =>This Inner Loop Header: Depth=1
	movq	%r14, %rdi
	movq	%rbx, %rsi
	callq	_ZNSt18condition_variable4waitERSt11unique_lockISt5mutexE
	cmpq	$0, 112(%rsp)
	jne	.LBB7_14
# BB#15:
	cmpb	$0, 16(%rsp)
	je	.LBB7_20
# BB#16:
	movq	8(%rsp), %rbx
	testq	%rbx, %rbx
	je	.LBB7_20
.LBB7_17:
	movl	$__pthread_key_create, %eax
	testq	%rax, %rax
	je	.LBB7_19
# BB#18:
	movq	%rbx, %rdi
	callq	pthread_mutex_unlock
.LBB7_19:
	movb	$0, 16(%rsp)
.LBB7_20:
	leaq	24(%rsp), %rdi
	callq	_ZNSt18condition_variableD1Ev
	xorl	%eax, %eax
	addq	$120, %rsp
	popq	%rbx
	popq	%r14
	retq
.LBB7_2:
.Ltmp39:
	movl	%eax, %edi
	callq	_ZSt20__throw_system_errori
.Ltmp40:
# BB#3:
.LBB7_10:
.Ltmp42:
	movl	%eax, %edi
	callq	_ZSt20__throw_system_errori
.Ltmp43:
# BB#11:
.LBB7_21:
.Ltmp44:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.LBB7_6:
.Ltmp41:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.Lfunc_end7:
	.size	main, .Lfunc_end7-main
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table7:
.Lexception4:
	.byte	255                     # @LPStart Encoding = omit
	.byte	3                       # @TType Encoding = udata4
	.asciz	"\242\200\200"          # @TType base offset
	.byte	3                       # Call site Encoding = udata4
	.byte	26                      # Call site table length
	.long	.Ltmp39-.Lfunc_begin4   # >> Call Site 1 <<
	.long	.Ltmp40-.Ltmp39         #   Call between .Ltmp39 and .Ltmp40
	.long	.Ltmp41-.Lfunc_begin4   #     jumps to .Ltmp41
	.byte	1                       #   On action: 1
	.long	.Ltmp42-.Lfunc_begin4   # >> Call Site 2 <<
	.long	.Ltmp43-.Ltmp42         #   Call between .Ltmp42 and .Ltmp43
	.long	.Ltmp44-.Lfunc_begin4   #     jumps to .Ltmp44
	.byte	1                       #   On action: 1
	.byte	1                       # >> Action Record 1 <<
                                        #   Catch TypeInfo 1
	.byte	0                       #   No further actions
                                        # >> Catch TypeInfos <<
	.long	0                       # TypeInfo 1
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
.Lfunc_end8:
	.size	__clang_call_terminate, .Lfunc_end8-__clang_call_terminate

	.weak	pthread_mutex_lock
	.weak	__pthread_key_create
	.weak	pthread_mutex_unlock

	.globl	_ZN4ecst5latchC1Em
	.type	_ZN4ecst5latchC1Em,@function
_ZN4ecst5latchC1Em = _ZN4ecst5latchC2Em
	.ident	"clang version 3.9.0 (tags/RELEASE_390/final)"
	.section	".note.GNU-stack","",@progbits
