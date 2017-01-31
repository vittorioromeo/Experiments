	.file	"fut0.cpp"
	.section	.text._ZNSt13__future_base13_State_baseV217_M_complete_asyncEv,"axG",@progbits,_ZNSt13__future_base13_State_baseV217_M_complete_asyncEv,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt13__future_base13_State_baseV217_M_complete_asyncEv
	.type	_ZNSt13__future_base13_State_baseV217_M_complete_asyncEv, @function
_ZNSt13__future_base13_State_baseV217_M_complete_asyncEv:
.LFB2810:
	.cfi_startproc
	rep ret
	.cfi_endproc
.LFE2810:
	.size	_ZNSt13__future_base13_State_baseV217_M_complete_asyncEv, .-_ZNSt13__future_base13_State_baseV217_M_complete_asyncEv
	.section	.text._ZNKSt13__future_base13_State_baseV221_M_is_deferred_futureEv,"axG",@progbits,_ZNKSt13__future_base13_State_baseV221_M_is_deferred_futureEv,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNKSt13__future_base13_State_baseV221_M_is_deferred_futureEv
	.type	_ZNKSt13__future_base13_State_baseV221_M_is_deferred_futureEv, @function
_ZNKSt13__future_base13_State_baseV221_M_is_deferred_futureEv:
.LFB2811:
	.cfi_startproc
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE2811:
	.size	_ZNKSt13__future_base13_State_baseV221_M_is_deferred_futureEv, .-_ZNKSt13__future_base13_State_baseV221_M_is_deferred_futureEv
	.text
	.p2align 4,,15
	.globl	_Z9get_valuev
	.type	_Z9get_valuev, @function
_Z9get_valuev:
.LFB2997:
	.cfi_startproc
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE2997:
	.size	_Z9get_valuev, .-_Z9get_valuev
	.section	.text._ZNSt15__exception_ptr12__dest_thunkISt12future_errorEEvPv,"axG",@progbits,_ZNSt15__exception_ptr12__dest_thunkISt12future_errorEEvPv,comdat
	.p2align 4,,15
	.weak	_ZNSt15__exception_ptr12__dest_thunkISt12future_errorEEvPv
	.type	_ZNSt15__exception_ptr12__dest_thunkISt12future_errorEEvPv, @function
_ZNSt15__exception_ptr12__dest_thunkISt12future_errorEEvPv:
.LFB3575:
	.cfi_startproc
	movq	(%rdi), %rax
	jmp	*(%rax)
	.cfi_endproc
.LFE3575:
	.size	_ZNSt15__exception_ptr12__dest_thunkISt12future_errorEEvPv, .-_ZNSt15__exception_ptr12__dest_thunkISt12future_errorEEvPv
	.section	.text._ZNSt13__future_base13_State_baseV2D2Ev,"axG",@progbits,_ZNSt13__future_base13_State_baseV2D5Ev,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt13__future_base13_State_baseV2D2Ev
	.type	_ZNSt13__future_base13_State_baseV2D2Ev, @function
_ZNSt13__future_base13_State_baseV2D2Ev:
.LFB4368:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA4368
	movq	$_ZTVNSt13__future_base13_State_baseV2E+16, (%rdi)
	movq	8(%rdi), %rdi
	testq	%rdi, %rdi
	je	.L6
	movq	(%rdi), %rax
	jmp	*(%rax)
	.p2align 4,,10
	.p2align 3
.L6:
	rep ret
	.cfi_endproc
.LFE4368:
	.globl	__gxx_personality_v0
	.section	.gcc_except_table._ZNSt13__future_base13_State_baseV2D2Ev,"aG",@progbits,_ZNSt13__future_base13_State_baseV2D5Ev,comdat
.LLSDA4368:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE4368-.LLSDACSB4368
.LLSDACSB4368:
.LLSDACSE4368:
	.section	.text._ZNSt13__future_base13_State_baseV2D2Ev,"axG",@progbits,_ZNSt13__future_base13_State_baseV2D5Ev,comdat
	.size	_ZNSt13__future_base13_State_baseV2D2Ev, .-_ZNSt13__future_base13_State_baseV2D2Ev
	.weak	_ZNSt13__future_base13_State_baseV2D1Ev
	.set	_ZNSt13__future_base13_State_baseV2D1Ev,_ZNSt13__future_base13_State_baseV2D2Ev
	.section	.text._ZNSt14_Function_base13_Base_managerINSt13__future_base12_Task_setterISt10unique_ptrINS1_7_ResultIiEENS1_12_Result_base8_DeleterEENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEEE10_M_managerERSt9_Any_dataRKSI_St18_Manager_operation,"axG",@progbits,_ZNSt14_Function_base13_Base_managerINSt13__future_base12_Task_setterISt10unique_ptrINS1_7_ResultIiEENS1_12_Result_base8_DeleterEENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEEE10_M_managerERSt9_Any_dataRKSI_St18_Manager_operation,comdat
	.p2align 4,,15
	.weak	_ZNSt14_Function_base13_Base_managerINSt13__future_base12_Task_setterISt10unique_ptrINS1_7_ResultIiEENS1_12_Result_base8_DeleterEENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEEE10_M_managerERSt9_Any_dataRKSI_St18_Manager_operation
	.type	_ZNSt14_Function_base13_Base_managerINSt13__future_base12_Task_setterISt10unique_ptrINS1_7_ResultIiEENS1_12_Result_base8_DeleterEENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEEE10_M_managerERSt9_Any_dataRKSI_St18_Manager_operation, @function
_ZNSt14_Function_base13_Base_managerINSt13__future_base12_Task_setterISt10unique_ptrINS1_7_ResultIiEENS1_12_Result_base8_DeleterEENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEEE10_M_managerERSt9_Any_dataRKSI_St18_Manager_operation:
.LFB4453:
	.cfi_startproc
	cmpl	$1, %edx
	je	.L10
	jb	.L11
	cmpl	$2, %edx
	jne	.L9
	movdqu	(%rsi), %xmm0
	movups	%xmm0, (%rdi)
.L9:
	xorl	%eax, %eax
	ret
	.p2align 4,,10
	.p2align 3
.L11:
	movq	$_ZTINSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE, (%rdi)
	xorl	%eax, %eax
	ret
	.p2align 4,,10
	.p2align 3
.L10:
	movq	%rsi, (%rdi)
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE4453:
	.size	_ZNSt14_Function_base13_Base_managerINSt13__future_base12_Task_setterISt10unique_ptrINS1_7_ResultIiEENS1_12_Result_base8_DeleterEENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEEE10_M_managerERSt9_Any_dataRKSI_St18_Manager_operation, .-_ZNSt14_Function_base13_Base_managerINSt13__future_base12_Task_setterISt10unique_ptrINS1_7_ResultIiEENS1_12_Result_base8_DeleterEENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEEE10_M_managerERSt9_Any_dataRKSI_St18_Manager_operation
	.section	.text._ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED2Ev,"axG",@progbits,_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED5Ev,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED2Ev
	.type	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED2Ev, @function
_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED2Ev:
.LFB4631:
	.cfi_startproc
	rep ret
	.cfi_endproc
.LFE4631:
	.size	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED2Ev, .-_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED2Ev
	.weak	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED1Ev
	.set	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED1Ev,_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED2Ev
	.section	.text._ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED2Ev,"axG",@progbits,_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED5Ev,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED2Ev
	.type	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED2Ev, @function
_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED2Ev:
.LFB4638:
	.cfi_startproc
	rep ret
	.cfi_endproc
.LFE4638:
	.size	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED2Ev, .-_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED2Ev
	.weak	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED1Ev
	.set	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED1Ev,_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED2Ev
	.section	.text._ZNKSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiE21_M_is_deferred_futureEv,"axG",@progbits,_ZNKSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiE21_M_is_deferred_futureEv,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNKSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiE21_M_is_deferred_futureEv
	.type	_ZNKSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiE21_M_is_deferred_futureEv, @function
_ZNKSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiE21_M_is_deferred_futureEv:
.LFB4651:
	.cfi_startproc
	movl	$1, %eax
	ret
	.cfi_endproc
.LFE4651:
	.size	_ZNKSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiE21_M_is_deferred_futureEv, .-_ZNKSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiE21_M_is_deferred_futureEv
	.section	.text._ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info,"axG",@progbits,_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info
	.type	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info, @function
_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info:
.LFB4643:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	movq	8(%rsi), %rdi
	cmpq	$_ZTSSt19_Sp_make_shared_tag, %rdi
	je	.L17
	cmpb	$42, (%rdi)
	je	.L20
	movl	$_ZTSSt19_Sp_make_shared_tag, %esi
	call	strcmp
	testl	%eax, %eax
	jne	.L20
.L17:
	leaq	16(%rbx), %rax
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L20:
	.cfi_restore_state
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE4643:
	.size	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info, .-_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info
	.section	.text._ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info,"axG",@progbits,_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info
	.type	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info, @function
_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info:
.LFB4636:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	movq	8(%rsi), %rdi
	cmpq	$_ZTSSt19_Sp_make_shared_tag, %rdi
	je	.L26
	cmpb	$42, (%rdi)
	je	.L29
	movl	$_ZTSSt19_Sp_make_shared_tag, %esi
	call	strcmp
	testl	%eax, %eax
	jne	.L29
.L26:
	leaq	16(%rbx), %rax
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L29:
	.cfi_restore_state
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE4636:
	.size	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info, .-_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info
	.section	.text._ZNSt13__future_base21_Async_state_commonV217_M_complete_asyncEv,"axG",@progbits,_ZNSt13__future_base21_Async_state_commonV217_M_complete_asyncEv,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt13__future_base21_Async_state_commonV217_M_complete_asyncEv
	.type	_ZNSt13__future_base21_Async_state_commonV217_M_complete_asyncEv, @function
_ZNSt13__future_base21_Async_state_commonV217_M_complete_asyncEv:
.LFB2988:
	.cfi_startproc
	subq	$72, %rsp
	.cfi_def_cfa_offset 80
	leaq	32(%rdi), %rax
	addq	$40, %rdi
	leaq	32(%rsp), %rcx
	leaq	48(%rsp), %rdx
	movq	$_ZNSt6thread4joinEv, 32(%rsp)
	movq	%rax, 24(%rsp)
	leaq	24(%rsp), %rax
	movq	$0, 40(%rsp)
	movq	%rcx, 8(%rsp)
	movq	8(%rsp), %xmm0
	movq	%rax, 8(%rsp)
	movq	_ZSt15__once_callable@gottpoff(%rip), %rax
	movhps	8(%rsp), %xmm0
	movaps	%xmm0, 48(%rsp)
	movq	%rdx, %fs:(%rax)
	movq	_ZSt11__once_call@gottpoff(%rip), %rax
	movq	$_ZZSt9call_onceIMSt6threadFvvEJPS0_EEvRSt9once_flagOT_DpOT0_ENUlvE0_4_FUNEv, %fs:(%rax)
	movl	$_ZL28__gthrw___pthread_key_createPjPFvPvE, %eax
	testq	%rax, %rax
	je	.L37
	movl	$__once_proxy, %esi
	call	_ZL20__gthrw_pthread_oncePiPFvvE
	testl	%eax, %eax
	jne	.L35
	addq	$72, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L37:
	.cfi_restore_state
	orl	$-1, %eax
.L35:
	movl	%eax, %edi
	call	_ZSt20__throw_system_errori
	.cfi_endproc
.LFE2988:
	.size	_ZNSt13__future_base21_Async_state_commonV217_M_complete_asyncEv, .-_ZNSt13__future_base21_Async_state_commonV217_M_complete_asyncEv
	.section	.text._ZNSt13__future_base13_State_baseV2D0Ev,"axG",@progbits,_ZNSt13__future_base13_State_baseV2D5Ev,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt13__future_base13_State_baseV2D0Ev
	.type	_ZNSt13__future_base13_State_baseV2D0Ev, @function
_ZNSt13__future_base13_State_baseV2D0Ev:
.LFB4370:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA4370
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	movq	$_ZTVNSt13__future_base13_State_baseV2E+16, (%rdi)
	movq	8(%rdi), %rdi
	testq	%rdi, %rdi
	je	.L40
	movq	(%rdi), %rax
	call	*(%rax)
.L40:
	movq	%rbx, %rdi
	movl	$32, %esi
	popq	%rbx
	.cfi_def_cfa_offset 8
	jmp	_ZdlPvm
	.cfi_endproc
.LFE4370:
	.section	.gcc_except_table._ZNSt13__future_base13_State_baseV2D0Ev,"aG",@progbits,_ZNSt13__future_base13_State_baseV2D5Ev,comdat
.LLSDA4370:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE4370-.LLSDACSB4370
.LLSDACSB4370:
.LLSDACSE4370:
	.section	.text._ZNSt13__future_base13_State_baseV2D0Ev,"axG",@progbits,_ZNSt13__future_base13_State_baseV2D5Ev,comdat
	.size	_ZNSt13__future_base13_State_baseV2D0Ev, .-_ZNSt13__future_base13_State_baseV2D0Ev
	.section	.text._ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED0Ev,"axG",@progbits,_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED5Ev,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED0Ev
	.type	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED0Ev, @function
_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED0Ev:
.LFB4640:
	.cfi_startproc
	movl	$80, %esi
	jmp	_ZdlPvm
	.cfi_endproc
.LFE4640:
	.size	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED0Ev, .-_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED0Ev
	.section	.text._ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED0Ev,"axG",@progbits,_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED5Ev,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED0Ev
	.type	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED0Ev, @function
_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED0Ev:
.LFB4633:
	.cfi_startproc
	movl	$64, %esi
	jmp	_ZdlPvm
	.cfi_endproc
.LFE4633:
	.size	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED0Ev, .-_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED0Ev
	.section	.text._ZNSt13__future_base7_ResultIiED2Ev,"axG",@progbits,_ZNSt13__future_base7_ResultIiED5Ev,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt13__future_base7_ResultIiED2Ev
	.type	_ZNSt13__future_base7_ResultIiED2Ev, @function
_ZNSt13__future_base7_ResultIiED2Ev:
.LFB4654:
	.cfi_startproc
	movq	$_ZTVNSt13__future_base7_ResultIiEE+16, (%rdi)
	jmp	_ZNSt13__future_base12_Result_baseD2Ev
	.cfi_endproc
.LFE4654:
	.size	_ZNSt13__future_base7_ResultIiED2Ev, .-_ZNSt13__future_base7_ResultIiED2Ev
	.weak	_ZNSt13__future_base7_ResultIiED1Ev
	.set	_ZNSt13__future_base7_ResultIiED1Ev,_ZNSt13__future_base7_ResultIiED2Ev
	.section	.text._ZNSt13__future_base7_ResultIiED0Ev,"axG",@progbits,_ZNSt13__future_base7_ResultIiED5Ev,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt13__future_base7_ResultIiED0Ev
	.type	_ZNSt13__future_base7_ResultIiED0Ev, @function
_ZNSt13__future_base7_ResultIiED0Ev:
.LFB4656:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	movq	$_ZTVNSt13__future_base7_ResultIiEE+16, (%rdi)
	call	_ZNSt13__future_base12_Result_baseD2Ev
	movq	%rbx, %rdi
	movl	$24, %esi
	popq	%rbx
	.cfi_def_cfa_offset 8
	jmp	_ZdlPvm
	.cfi_endproc
.LFE4656:
	.size	_ZNSt13__future_base7_ResultIiED0Ev, .-_ZNSt13__future_base7_ResultIiED0Ev
	.section	.text._ZNSt13__future_base21_Async_state_commonV2D2Ev,"axG",@progbits,_ZNSt13__future_base21_Async_state_commonV2D5Ev,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt13__future_base21_Async_state_commonV2D2Ev
	.type	_ZNSt13__future_base21_Async_state_commonV2D2Ev, @function
_ZNSt13__future_base21_Async_state_commonV2D2Ev:
.LFB4384:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA4384
	cmpq	$0, 32(%rdi)
	movq	$_ZTVNSt13__future_base21_Async_state_commonV2E+16, (%rdi)
	jne	.L56
	movq	$_ZTVNSt13__future_base13_State_baseV2E+16, (%rdi)
	movq	8(%rdi), %rdi
	testq	%rdi, %rdi
	je	.L50
	movq	(%rdi), %rax
	jmp	*(%rax)
	.p2align 4,,10
	.p2align 3
.L50:
	rep ret
.L56:
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	call	_ZSt9terminatev
	.cfi_endproc
.LFE4384:
	.section	.gcc_except_table._ZNSt13__future_base21_Async_state_commonV2D2Ev,"aG",@progbits,_ZNSt13__future_base21_Async_state_commonV2D5Ev,comdat
.LLSDA4384:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE4384-.LLSDACSB4384
.LLSDACSB4384:
.LLSDACSE4384:
	.section	.text._ZNSt13__future_base21_Async_state_commonV2D2Ev,"axG",@progbits,_ZNSt13__future_base21_Async_state_commonV2D5Ev,comdat
	.size	_ZNSt13__future_base21_Async_state_commonV2D2Ev, .-_ZNSt13__future_base21_Async_state_commonV2D2Ev
	.weak	_ZNSt13__future_base21_Async_state_commonV2D1Ev
	.set	_ZNSt13__future_base21_Async_state_commonV2D1Ev,_ZNSt13__future_base21_Async_state_commonV2D2Ev
	.section	.text._ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv,"axG",@progbits,_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv
	.type	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv, @function
_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv:
.LFB4635:
	.cfi_startproc
	jmp	_ZdlPv
	.cfi_endproc
.LFE4635:
	.size	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv, .-_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv
	.section	.text._ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv,"axG",@progbits,_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv
	.type	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv, @function
_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv:
.LFB4642:
	.cfi_startproc
	jmp	_ZdlPv
	.cfi_endproc
.LFE4642:
	.size	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv, .-_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv
	.section	.text._ZNSt17_Function_handlerIFSt10unique_ptrINSt13__future_base12_Result_baseENS2_8_DeleterEEvENS1_12_Task_setterIS0_INS1_7_ResultIiEES3_ENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEEE9_M_invokeERKSt9_Any_data,"axG",@progbits,_ZNSt17_Function_handlerIFSt10unique_ptrINSt13__future_base12_Result_baseENS2_8_DeleterEEvENS1_12_Task_setterIS0_INS1_7_ResultIiEES3_ENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEEE9_M_invokeERKSt9_Any_data,comdat
	.p2align 4,,15
	.weak	_ZNSt17_Function_handlerIFSt10unique_ptrINSt13__future_base12_Result_baseENS2_8_DeleterEEvENS1_12_Task_setterIS0_INS1_7_ResultIiEES3_ENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEEE9_M_invokeERKSt9_Any_data
	.type	_ZNSt17_Function_handlerIFSt10unique_ptrINSt13__future_base12_Result_baseENS2_8_DeleterEEvENS1_12_Task_setterIS0_INS1_7_ResultIiEES3_ENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEEE9_M_invokeERKSt9_Any_data, @function
_ZNSt17_Function_handlerIFSt10unique_ptrINSt13__future_base12_Result_baseENS2_8_DeleterEEvENS1_12_Task_setterIS0_INS1_7_ResultIiEES3_ENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEEE9_M_invokeERKSt9_Any_data:
.LFB4452:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA4452
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	movq	%rdi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movq	%rsi, %rbx
	subq	$32, %rsp
	.cfi_def_cfa_offset 64
	movq	(%rsi), %rax
	movq	(%rax), %r12
	movq	8(%rsi), %rax
.LEHB0:
	call	*(%rax)
.LEHE0:
	movl	%eax, 16(%r12)
	movb	$1, 20(%r12)
.L63:
	movq	(%rbx), %rax
	movq	(%rax), %rdx
	movq	$0, (%rax)
	movq	%rbp, %rax
	movq	%rdx, 0(%rbp)
	addq	$32, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 32
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
.L66:
	.cfi_restore_state
	subq	$1, %rdx
	movq	%rax, %rdi
	jne	.L68
	call	__cxa_begin_catch
.LEHB1:
	call	__cxa_rethrow
.LEHE1:
.L68:
	call	__cxa_begin_catch
	leaq	16(%rsp), %rdi
	call	_ZSt17current_exceptionv
	movq	(%rbx), %rax
	movq	%rsp, %rdi
	movq	(%rax), %rsi
	movq	16(%rsp), %rax
	movq	$0, 16(%rsp)
	addq	$8, %rsi
	movq	%rax, (%rsp)
	call	_ZNSt15__exception_ptr13exception_ptr4swapERS0_
	movq	%rsp, %rdi
	call	_ZNSt15__exception_ptr13exception_ptrD1Ev
	leaq	16(%rsp), %rdi
	call	_ZNSt15__exception_ptr13exception_ptrD1Ev
.LEHB2:
	call	__cxa_end_catch
	jmp	.L63
.L65:
	movq	%rax, %rbx
	call	__cxa_end_catch
	movq	%rbx, %rdi
	call	_Unwind_Resume
.LEHE2:
	.cfi_endproc
.LFE4452:
	.section	.gcc_except_table._ZNSt17_Function_handlerIFSt10unique_ptrINSt13__future_base12_Result_baseENS2_8_DeleterEEvENS1_12_Task_setterIS0_INS1_7_ResultIiEES3_ENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEEE9_M_invokeERKSt9_Any_data,"aG",@progbits,_ZNSt17_Function_handlerIFSt10unique_ptrINSt13__future_base12_Result_baseENS2_8_DeleterEEvENS1_12_Task_setterIS0_INS1_7_ResultIiEES3_ENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEEE9_M_invokeERKSt9_Any_data,comdat
	.align 4
.LLSDA4452:
	.byte	0xff
	.byte	0x3
	.uleb128 .LLSDATT4452-.LLSDATTD4452
.LLSDATTD4452:
	.byte	0x1
	.uleb128 .LLSDACSE4452-.LLSDACSB4452
.LLSDACSB4452:
	.uleb128 .LEHB0-.LFB4452
	.uleb128 .LEHE0-.LEHB0
	.uleb128 .L66-.LFB4452
	.uleb128 0x3
	.uleb128 .LEHB1-.LFB4452
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L65-.LFB4452
	.uleb128 0
	.uleb128 .LEHB2-.LFB4452
	.uleb128 .LEHE2-.LEHB2
	.uleb128 0
	.uleb128 0
.LLSDACSE4452:
	.byte	0x2
	.byte	0
	.byte	0x1
	.byte	0x7d
	.align 4
	.long	0

	.long	_ZTIN10__cxxabiv115__forced_unwindE
.LLSDATT4452:
	.section	.text._ZNSt17_Function_handlerIFSt10unique_ptrINSt13__future_base12_Result_baseENS2_8_DeleterEEvENS1_12_Task_setterIS0_INS1_7_ResultIiEES3_ENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEEE9_M_invokeERKSt9_Any_data,"axG",@progbits,_ZNSt17_Function_handlerIFSt10unique_ptrINSt13__future_base12_Result_baseENS2_8_DeleterEEvENS1_12_Task_setterIS0_INS1_7_ResultIiEES3_ENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEEE9_M_invokeERKSt9_Any_data,comdat
	.size	_ZNSt17_Function_handlerIFSt10unique_ptrINSt13__future_base12_Result_baseENS2_8_DeleterEEvENS1_12_Task_setterIS0_INS1_7_ResultIiEES3_ENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEEE9_M_invokeERKSt9_Any_data, .-_ZNSt17_Function_handlerIFSt10unique_ptrINSt13__future_base12_Result_baseENS2_8_DeleterEEvENS1_12_Task_setterIS0_INS1_7_ResultIiEES3_ENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEEE9_M_invokeERKSt9_Any_data
	.section	.text._ZNSt13__future_base13_State_baseV29_M_do_setEPSt8functionIFSt10unique_ptrINS_12_Result_baseENS3_8_DeleterEEvEEPb,"axG",@progbits,_ZNSt13__future_base13_State_baseV29_M_do_setEPSt8functionIFSt10unique_ptrINS_12_Result_baseENS3_8_DeleterEEvEEPb,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt13__future_base13_State_baseV29_M_do_setEPSt8functionIFSt10unique_ptrINS_12_Result_baseENS3_8_DeleterEEvEEPb
	.type	_ZNSt13__future_base13_State_baseV29_M_do_setEPSt8functionIFSt10unique_ptrINS_12_Result_baseENS3_8_DeleterEEvEEPb, @function
_ZNSt13__future_base13_State_baseV29_M_do_setEPSt8functionIFSt10unique_ptrINS_12_Result_baseENS3_8_DeleterEEvEEPb:
.LFB2809:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA2809
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$24, %rsp
	.cfi_def_cfa_offset 48
	cmpq	$0, 16(%rsi)
	je	.L77
	movq	%rdi, %rbx
	movq	%rdx, %rbp
	movq	%rsp, %rdi
.LEHB3:
	call	*24(%rsi)
.LEHE3:
	movq	8(%rbx), %rdi
	movq	(%rsp), %rax
	movb	$1, 0(%rbp)
	testq	%rdi, %rdi
	movq	%rax, 8(%rbx)
	movq	%rdi, (%rsp)
	je	.L70
	movq	(%rdi), %rax
	call	*(%rax)
.L70:
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
.L77:
	.cfi_restore_state
.LEHB4:
	call	_ZSt25__throw_bad_function_callv
.LEHE4:
	.cfi_endproc
.LFE2809:
	.section	.gcc_except_table._ZNSt13__future_base13_State_baseV29_M_do_setEPSt8functionIFSt10unique_ptrINS_12_Result_baseENS3_8_DeleterEEvEEPb,"aG",@progbits,_ZNSt13__future_base13_State_baseV29_M_do_setEPSt8functionIFSt10unique_ptrINS_12_Result_baseENS3_8_DeleterEEvEEPb,comdat
.LLSDA2809:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE2809-.LLSDACSB2809
.LLSDACSB2809:
	.uleb128 .LEHB3-.LFB2809
	.uleb128 .LEHE3-.LEHB3
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB4-.LFB2809
	.uleb128 .LEHE4-.LEHB4
	.uleb128 0
	.uleb128 0
.LLSDACSE2809:
	.section	.text._ZNSt13__future_base13_State_baseV29_M_do_setEPSt8functionIFSt10unique_ptrINS_12_Result_baseENS3_8_DeleterEEvEEPb,"axG",@progbits,_ZNSt13__future_base13_State_baseV29_M_do_setEPSt8functionIFSt10unique_ptrINS_12_Result_baseENS3_8_DeleterEEvEEPb,comdat
	.size	_ZNSt13__future_base13_State_baseV29_M_do_setEPSt8functionIFSt10unique_ptrINS_12_Result_baseENS3_8_DeleterEEvEEPb, .-_ZNSt13__future_base13_State_baseV29_M_do_setEPSt8functionIFSt10unique_ptrINS_12_Result_baseENS3_8_DeleterEEvEEPb
	.section	.text._ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEED2Ev,"axG",@progbits,_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEED5Ev,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEED2Ev
	.type	_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEED2Ev, @function
_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEED2Ev:
.LFB4491:
	.cfi_startproc
	movq	$_ZTVNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEEE+16, (%rdi)
	jmp	_ZNSt6thread6_StateD2Ev
	.cfi_endproc
.LFE4491:
	.size	_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEED2Ev, .-_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEED2Ev
	.weak	_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEED1Ev
	.set	_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEED1Ev,_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEED2Ev
	.section	.text._ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEED0Ev,"axG",@progbits,_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEED5Ev,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEED0Ev
	.type	_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEED0Ev, @function
_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEED0Ev:
.LFB4493:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	movq	$_ZTVNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEEE+16, (%rdi)
	call	_ZNSt6thread6_StateD2Ev
	movq	%rbx, %rdi
	movl	$16, %esi
	popq	%rbx
	.cfi_def_cfa_offset 8
	jmp	_ZdlPvm
	.cfi_endproc
.LFE4493:
	.size	_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEED0Ev, .-_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEED0Ev
	.section	.text._ZNSt13__future_base7_ResultIiE10_M_destroyEv,"axG",@progbits,_ZNSt13__future_base7_ResultIiE10_M_destroyEv,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt13__future_base7_ResultIiE10_M_destroyEv
	.type	_ZNSt13__future_base7_ResultIiE10_M_destroyEv, @function
_ZNSt13__future_base7_ResultIiE10_M_destroyEv:
.LFB4652:
	.cfi_startproc
	movq	(%rdi), %rax
	movq	16(%rax), %rax
	cmpq	$_ZNSt13__future_base7_ResultIiED0Ev, %rax
	jne	.L82
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	movq	$_ZTVNSt13__future_base7_ResultIiEE+16, (%rdi)
	call	_ZNSt13__future_base12_Result_baseD2Ev
	movq	%rbx, %rdi
	movl	$24, %esi
	popq	%rbx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	jmp	_ZdlPvm
	.p2align 4,,10
	.p2align 3
.L82:
	jmp	*%rax
	.cfi_endproc
.LFE4652:
	.size	_ZNSt13__future_base7_ResultIiE10_M_destroyEv, .-_ZNSt13__future_base7_ResultIiE10_M_destroyEv
	.section	.text._ZZSt9call_onceIMSt6threadFvvEJPS0_EEvRSt9once_flagOT_DpOT0_ENUlvE0_4_FUNEv,"axG",@progbits,_ZZSt9call_onceIMSt6threadFvvEJPS0_EEvRSt9once_flagOT_DpOT0_ENKUlvE0_clEv,comdat
	.p2align 4,,15
	.weak	_ZZSt9call_onceIMSt6threadFvvEJPS0_EEvRSt9once_flagOT_DpOT0_ENUlvE0_4_FUNEv
	.type	_ZZSt9call_onceIMSt6threadFvvEJPS0_EEvRSt9once_flagOT_DpOT0_ENUlvE0_4_FUNEv, @function
_ZZSt9call_onceIMSt6threadFvvEJPS0_EEvRSt9once_flagOT_DpOT0_ENUlvE0_4_FUNEv:
.LFB3460:
	.cfi_startproc
	movq	_ZSt15__once_callable@gottpoff(%rip), %rax
	movq	%fs:(%rax), %rdx
	movq	(%rdx), %rax
	movq	8(%rdx), %rdx
	movq	8(%rax), %rdi
	movq	(%rax), %rax
	addq	(%rdx), %rdi
	testb	$1, %al
	je	.L87
	movq	(%rdi), %rdx
	movq	-1(%rdx,%rax), %rax
.L87:
	jmp	*%rax
	.cfi_endproc
.LFE3460:
	.size	_ZZSt9call_onceIMSt6threadFvvEJPS0_EEvRSt9once_flagOT_DpOT0_ENUlvE0_4_FUNEv, .-_ZZSt9call_onceIMSt6threadFvvEJPS0_EEvRSt9once_flagOT_DpOT0_ENUlvE0_4_FUNEv
	.section	.text._ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED2Ev,"axG",@progbits,_ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED5Ev,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED2Ev
	.type	_ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED2Ev, @function
_ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED2Ev:
.LFB4371:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA4371
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	32(%rdi), %rbp
	movq	$_ZTVNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE+16, (%rdi)
	testq	%rbp, %rbp
	je	.L92
	movq	0(%rbp), %rax
	movq	(%rax), %rdx
	cmpq	$_ZNSt13__future_base7_ResultIiE10_M_destroyEv, %rdx
	jne	.L93
	movq	16(%rax), %rax
	cmpq	$_ZNSt13__future_base7_ResultIiED0Ev, %rax
	jne	.L94
	movq	%rbp, %rdi
	movq	$_ZTVNSt13__future_base7_ResultIiEE+16, 0(%rbp)
	call	_ZNSt13__future_base12_Result_baseD2Ev
	movl	$24, %esi
	movq	%rbp, %rdi
	call	_ZdlPvm
.L92:
	movq	8(%rbx), %rdi
	movq	$_ZTVNSt13__future_base13_State_baseV2E+16, (%rbx)
	testq	%rdi, %rdi
	je	.L91
	movq	(%rdi), %rax
	movq	(%rax), %rax
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	*%rax
	.p2align 4,,10
	.p2align 3
.L91:
	.cfi_restore_state
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L93:
	.cfi_restore_state
	movq	%rbp, %rdi
	call	*%rdx
	jmp	.L92
	.p2align 4,,10
	.p2align 3
.L94:
	movq	%rbp, %rdi
	call	*%rax
	jmp	.L92
	.cfi_endproc
.LFE4371:
	.section	.gcc_except_table._ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED2Ev,"aG",@progbits,_ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED5Ev,comdat
.LLSDA4371:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE4371-.LLSDACSB4371
.LLSDACSB4371:
.LLSDACSE4371:
	.section	.text._ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED2Ev,"axG",@progbits,_ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED5Ev,comdat
	.size	_ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED2Ev, .-_ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED2Ev
	.weak	_ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED1Ev
	.set	_ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED1Ev,_ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED2Ev
	.section	.text._ZZSt9call_onceIMNSt13__future_base13_State_baseV2EFvPSt8functionIFSt10unique_ptrINS0_12_Result_baseENS4_8_DeleterEEvEEPbEJPS1_S9_SA_EEvRSt9once_flagOT_DpOT0_ENUlvE0_4_FUNEv,"axG",@progbits,_ZZSt9call_onceIMNSt13__future_base13_State_baseV2EFvPSt8functionIFSt10unique_ptrINS0_12_Result_baseENS4_8_DeleterEEvEEPbEJPS1_S9_SA_EEvRSt9once_flagOT_DpOT0_ENKUlvE0_clEv,comdat
	.p2align 4,,15
	.weak	_ZZSt9call_onceIMNSt13__future_base13_State_baseV2EFvPSt8functionIFSt10unique_ptrINS0_12_Result_baseENS4_8_DeleterEEvEEPbEJPS1_S9_SA_EEvRSt9once_flagOT_DpOT0_ENUlvE0_4_FUNEv
	.type	_ZZSt9call_onceIMNSt13__future_base13_State_baseV2EFvPSt8functionIFSt10unique_ptrINS0_12_Result_baseENS4_8_DeleterEEvEEPbEJPS1_S9_SA_EEvRSt9once_flagOT_DpOT0_ENUlvE0_4_FUNEv, @function
_ZZSt9call_onceIMNSt13__future_base13_State_baseV2EFvPSt8functionIFSt10unique_ptrINS0_12_Result_baseENS4_8_DeleterEEvEEPbEJPS1_S9_SA_EEvRSt9once_flagOT_DpOT0_ENUlvE0_4_FUNEv:
.LFB3341:
	.cfi_startproc
	movq	_ZSt15__once_callable@gottpoff(%rip), %rax
	movq	%fs:(%rax), %rax
	movq	16(%rax), %rdx
	movq	(%rax), %rcx
	movq	(%rdx), %rsi
	movq	24(%rax), %rdx
	movq	8(%rax), %rax
	movq	8(%rcx), %rdi
	movq	(%rdx), %rdx
	addq	(%rax), %rdi
	movq	(%rcx), %rax
	testb	$1, %al
	je	.L101
	movq	(%rdi), %rcx
	movq	-1(%rcx,%rax), %rax
.L101:
	jmp	*%rax
	.cfi_endproc
.LFE3341:
	.size	_ZZSt9call_onceIMNSt13__future_base13_State_baseV2EFvPSt8functionIFSt10unique_ptrINS0_12_Result_baseENS4_8_DeleterEEvEEPbEJPS1_S9_SA_EEvRSt9once_flagOT_DpOT0_ENUlvE0_4_FUNEv, .-_ZZSt9call_onceIMNSt13__future_base13_State_baseV2EFvPSt8functionIFSt10unique_ptrINS0_12_Result_baseENS4_8_DeleterEEvEEPbEJPS1_S9_SA_EEvRSt9once_flagOT_DpOT0_ENUlvE0_4_FUNEv
	.section	.text._ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiE17_M_complete_asyncEv,"axG",@progbits,_ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiE17_M_complete_asyncEv,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiE17_M_complete_asyncEv
	.type	_ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiE17_M_complete_asyncEv, @function
_ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiE17_M_complete_asyncEv:
.LFB4650:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA4650
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	leaq	32(%rdi), %rax
	subq	$136, %rsp
	.cfi_def_cfa_offset 160
	movq	%rax, 8(%rsp)
	leaq	40(%rdi), %rax
	leaq	48(%rsp), %rcx
	movq	8(%rsp), %xmm0
	leaq	32(%rsp), %rsi
	movq	%rax, 8(%rsp)
	leaq	23(%rsp), %rax
	leaq	96(%rsp), %rdx
	movhps	8(%rsp), %xmm0
	movq	$_ZNSt14_Function_base13_Base_managerINSt13__future_base12_Task_setterISt10unique_ptrINS1_7_ResultIiEENS1_12_Result_base8_DeleterEENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEEE10_M_managerERSt9_Any_dataRKSI_St18_Manager_operation, 8(%rsp)
	leaq	64(%rsp), %rbp
	movq	%rax, 40(%rsp)
	leaq	24(%rsp), %rax
	movb	$0, 23(%rsp)
	movaps	%xmm0, 64(%rsp)
	movq	%rbp, 32(%rsp)
	movq	%rdi, 24(%rsp)
	movq	8(%rsp), %xmm0
	movq	%rcx, 8(%rsp)
	movhps	.LC0(%rip), %xmm0
	movq	$_ZNSt13__future_base13_State_baseV29_M_do_setEPSt8functionIFSt10unique_ptrINS_12_Result_baseENS3_8_DeleterEEvEEPb, 48(%rsp)
	movq	$0, 56(%rsp)
	movaps	%xmm0, 80(%rsp)
	movq	8(%rsp), %xmm0
	movq	%rax, 8(%rsp)
	movhps	8(%rsp), %xmm0
	leaq	40(%rsp), %rax
	movq	%rsi, 8(%rsp)
	movaps	%xmm0, 96(%rsp)
	movq	8(%rsp), %xmm0
	movq	%rax, 8(%rsp)
	movq	_ZSt15__once_callable@gottpoff(%rip), %rax
	movhps	8(%rsp), %xmm0
	movaps	%xmm0, 112(%rsp)
	movq	%rdx, %fs:(%rax)
	movq	_ZSt11__once_call@gottpoff(%rip), %rax
	movq	$_ZZSt9call_onceIMNSt13__future_base13_State_baseV2EFvPSt8functionIFSt10unique_ptrINS0_12_Result_baseENS4_8_DeleterEEvEEPbEJPS1_S9_SA_EEvRSt9once_flagOT_DpOT0_ENUlvE0_4_FUNEv, %fs:(%rax)
	movl	$_ZL28__gthrw___pthread_key_createPjPFvPvE, %eax
	testq	%rax, %rax
	je	.L114
	movq	%rdi, %rbx
	movl	$__once_proxy, %esi
	addq	$24, %rdi
.LEHB5:
	call	_ZL20__gthrw_pthread_oncePiPFvvE
.LEHE5:
	testl	%eax, %eax
	jne	.L106
	cmpb	$0, 23(%rsp)
	jne	.L108
.L111:
	movq	80(%rsp), %rax
	testq	%rax, %rax
	je	.L105
	movl	$3, %edx
	movq	%rbp, %rsi
	movq	%rbp, %rdi
	call	*%rax
.L105:
	addq	$136, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L108:
	.cfi_restore_state
	leaq	16(%rbx), %rdi
	movl	$1, %eax
	xchgl	(%rdi), %eax
	testl	%eax, %eax
	jns	.L111
.LEHB6:
	call	_ZNSt28__atomic_futex_unsigned_base19_M_futex_notify_allEPj
	jmp	.L111
.L114:
	orl	$-1, %eax
.L106:
	movl	%eax, %edi
	call	_ZSt20__throw_system_errori
.LEHE6:
.L115:
	movq	%rax, %rbx
	movq	80(%rsp), %rax
	testq	%rax, %rax
	je	.L113
	movl	$3, %edx
	movq	%rbp, %rsi
	movq	%rbp, %rdi
	call	*%rax
.L113:
	movq	%rbx, %rdi
.LEHB7:
	call	_Unwind_Resume
.LEHE7:
	.cfi_endproc
.LFE4650:
	.section	.gcc_except_table._ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiE17_M_complete_asyncEv,"aG",@progbits,_ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiE17_M_complete_asyncEv,comdat
.LLSDA4650:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE4650-.LLSDACSB4650
.LLSDACSB4650:
	.uleb128 .LEHB5-.LFB4650
	.uleb128 .LEHE5-.LEHB5
	.uleb128 .L115-.LFB4650
	.uleb128 0
	.uleb128 .LEHB6-.LFB4650
	.uleb128 .LEHE6-.LEHB6
	.uleb128 .L115-.LFB4650
	.uleb128 0
	.uleb128 .LEHB7-.LFB4650
	.uleb128 .LEHE7-.LEHB7
	.uleb128 0
	.uleb128 0
.LLSDACSE4650:
	.section	.text._ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiE17_M_complete_asyncEv,"axG",@progbits,_ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiE17_M_complete_asyncEv,comdat
	.size	_ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiE17_M_complete_asyncEv, .-_ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiE17_M_complete_asyncEv
	.section	.text._ZNSt13__future_base21_Async_state_commonV2D0Ev,"axG",@progbits,_ZNSt13__future_base21_Async_state_commonV2D5Ev,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt13__future_base21_Async_state_commonV2D0Ev
	.type	_ZNSt13__future_base21_Async_state_commonV2D0Ev, @function
_ZNSt13__future_base21_Async_state_commonV2D0Ev:
.LFB4386:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA4386
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	cmpq	$0, 32(%rdi)
	movq	$_ZTVNSt13__future_base21_Async_state_commonV2E+16, (%rdi)
	jne	.L127
	movq	%rdi, %rbx
	movq	$_ZTVNSt13__future_base13_State_baseV2E+16, (%rdi)
	movq	8(%rdi), %rdi
	testq	%rdi, %rdi
	je	.L122
	movq	(%rdi), %rax
	call	*(%rax)
.L122:
	movq	%rbx, %rdi
	movl	$48, %esi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	jmp	_ZdlPvm
.L127:
	.cfi_restore_state
	call	_ZSt9terminatev
	.cfi_endproc
.LFE4386:
	.section	.gcc_except_table._ZNSt13__future_base21_Async_state_commonV2D0Ev,"aG",@progbits,_ZNSt13__future_base21_Async_state_commonV2D5Ev,comdat
.LLSDA4386:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE4386-.LLSDACSB4386
.LLSDACSB4386:
.LLSDACSE4386:
	.section	.text._ZNSt13__future_base21_Async_state_commonV2D0Ev,"axG",@progbits,_ZNSt13__future_base21_Async_state_commonV2D5Ev,comdat
	.size	_ZNSt13__future_base21_Async_state_commonV2D0Ev, .-_ZNSt13__future_base21_Async_state_commonV2D0Ev
	.section	.text._ZNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED2Ev,"axG",@progbits,_ZNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED5Ev,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED2Ev
	.type	_ZNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED2Ev, @function
_ZNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED2Ev:
.LFB4405:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA4405
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	cmpq	$0, 32(%rdi)
	movq	$_ZTVNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE+16, (%rdi)
	jne	.L139
.L129:
	movq	48(%rbx), %rbp
	testq	%rbp, %rbp
	je	.L130
	movq	0(%rbp), %rax
	movq	(%rax), %rdx
	cmpq	$_ZNSt13__future_base7_ResultIiE10_M_destroyEv, %rdx
	jne	.L131
	movq	16(%rax), %rax
	cmpq	$_ZNSt13__future_base7_ResultIiED0Ev, %rax
	jne	.L132
	movq	%rbp, %rdi
	movq	$_ZTVNSt13__future_base7_ResultIiEE+16, 0(%rbp)
	call	_ZNSt13__future_base12_Result_baseD2Ev
	movl	$24, %esi
	movq	%rbp, %rdi
	call	_ZdlPvm
.L130:
	cmpq	$0, 32(%rbx)
	movq	$_ZTVNSt13__future_base21_Async_state_commonV2E+16, (%rbx)
	jne	.L140
	movq	8(%rbx), %rdi
	movq	$_ZTVNSt13__future_base13_State_baseV2E+16, (%rbx)
	testq	%rdi, %rdi
	je	.L128
	movq	(%rdi), %rax
	movq	(%rax), %rax
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	*%rax
	.p2align 4,,10
	.p2align 3
.L128:
	.cfi_restore_state
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L139:
	.cfi_restore_state
	leaq	32(%rdi), %rdi
	call	_ZNSt6thread4joinEv
	jmp	.L129
	.p2align 4,,10
	.p2align 3
.L131:
	movq	%rbp, %rdi
	call	*%rdx
	jmp	.L130
	.p2align 4,,10
	.p2align 3
.L132:
	movq	%rbp, %rdi
	call	*%rax
	jmp	.L130
.L140:
	call	_ZSt9terminatev
	.cfi_endproc
.LFE4405:
	.section	.gcc_except_table._ZNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED2Ev,"aG",@progbits,_ZNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED5Ev,comdat
.LLSDA4405:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE4405-.LLSDACSB4405
.LLSDACSB4405:
.LLSDACSE4405:
	.section	.text._ZNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED2Ev,"axG",@progbits,_ZNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED5Ev,comdat
	.size	_ZNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED2Ev, .-_ZNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED2Ev
	.weak	_ZNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED1Ev
	.set	_ZNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED1Ev,_ZNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED2Ev
	.section	.text._ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv,"axG",@progbits,_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv
	.type	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv, @function
_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv:
.LFB4634:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA4634
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	48(%rdi), %rbp
	movq	$_ZTVNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE+16, 16(%rdi)
	testq	%rbp, %rbp
	je	.L142
	movq	0(%rbp), %rax
	movq	(%rax), %rdx
	cmpq	$_ZNSt13__future_base7_ResultIiE10_M_destroyEv, %rdx
	jne	.L143
	movq	16(%rax), %rax
	cmpq	$_ZNSt13__future_base7_ResultIiED0Ev, %rax
	jne	.L144
	movq	%rbp, %rdi
	movq	$_ZTVNSt13__future_base7_ResultIiEE+16, 0(%rbp)
	call	_ZNSt13__future_base12_Result_baseD2Ev
	movl	$24, %esi
	movq	%rbp, %rdi
	call	_ZdlPvm
.L142:
	movq	24(%rbx), %rdi
	movq	$_ZTVNSt13__future_base13_State_baseV2E+16, 16(%rbx)
	testq	%rdi, %rdi
	je	.L141
	movq	(%rdi), %rax
	movq	(%rax), %rax
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	*%rax
	.p2align 4,,10
	.p2align 3
.L141:
	.cfi_restore_state
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L143:
	.cfi_restore_state
	movq	%rbp, %rdi
	call	*%rdx
	jmp	.L142
	.p2align 4,,10
	.p2align 3
.L144:
	movq	%rbp, %rdi
	call	*%rax
	jmp	.L142
	.cfi_endproc
.LFE4634:
	.section	.gcc_except_table._ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv,"aG",@progbits,_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv,comdat
.LLSDA4634:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE4634-.LLSDACSB4634
.LLSDACSB4634:
.LLSDACSE4634:
	.section	.text._ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv,"axG",@progbits,_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv,comdat
	.size	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv, .-_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv
	.section	.text._ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED0Ev,"axG",@progbits,_ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED5Ev,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED0Ev
	.type	_ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED0Ev, @function
_ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED0Ev:
.LFB4373:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA4373
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	32(%rdi), %rbp
	movq	$_ZTVNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE+16, (%rdi)
	testq	%rbp, %rbp
	je	.L151
	movq	0(%rbp), %rax
	movq	(%rax), %rdx
	cmpq	$_ZNSt13__future_base7_ResultIiE10_M_destroyEv, %rdx
	jne	.L152
	movq	16(%rax), %rax
	cmpq	$_ZNSt13__future_base7_ResultIiED0Ev, %rax
	jne	.L153
	movq	%rbp, %rdi
	movq	$_ZTVNSt13__future_base7_ResultIiEE+16, 0(%rbp)
	call	_ZNSt13__future_base12_Result_baseD2Ev
	movl	$24, %esi
	movq	%rbp, %rdi
	call	_ZdlPvm
.L151:
	movq	8(%rbx), %rdi
	movq	$_ZTVNSt13__future_base13_State_baseV2E+16, (%rbx)
	testq	%rdi, %rdi
	je	.L154
	movq	(%rdi), %rax
	call	*(%rax)
.L154:
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rbx, %rdi
	movl	$48, %esi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	_ZdlPvm
	.p2align 4,,10
	.p2align 3
.L152:
	.cfi_restore_state
	movq	%rbp, %rdi
	call	*%rdx
	jmp	.L151
	.p2align 4,,10
	.p2align 3
.L153:
	movq	%rbp, %rdi
	call	*%rax
	jmp	.L151
	.cfi_endproc
.LFE4373:
	.section	.gcc_except_table._ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED0Ev,"aG",@progbits,_ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED5Ev,comdat
.LLSDA4373:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE4373-.LLSDACSB4373
.LLSDACSB4373:
.LLSDACSE4373:
	.section	.text._ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED0Ev,"axG",@progbits,_ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED5Ev,comdat
	.size	_ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED0Ev, .-_ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED0Ev
	.section	.text._ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv,"axG",@progbits,_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv
	.type	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv, @function
_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv:
.LFB4641:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA4641
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	cmpq	$0, 48(%rdi)
	movq	$_ZTVNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE+16, 16(%rdi)
	jne	.L173
.L163:
	movq	64(%rbx), %rbp
	testq	%rbp, %rbp
	je	.L164
	movq	0(%rbp), %rax
	movq	(%rax), %rdx
	cmpq	$_ZNSt13__future_base7_ResultIiE10_M_destroyEv, %rdx
	jne	.L165
	movq	16(%rax), %rax
	cmpq	$_ZNSt13__future_base7_ResultIiED0Ev, %rax
	jne	.L166
	movq	%rbp, %rdi
	movq	$_ZTVNSt13__future_base7_ResultIiEE+16, 0(%rbp)
	call	_ZNSt13__future_base12_Result_baseD2Ev
	movl	$24, %esi
	movq	%rbp, %rdi
	call	_ZdlPvm
.L164:
	cmpq	$0, 48(%rbx)
	movq	$_ZTVNSt13__future_base21_Async_state_commonV2E+16, 16(%rbx)
	jne	.L174
	movq	24(%rbx), %rdi
	movq	$_ZTVNSt13__future_base13_State_baseV2E+16, 16(%rbx)
	testq	%rdi, %rdi
	je	.L162
	movq	(%rdi), %rax
	movq	(%rax), %rax
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	*%rax
	.p2align 4,,10
	.p2align 3
.L162:
	.cfi_restore_state
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L173:
	.cfi_restore_state
	leaq	48(%rdi), %rdi
	call	_ZNSt6thread4joinEv
	jmp	.L163
	.p2align 4,,10
	.p2align 3
.L165:
	movq	%rbp, %rdi
	call	*%rdx
	jmp	.L164
	.p2align 4,,10
	.p2align 3
.L166:
	movq	%rbp, %rdi
	call	*%rax
	jmp	.L164
.L174:
	call	_ZSt9terminatev
	.cfi_endproc
.LFE4641:
	.section	.gcc_except_table._ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv,"aG",@progbits,_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv,comdat
.LLSDA4641:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE4641-.LLSDACSB4641
.LLSDACSB4641:
.LLSDACSE4641:
	.section	.text._ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv,"axG",@progbits,_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv,comdat
	.size	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv, .-_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv
	.section	.text._ZNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED0Ev,"axG",@progbits,_ZNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED5Ev,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED0Ev
	.type	_ZNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED0Ev, @function
_ZNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED0Ev:
.LFB4407:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA4407
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	cmpq	$0, 32(%rdi)
	movq	$_ZTVNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE+16, (%rdi)
	jne	.L189
.L176:
	movq	48(%rbx), %rbp
	testq	%rbp, %rbp
	je	.L177
	movq	0(%rbp), %rax
	movq	(%rax), %rdx
	cmpq	$_ZNSt13__future_base7_ResultIiE10_M_destroyEv, %rdx
	jne	.L178
	movq	16(%rax), %rax
	cmpq	$_ZNSt13__future_base7_ResultIiED0Ev, %rax
	jne	.L179
	movq	%rbp, %rdi
	movq	$_ZTVNSt13__future_base7_ResultIiEE+16, 0(%rbp)
	call	_ZNSt13__future_base12_Result_baseD2Ev
	movl	$24, %esi
	movq	%rbp, %rdi
	call	_ZdlPvm
.L177:
	cmpq	$0, 32(%rbx)
	movq	$_ZTVNSt13__future_base21_Async_state_commonV2E+16, (%rbx)
	jne	.L190
	movq	8(%rbx), %rdi
	movq	$_ZTVNSt13__future_base13_State_baseV2E+16, (%rbx)
	testq	%rdi, %rdi
	je	.L181
	movq	(%rdi), %rax
	call	*(%rax)
.L181:
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rbx, %rdi
	movl	$64, %esi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	_ZdlPvm
	.p2align 4,,10
	.p2align 3
.L189:
	.cfi_restore_state
	leaq	32(%rdi), %rdi
	call	_ZNSt6thread4joinEv
	jmp	.L176
	.p2align 4,,10
	.p2align 3
.L178:
	movq	%rbp, %rdi
	call	*%rdx
	jmp	.L177
	.p2align 4,,10
	.p2align 3
.L179:
	movq	%rbp, %rdi
	call	*%rax
	jmp	.L177
.L190:
	call	_ZSt9terminatev
	.cfi_endproc
.LFE4407:
	.section	.gcc_except_table._ZNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED0Ev,"aG",@progbits,_ZNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED5Ev,comdat
.LLSDA4407:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE4407-.LLSDACSB4407
.LLSDACSB4407:
.LLSDACSE4407:
	.section	.text._ZNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED0Ev,"axG",@progbits,_ZNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED5Ev,comdat
	.size	_ZNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED0Ev, .-_ZNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED0Ev
	.section	.rodata._ZNSt12future_errorC2ESt10error_code.str1.1,"aMS",@progbits,1
.LC1:
	.string	"basic_string::_M_replace"
.LC2:
	.string	"std::future_error: "
.LC3:
	.string	"basic_string::_M_create"
	.section	.text._ZNSt12future_errorC2ESt10error_code,"axG",@progbits,_ZNSt12future_errorC5ESt10error_code,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt12future_errorC2ESt10error_code
	.type	_ZNSt12future_errorC2ESt10error_code, @function
_ZNSt12future_errorC2ESt10error_code:
.LFB2751:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA2751
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	movq	%rsi, %r15
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	movq	%rdx, %r13
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	movq	%rdi, %r12
	subq	$88, %rsp
	.cfi_def_cfa_offset 144
	movq	(%rdx), %rax
	movl	%esi, %edx
	leaq	48(%rsp), %rdi
	movq	%r13, %rsi
.LEHB8:
	call	*32(%rax)
.LEHE8:
	movq	56(%rsp), %rbx
	movabsq	$9223372036854775807, %rdx
	movq	%rdx, %rax
	subq	%rbx, %rax
	cmpq	$18, %rax
	jbe	.L231
	movq	48(%rsp), %rbp
	leaq	64(%rsp), %rax
	leaq	19(%rbx), %r14
	cmpq	%rax, %rbp
	movl	$15, %eax
	cmovne	64(%rsp), %rax
	cmpq	%rax, %r14
	ja	.L194
	cmpq	$.LC2, %rbp
	jbe	.L232
.L195:
	testq	%rbx, %rbx
	jne	.L233
.L230:
	movdqa	.LC4(%rip), %xmm0
	movl	$14962, %edx
	movw	%dx, 16(%rbp)
	movb	$32, 18(%rbp)
	movups	%xmm0, 0(%rbp)
	movq	48(%rsp), %rcx
.L199:
	leaq	32(%rsp), %rax
	leaq	64(%rsp), %rdx
	movq	%r14, 56(%rsp)
	movb	$0, 19(%rcx,%rbx)
	movq	%rax, 16(%rsp)
	movq	48(%rsp), %rax
	cmpq	%rdx, %rax
	je	.L234
	movq	%rax, 16(%rsp)
	movq	64(%rsp), %rax
	movq	%rax, 32(%rsp)
.L210:
	movq	56(%rsp), %rax
	leaq	16(%rsp), %rsi
	movq	%r12, %rdi
	movq	$0, 56(%rsp)
	movb	$0, 64(%rsp)
	movq	%rax, 24(%rsp)
	leaq	64(%rsp), %rax
	movq	%rax, 48(%rsp)
.LEHB9:
	call	_ZNSt11logic_errorC2ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
.LEHE9:
	movq	16(%rsp), %rdi
	leaq	32(%rsp), %rax
	cmpq	%rax, %rdi
	je	.L211
	call	_ZdlPv
.L211:
	movq	48(%rsp), %rdi
	leaq	64(%rsp), %rax
	cmpq	%rax, %rdi
	je	.L212
	call	_ZdlPv
.L212:
	movq	%r15, 16(%r12)
	movq	%r13, 24(%r12)
	movq	$_ZTVSt12future_error+16, (%r12)
	addq	$88, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L232:
	.cfi_restore_state
	leaq	0(%rbp,%rbx), %rax
	cmpq	$.LC2, %rax
	jb	.L195
	testq	%rbx, %rbx
	je	.L201
	cmpq	$1, %rbx
	je	.L235
	leaq	19(%rbp), %rdi
	movq	%rbx, %rdx
	movq	%rbp, %rsi
	call	memmove
	.p2align 4,,10
	.p2align 3
.L201:
	cmpq	$.LC2+19, %rbp
	jnb	.L230
	movzwl	.LC2+35(%rip), %eax
	movdqu	.LC2+19(%rip), %xmm0
	movups	%xmm0, 0(%rbp)
	movw	%ax, 16(%rbp)
	movzbl	.LC2+37(%rip), %eax
	movq	48(%rsp), %rcx
	movb	%al, 18(%rbp)
	jmp	.L199
	.p2align 4,,10
	.p2align 3
.L233:
	cmpq	$1, %rbx
	je	.L236
	leaq	19(%rbp), %rdi
	movq	%rbx, %rdx
	movq	%rbp, %rsi
	call	memmove
	jmp	.L230
	.p2align 4,,10
	.p2align 3
.L194:
	testq	%r14, %r14
	js	.L237
	leaq	(%rax,%rax), %rbp
	cmpq	%rbp, %r14
	jnb	.L238
	testq	%rbp, %rbp
	leaq	1(%rbp), %rdi
	jns	.L205
	movabsq	$-9223372036854775808, %rdi
	movq	%rdx, %rbp
	jmp	.L205
	.p2align 4,,10
	.p2align 3
.L234:
	movdqa	64(%rsp), %xmm0
	movaps	%xmm0, 32(%rsp)
	jmp	.L210
	.p2align 4,,10
	.p2align 3
.L236:
	movzbl	0(%rbp), %eax
	movb	%al, 19(%rbp)
	jmp	.L230
	.p2align 4,,10
	.p2align 3
.L238:
	leaq	20(%rbx), %rdi
	movq	%r14, %rbp
.L205:
.LEHB10:
	call	_Znwm
	movdqa	.LC4(%rip), %xmm0
	movq	%rax, %rcx
	testq	%rbx, %rbx
	movq	48(%rsp), %r8
	movups	%xmm0, (%rax)
	movl	$14962, %eax
	movb	$32, 18(%rcx)
	movw	%ax, 16(%rcx)
	je	.L206
	cmpq	$1, %rbx
	jne	.L207
	movzbl	(%r8), %eax
	movb	%al, 19(%rcx)
.L206:
	leaq	64(%rsp), %rax
	cmpq	%rax, %r8
	je	.L208
	movq	%r8, %rdi
	movq	%rcx, (%rsp)
	call	_ZdlPv
	movq	(%rsp), %rcx
.L208:
	movq	%rcx, 48(%rsp)
	movq	%rbp, 64(%rsp)
	jmp	.L199
	.p2align 4,,10
	.p2align 3
.L207:
	leaq	19(%rcx), %rdi
	movq	%r8, %rsi
	movq	%rbx, %rdx
	movq	%rcx, 8(%rsp)
	movq	%r8, (%rsp)
	call	memcpy
	movq	8(%rsp), %rcx
	movq	(%rsp), %r8
	jmp	.L206
	.p2align 4,,10
	.p2align 3
.L235:
	movzbl	0(%rbp), %eax
	movb	%al, 19(%rbp)
	jmp	.L201
.L231:
	movl	$.LC1, %edi
	call	_ZSt20__throw_length_errorPKc
.LEHE10:
.L220:
	movq	%rax, %rbx
.L215:
	movq	48(%rsp), %rdi
	leaq	64(%rsp), %rax
	cmpq	%rax, %rdi
	je	.L216
	call	_ZdlPv
.L216:
	movq	%rbx, %rdi
.LEHB11:
	call	_Unwind_Resume
.LEHE11:
.L237:
	movl	$.LC3, %edi
.LEHB12:
	call	_ZSt20__throw_length_errorPKc
.LEHE12:
.L221:
	movq	16(%rsp), %rdi
	leaq	32(%rsp), %rdx
	movq	%rax, %rbx
	cmpq	%rdx, %rdi
	je	.L215
	call	_ZdlPv
	jmp	.L215
	.cfi_endproc
.LFE2751:
	.section	.gcc_except_table._ZNSt12future_errorC2ESt10error_code,"aG",@progbits,_ZNSt12future_errorC5ESt10error_code,comdat
.LLSDA2751:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE2751-.LLSDACSB2751
.LLSDACSB2751:
	.uleb128 .LEHB8-.LFB2751
	.uleb128 .LEHE8-.LEHB8
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB9-.LFB2751
	.uleb128 .LEHE9-.LEHB9
	.uleb128 .L221-.LFB2751
	.uleb128 0
	.uleb128 .LEHB10-.LFB2751
	.uleb128 .LEHE10-.LEHB10
	.uleb128 .L220-.LFB2751
	.uleb128 0
	.uleb128 .LEHB11-.LFB2751
	.uleb128 .LEHE11-.LEHB11
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB12-.LFB2751
	.uleb128 .LEHE12-.LEHB12
	.uleb128 .L220-.LFB2751
	.uleb128 0
.LLSDACSE2751:
	.section	.text._ZNSt12future_errorC2ESt10error_code,"axG",@progbits,_ZNSt12future_errorC5ESt10error_code,comdat
	.size	_ZNSt12future_errorC2ESt10error_code, .-_ZNSt12future_errorC2ESt10error_code
	.weak	_ZNSt12future_errorC1ESt10error_code
	.set	_ZNSt12future_errorC1ESt10error_code,_ZNSt12future_errorC2ESt10error_code
	.section	.text._ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEE6_M_runEv,"axG",@progbits,_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEE6_M_runEv,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEE6_M_runEv
	.type	_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEE6_M_runEv, @function
_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEE6_M_runEv:
.LFB4629:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA4629
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	movq	%rdi, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	subq	$168, %rsp
	.cfi_def_cfa_offset 208
	movq	8(%rdi), %rbx
	leaq	80(%rsp), %rcx
	leaq	64(%rsp), %rsi
	leaq	128(%rsp), %rbp
	leaq	96(%rsp), %rdx
	movb	$0, 31(%rsp)
	movq	$_ZNSt13__future_base13_State_baseV29_M_do_setEPSt8functionIFSt10unique_ptrINS_12_Result_baseENS3_8_DeleterEEvEEPb, 80(%rsp)
	leaq	48(%rbx), %rax
	movq	%rbp, 64(%rsp)
	movq	%rbx, 56(%rsp)
	movq	$0, 88(%rsp)
	movq	%rax, 8(%rsp)
	leaq	56(%rbx), %rax
	movq	8(%rsp), %xmm0
	movq	%rax, 8(%rsp)
	leaq	31(%rsp), %rax
	movhps	8(%rsp), %xmm0
	movq	$_ZNSt14_Function_base13_Base_managerINSt13__future_base12_Task_setterISt10unique_ptrINS1_7_ResultIiEENS1_12_Result_base8_DeleterEENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEEE10_M_managerERSt9_Any_dataRKSI_St18_Manager_operation, 8(%rsp)
	movq	%rax, 72(%rsp)
	leaq	56(%rsp), %rax
	movaps	%xmm0, 128(%rsp)
	movq	8(%rsp), %xmm0
	movq	%rcx, 8(%rsp)
	movhps	.LC0(%rip), %xmm0
	movaps	%xmm0, 144(%rsp)
	movq	8(%rsp), %xmm0
	movq	%rax, 8(%rsp)
	movhps	8(%rsp), %xmm0
	leaq	72(%rsp), %rax
	movq	%rsi, 8(%rsp)
	movaps	%xmm0, 96(%rsp)
	movq	8(%rsp), %xmm0
	movq	%rax, 8(%rsp)
	movq	_ZSt15__once_callable@gottpoff(%rip), %rax
	movhps	8(%rsp), %xmm0
	movaps	%xmm0, 112(%rsp)
	movq	%rdx, %fs:(%rax)
	movq	_ZSt11__once_call@gottpoff(%rip), %rax
	movq	$_ZZSt9call_onceIMNSt13__future_base13_State_baseV2EFvPSt8functionIFSt10unique_ptrINS0_12_Result_baseENS4_8_DeleterEEvEEPbEJPS1_S9_SA_EEvRSt9once_flagOT_DpOT0_ENUlvE0_4_FUNEv, %fs:(%rax)
	movl	$_ZL28__gthrw___pthread_key_createPjPFvPvE, %eax
	testq	%rax, %rax
	je	.L260
	leaq	24(%rbx), %rdi
	movl	$__once_proxy, %esi
.LEHB13:
	call	_ZL20__gthrw_pthread_oncePiPFvvE
	testl	%eax, %eax
	jne	.L240
	cmpb	$0, 31(%rsp)
	jne	.L279
	movl	$2, %edi
	call	_ZSt20__throw_future_errori
.LEHE13:
	.p2align 4,,10
	.p2align 3
.L279:
	leaq	16(%rbx), %rdi
	movl	$1, %eax
	xchgl	(%rdi), %eax
	testl	%eax, %eax
	js	.L243
.L246:
	movq	144(%rsp), %rax
	testq	%rax, %rax
	je	.L239
	movl	$3, %edx
	movq	%rbp, %rsi
	movq	%rbp, %rdi
	call	*%rax
.L239:
	addq	$168, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L243:
	.cfi_restore_state
.LEHB14:
	call	_ZNSt28__atomic_futex_unsigned_base19_M_futex_notify_allEPj
.LEHE14:
	jmp	.L246
.L263:
	movq	%rax, %rbx
	movq	144(%rsp), %rax
	movq	%rdx, %r13
	testq	%rax, %rax
	je	.L248
	movl	$3, %edx
	movq	%rbp, %rsi
	movq	%rbp, %rdi
	call	*%rax
.L248:
	subq	$1, %r13
	movq	%rbx, %rdi
	jne	.L278
	call	__cxa_begin_catch
	movq	8(%r12), %rbx
	movq	48(%rbx), %r12
	testq	%r12, %r12
	je	.L251
	movq	$0, 48(%rbx)
	call	_ZSt15future_categoryv
	movl	$4, %esi
	movq	%rax, %rdx
	movq	%rbp, %rdi
.LEHB15:
	call	_ZNSt12future_errorC1ESt10error_code
.LEHE15:
	movl	$32, %edi
	call	__cxa_allocate_exception
	movl	$_ZNSt15__exception_ptr12__dest_thunkISt12future_errorEEvPv, %edx
	movq	%rax, %r13
	movl	$_ZTISt12future_error, %esi
	movq	%rax, %rdi
	call	__cxa_init_primary_exception
	testq	%r13, %r13
	je	.L252
	movq	%rbp, %rsi
	movq	%r13, %rdi
	call	_ZNSt11logic_errorC2ERKS_
	movdqa	144(%rsp), %xmm0
	movq	$_ZTVSt12future_error+16, 0(%r13)
	movups	%xmm0, 16(%r13)
.L252:
	leaq	32(%rsp), %rdi
	movq	%r13, %rsi
	call	_ZNSt15__exception_ptr13exception_ptrC1EPv
	movq	32(%rsp), %rax
	leaq	8(%r12), %rsi
	leaq	48(%rsp), %rdi
	movq	$0, 32(%rsp)
	movq	%rax, 48(%rsp)
	call	_ZNSt15__exception_ptr13exception_ptr4swapERS0_
	leaq	48(%rsp), %rdi
	call	_ZNSt15__exception_ptr13exception_ptrD1Ev
	leaq	32(%rsp), %rdi
	call	_ZNSt15__exception_ptr13exception_ptrD1Ev
	movq	%rbp, %rdi
	call	_ZNSt12future_errorD1Ev
	movq	8(%rbx), %rbp
	leaq	16(%rbx), %rdi
	movl	$1, %eax
	movq	%r12, 8(%rbx)
	xchgl	(%rdi), %eax
	testl	%eax, %eax
	js	.L253
.L255:
	testq	%rbp, %rbp
	je	.L251
	movq	0(%rbp), %rax
	movq	%rbp, %rdi
	call	*(%rax)
.L251:
.LEHB16:
	call	__cxa_rethrow
.LEHE16:
.L260:
	orl	$-1, %eax
.L240:
	movl	%eax, %edi
.LEHB17:
	call	_ZSt20__throw_system_errori
.LEHE17:
.L253:
.LEHB18:
	call	_ZNSt28__atomic_futex_unsigned_base19_M_futex_notify_allEPj
.LEHE18:
	jmp	.L255
.L261:
	movq	%rax, %rbx
.L259:
	call	__cxa_end_catch
	movq	%rbx, %rdi
.L278:
.LEHB19:
	call	_Unwind_Resume
.LEHE19:
.L264:
	movq	%rax, %rbx
.L258:
	movq	(%r12), %rax
	movq	%r12, %rdi
	call	*(%rax)
	jmp	.L259
.L262:
	testq	%rbp, %rbp
	movq	%rax, %rbx
	movq	%rbp, %r12
	jne	.L258
	jmp	.L259
	.cfi_endproc
.LFE4629:
	.section	.gcc_except_table._ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEE6_M_runEv,"aG",@progbits,_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEE6_M_runEv,comdat
	.align 4
.LLSDA4629:
	.byte	0xff
	.byte	0x3
	.uleb128 .LLSDATT4629-.LLSDATTD4629
.LLSDATTD4629:
	.byte	0x1
	.uleb128 .LLSDACSE4629-.LLSDACSB4629
.LLSDACSB4629:
	.uleb128 .LEHB13-.LFB4629
	.uleb128 .LEHE13-.LEHB13
	.uleb128 .L263-.LFB4629
	.uleb128 0x3
	.uleb128 .LEHB14-.LFB4629
	.uleb128 .LEHE14-.LEHB14
	.uleb128 .L263-.LFB4629
	.uleb128 0x3
	.uleb128 .LEHB15-.LFB4629
	.uleb128 .LEHE15-.LEHB15
	.uleb128 .L264-.LFB4629
	.uleb128 0
	.uleb128 .LEHB16-.LFB4629
	.uleb128 .LEHE16-.LEHB16
	.uleb128 .L261-.LFB4629
	.uleb128 0
	.uleb128 .LEHB17-.LFB4629
	.uleb128 .LEHE17-.LEHB17
	.uleb128 .L263-.LFB4629
	.uleb128 0x3
	.uleb128 .LEHB18-.LFB4629
	.uleb128 .LEHE18-.LEHB18
	.uleb128 .L262-.LFB4629
	.uleb128 0
	.uleb128 .LEHB19-.LFB4629
	.uleb128 .LEHE19-.LEHB19
	.uleb128 0
	.uleb128 0
.LLSDACSE4629:
	.byte	0x1
	.byte	0
	.byte	0
	.byte	0x7d
	.align 4
	.long	_ZTIN10__cxxabiv115__forced_unwindE
.LLSDATT4629:
	.section	.text._ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEE6_M_runEv,"axG",@progbits,_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEE6_M_runEv,comdat
	.size	_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEE6_M_runEv, .-_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEE6_M_runEv
	.section	.text._ZZSt9call_onceIMNSt13__future_base13_State_baseV2EFvPSt8functionIFSt10unique_ptrINS0_12_Result_baseENS4_8_DeleterEEvEEPbEJPS1_S9_SA_EEvRSt9once_flagOT_DpOT0_ENKUlvE0_clEv,"axG",@progbits,_ZZSt9call_onceIMNSt13__future_base13_State_baseV2EFvPSt8functionIFSt10unique_ptrINS0_12_Result_baseENS4_8_DeleterEEvEEPbEJPS1_S9_SA_EEvRSt9once_flagOT_DpOT0_ENKUlvE0_clEv,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZZSt9call_onceIMNSt13__future_base13_State_baseV2EFvPSt8functionIFSt10unique_ptrINS0_12_Result_baseENS4_8_DeleterEEvEEPbEJPS1_S9_SA_EEvRSt9once_flagOT_DpOT0_ENKUlvE0_clEv
	.type	_ZZSt9call_onceIMNSt13__future_base13_State_baseV2EFvPSt8functionIFSt10unique_ptrINS0_12_Result_baseENS4_8_DeleterEEvEEPbEJPS1_S9_SA_EEvRSt9once_flagOT_DpOT0_ENKUlvE0_clEv, @function
_ZZSt9call_onceIMNSt13__future_base13_State_baseV2EFvPSt8functionIFSt10unique_ptrINS0_12_Result_baseENS4_8_DeleterEEvEEPbEJPS1_S9_SA_EEvRSt9once_flagOT_DpOT0_ENKUlvE0_clEv:
.LFB3340:
	.cfi_startproc
	movq	_ZSt15__once_callable@gottpoff(%rip), %rax
	movq	%fs:(%rax), %rax
	movq	16(%rax), %rdx
	movq	(%rax), %rcx
	movq	(%rdx), %rsi
	movq	24(%rax), %rdx
	movq	8(%rax), %rax
	movq	8(%rcx), %rdi
	movq	(%rdx), %rdx
	addq	(%rax), %rdi
	movq	(%rcx), %rax
	testb	$1, %al
	je	.L281
	movq	(%rdi), %rcx
	movq	-1(%rcx,%rax), %rax
.L281:
	jmp	*%rax
	.cfi_endproc
.LFE3340:
	.size	_ZZSt9call_onceIMNSt13__future_base13_State_baseV2EFvPSt8functionIFSt10unique_ptrINS0_12_Result_baseENS4_8_DeleterEEvEEPbEJPS1_S9_SA_EEvRSt9once_flagOT_DpOT0_ENKUlvE0_clEv, .-_ZZSt9call_onceIMNSt13__future_base13_State_baseV2EFvPSt8functionIFSt10unique_ptrINS0_12_Result_baseENS4_8_DeleterEEvEEPbEJPS1_S9_SA_EEvRSt9once_flagOT_DpOT0_ENKUlvE0_clEv
	.section	.text._ZZSt9call_onceIMSt6threadFvvEJPS0_EEvRSt9once_flagOT_DpOT0_ENKUlvE0_clEv,"axG",@progbits,_ZZSt9call_onceIMSt6threadFvvEJPS0_EEvRSt9once_flagOT_DpOT0_ENKUlvE0_clEv,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZZSt9call_onceIMSt6threadFvvEJPS0_EEvRSt9once_flagOT_DpOT0_ENKUlvE0_clEv
	.type	_ZZSt9call_onceIMSt6threadFvvEJPS0_EEvRSt9once_flagOT_DpOT0_ENKUlvE0_clEv, @function
_ZZSt9call_onceIMSt6threadFvvEJPS0_EEvRSt9once_flagOT_DpOT0_ENKUlvE0_clEv:
.LFB3459:
	.cfi_startproc
	movq	_ZSt15__once_callable@gottpoff(%rip), %rax
	movq	%fs:(%rax), %rdx
	movq	(%rdx), %rax
	movq	8(%rdx), %rdx
	movq	8(%rax), %rdi
	movq	(%rax), %rax
	addq	(%rdx), %rdi
	testb	$1, %al
	je	.L286
	movq	(%rdi), %rdx
	movq	-1(%rdx,%rax), %rax
.L286:
	jmp	*%rax
	.cfi_endproc
.LFE3459:
	.size	_ZZSt9call_onceIMSt6threadFvvEJPS0_EEvRSt9once_flagOT_DpOT0_ENKUlvE0_clEv, .-_ZZSt9call_onceIMSt6threadFvvEJPS0_EEvRSt9once_flagOT_DpOT0_ENKUlvE0_clEv
	.section	.text._ZNSt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE10_M_releaseEv,"axG",@progbits,_ZNSt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE10_M_releaseEv,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE10_M_releaseEv
	.type	_ZNSt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE10_M_releaseEv, @function
_ZNSt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE10_M_releaseEv:
.LFB3555:
	.cfi_startproc
	movl	$_ZL28__gthrw___pthread_key_createPjPFvPvE, %eax
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	testq	%rax, %rax
	je	.L291
	lock subl	$1, 8(%rdi)
	je	.L298
.L290:
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L291:
	.cfi_restore_state
	movl	8(%rdi), %eax
	leal	-1(%rax), %edx
	cmpl	$1, %eax
	movl	%edx, 8(%rdi)
	jne	.L290
	movq	(%rdi), %rax
	call	*16(%rax)
	movl	12(%rbx), %eax
	leal	-1(%rax), %edx
	movl	%edx, 12(%rbx)
	jmp	.L296
	.p2align 4,,10
	.p2align 3
.L298:
	movq	(%rdi), %rax
	call	*16(%rax)
	movl	$-1, %eax
	lock xaddl	%eax, 12(%rbx)
.L296:
	cmpl	$1, %eax
	jne	.L290
	movq	(%rbx), %rax
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 8
	movq	24(%rax), %rax
	jmp	*%rax
	.cfi_endproc
.LFE3555:
	.size	_ZNSt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE10_M_releaseEv, .-_ZNSt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE10_M_releaseEv
	.section	.text._ZSt5asyncIRFivEJEESt6futureINSt9result_ofIFNSt5decayIT_E4typeEDpNS4_IT0_E4typeEEE4typeEESt6launchOS5_DpOS8_,"axG",@progbits,_ZSt5asyncIRFivEJEESt6futureINSt9result_ofIFNSt5decayIT_E4typeEDpNS4_IT0_E4typeEEE4typeEESt6launchOS5_DpOS8_,comdat
	.p2align 4,,15
	.weak	_ZSt5asyncIRFivEJEESt6futureINSt9result_ofIFNSt5decayIT_E4typeEDpNS4_IT0_E4typeEEE4typeEESt6launchOS5_DpOS8_
	.type	_ZSt5asyncIRFivEJEESt6futureINSt9result_ofIFNSt5decayIT_E4typeEDpNS4_IT0_E4typeEEE4typeEESt6launchOS5_DpOS8_, @function
_ZSt5asyncIRFivEJEESt6futureINSt9result_ofIFNSt5decayIT_E4typeEDpNS4_IT0_E4typeEEE4typeEESt6launchOS5_DpOS8_:
.LFB3463:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA3463
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	movq	%rdx, %r14
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	movq	%rdi, %rbp
	subq	$56, %rsp
	.cfi_def_cfa_offset 112
	testb	$1, %sil
	jne	.L382
.L300:
	xorl	%r13d, %r13d
.L337:
	movl	$64, %edi
.LEHB20:
	call	_Znwm
.LEHE20:
	movq	%rax, %rbx
	movabsq	$4294967297, %rax
	movl	$24, %edi
	movq	%rax, 8(%rbx)
	movq	$_ZTVSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE+16, (%rbx)
	movq	$0, 24(%rbx)
	movl	$0, 32(%rbx)
	movb	$0, 36(%rbx)
	movl	$0, 40(%rbx)
	movq	$_ZTVNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE+16, 16(%rbx)
.LEHB21:
	call	_Znwm
.LEHE21:
	movq	%rax, %rdi
	movq	%rax, %r12
	call	_ZNSt13__future_base12_Result_baseC2Ev
	movq	(%rbx), %rax
	movq	$_ZTVNSt13__future_base7_ResultIiEE+16, (%r12)
	movl	$_ZTISt19_Sp_make_shared_tag, %esi
	movb	$0, 20(%r12)
	movq	%r12, 48(%rbx)
	movq	%rbx, %rdi
	movq	%r14, 56(%rbx)
	movl	$_ZL28__gthrw___pthread_key_createPjPFvPvE, %r12d
	call	*32(%rax)
	testq	%r13, %r13
	je	.L311
	testq	%r12, %r12
	je	.L317
	lock subl	$1, 8(%r13)
	je	.L383
.L311:
	movq	%rax, 0(%rbp)
	movq	%rbx, 8(%rbp)
.L343:
	testq	%r12, %r12
	jne	.L384
	addl	$1, 8(%rbx)
.L322:
	testq	%rax, %rax
	je	.L385
	movl	$1, %edx
	xchgb	20(%rax), %dl
	testb	%dl, %dl
	jne	.L386
	testq	%r12, %r12
	je	.L327
	lock subl	$1, 8(%rbx)
	je	.L387
.L299:
	addq	$56, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	movq	%rbp, %rax
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L384:
	.cfi_restore_state
	lock addl	$1, 8(%rbx)
	movq	0(%rbp), %rax
	jmp	.L322
	.p2align 4,,10
	.p2align 3
.L382:
	movl	$80, %edi
	movl	%esi, %r12d
.LEHB22:
	call	_Znwm
.LEHE22:
	movq	%rax, %rbx
	movabsq	$4294967297, %rax
	movl	$24, %edi
	movq	%rax, 8(%rbx)
	movq	$_ZTVSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE+16, (%rbx)
	leaq	16(%rbx), %r15
	movq	$0, 24(%rbx)
	movl	$0, 32(%rbx)
	movb	$0, 36(%rbx)
	movl	$0, 40(%rbx)
	movq	$0, 48(%rbx)
	movl	$0, 56(%rbx)
	movq	$_ZTVNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE+16, 16(%rbx)
.LEHB23:
	call	_Znwm
.LEHE23:
	movq	%rax, %r13
	movq	%rax, %rdi
	call	_ZNSt13__future_base12_Result_baseC2Ev
	movq	$_ZTVNSt13__future_base7_ResultIiEE+16, 0(%r13)
	movb	$0, 20(%r13)
	movl	$16, %edi
	movq	%r13, 64(%rbx)
	movq	%r14, 72(%rbx)
	movq	$0, 16(%rsp)
.LEHB24:
	call	_Znwm
.LEHE24:
	leaq	32(%rsp), %rsi
	leaq	16(%rsp), %rdi
	movq	$_ZTVNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEEE+16, (%rax)
	movq	%r15, 8(%rax)
	movl	$pthread_create, %edx
	movq	%rax, 32(%rsp)
.LEHB25:
	call	_ZNSt6thread15_M_start_threadESt10unique_ptrINS_6_StateESt14default_deleteIS1_EEPFvvE
.LEHE25:
	movq	32(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L301
	movq	(%rdi), %rax
	call	*8(%rax)
.L301:
	cmpq	$0, 48(%rbx)
	jne	.L388
	movq	16(%rsp), %rax
	movl	$_ZTISt19_Sp_make_shared_tag, %esi
	movq	%rbx, %rdi
	movq	%rax, 48(%rbx)
	movq	(%rbx), %rax
	call	*32(%rax)
	testq	%rax, %rax
	je	.L389
	movq	%rax, 0(%rbp)
	movq	%rbx, 8(%rbp)
	movl	$_ZL28__gthrw___pthread_key_createPjPFvPvE, %r12d
	jmp	.L343
	.p2align 4,,10
	.p2align 3
.L327:
	movl	8(%rbx), %eax
	leal	-1(%rax), %edx
	cmpl	$1, %eax
	movl	%edx, 8(%rbx)
	jne	.L299
	movq	(%rbx), %rax
	movq	%rbx, %rdi
	call	*16(%rax)
	movl	12(%rbx), %eax
	leal	-1(%rax), %edx
	movl	%edx, 12(%rbx)
.L342:
	cmpl	$1, %eax
	jne	.L299
	movq	(%rbx), %rax
	movq	%rbx, %rdi
	call	*24(%rax)
	jmp	.L299
	.p2align 4,,10
	.p2align 3
.L317:
	movl	8(%r13), %edx
	leal	-1(%rdx), %ecx
	cmpl	$1, %edx
	movl	%ecx, 8(%r13)
	jne	.L311
	movq	0(%r13), %rdx
	movq	%rax, 8(%rsp)
	movq	%r13, %rdi
	call	*16(%rdx)
	movl	12(%r13), %edx
	movq	8(%rsp), %rax
	leal	-1(%rdx), %ecx
	movl	%ecx, 12(%r13)
	jmp	.L341
	.p2align 4,,10
	.p2align 3
.L387:
	movq	(%rbx), %rax
	movq	%rbx, %rdi
	call	*16(%rax)
	movl	$-1, %eax
	lock xaddl	%eax, 12(%rbx)
	jmp	.L342
.L388:
	call	_ZSt9terminatev
	.p2align 4,,10
	.p2align 3
.L383:
	movq	0(%r13), %rdx
	movq	%rax, 8(%rsp)
	movq	%r13, %rdi
	call	*16(%rdx)
	movl	$-1, %edx
	lock xaddl	%edx, 12(%r13)
	movq	8(%rsp), %rax
.L341:
	cmpl	$1, %edx
	jne	.L311
	movq	0(%r13), %rdx
	movq	%rax, 8(%rsp)
	movq	%r13, %rdi
	call	*24(%rdx)
	movq	8(%rsp), %rax
	jmp	.L311
.L386:
	movl	$1, %edi
.LEHB26:
	call	_ZSt20__throw_future_errori
.LEHE26:
.L354:
	movq	8(%rbp), %rdi
	movq	%rax, %r12
	testq	%rdi, %rdi
	je	.L325
	call	_ZNSt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE10_M_releaseEv
.L325:
	movq	%r12, %rbp
	movq	%rbx, %r13
.L315:
	movq	%r13, %rdi
	call	_ZNSt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE10_M_releaseEv
.L316:
	movq	%rbp, %rdi
.LEHB27:
	call	_Unwind_Resume
.LEHE27:
.L385:
	movl	$3, %edi
.LEHB28:
	call	_ZSt20__throw_future_errori
.LEHE28:
.L349:
	movq	%rax, %r13
.L306:
	movq	%r15, %rdi
	movq	%rdx, 8(%rsp)
	call	_ZNSt13__future_base21_Async_state_commonV2D2Ev
	movq	%rbx, %rdi
	call	_ZdlPv
	movq	8(%rsp), %rdx
	movq	%r13, %rax
.L307:
	subq	$1, %rdx
	jne	.L390
	movq	%rax, %rdi
	call	__cxa_begin_catch
	movq	%rax, %rbx
	call	_ZNSt3_V216generic_categoryEv
	movl	$11, 32(%rsp)
	movq	%rax, 40(%rsp)
	leaq	32(%rsp), %rdx
	movq	24(%rbx), %rdi
	movl	16(%rbx), %esi
	leaq	16(%rbx), %r13
	movq	(%rdi), %rax
	call	*48(%rax)
	testb	%al, %al
	je	.L332
.L335:
	andb	$2, %r12b
	je	.L333
	call	__cxa_end_catch
	jmp	.L300
.L350:
	movq	%rax, %r13
.L304:
	movq	64(%rbx), %rdi
	testq	%rdi, %rdi
	je	.L306
	movq	(%rdi), %rax
	movq	%rdx, 8(%rsp)
	call	*(%rax)
	movq	8(%rsp), %rdx
	jmp	.L306
.L348:
	jmp	.L307
.L332:
	movq	40(%rsp), %rdi
	movl	32(%rsp), %edx
	movq	%r13, %rsi
	movq	(%rdi), %rax
	call	*56(%rax)
	testb	%al, %al
	jne	.L335
.L333:
.LEHB29:
	call	__cxa_rethrow
.LEHE29:
.L390:
	movq	%rax, %rbp
	jmp	.L316
.L347:
	movq	%rax, %rbp
	call	__cxa_end_catch
	jmp	.L316
.L351:
	movq	32(%rsp), %rdi
	movq	%rax, %r13
	testq	%rdi, %rdi
	je	.L304
	movq	(%rdi), %rax
	movq	%rdx, 8(%rsp)
	call	*8(%rax)
	movq	8(%rsp), %rdx
	jmp	.L304
.L353:
	movq	24(%rbx), %rdi
	movq	%rax, %rbp
	movq	$_ZTVNSt13__future_base13_State_baseV2E+16, 16(%rbx)
	testq	%rdi, %rdi
	je	.L313
	movq	(%rdi), %rax
	call	*(%rax)
.L313:
	movq	%rbx, %rdi
	call	_ZdlPv
.L314:
	testq	%r13, %r13
	jne	.L315
	jmp	.L316
.L352:
	movq	%rax, %rbp
	jmp	.L314
.L389:
	movq	%rbx, %r13
	jmp	.L337
	.cfi_endproc
.LFE3463:
	.section	.gcc_except_table._ZSt5asyncIRFivEJEESt6futureINSt9result_ofIFNSt5decayIT_E4typeEDpNS4_IT0_E4typeEEE4typeEESt6launchOS5_DpOS8_,"aG",@progbits,_ZSt5asyncIRFivEJEESt6futureINSt9result_ofIFNSt5decayIT_E4typeEDpNS4_IT0_E4typeEEE4typeEESt6launchOS5_DpOS8_,comdat
	.align 4
.LLSDA3463:
	.byte	0xff
	.byte	0x3
	.uleb128 .LLSDATT3463-.LLSDATTD3463
.LLSDATTD3463:
	.byte	0x1
	.uleb128 .LLSDACSE3463-.LLSDACSB3463
.LLSDACSB3463:
	.uleb128 .LEHB20-.LFB3463
	.uleb128 .LEHE20-.LEHB20
	.uleb128 .L352-.LFB3463
	.uleb128 0
	.uleb128 .LEHB21-.LFB3463
	.uleb128 .LEHE21-.LEHB21
	.uleb128 .L353-.LFB3463
	.uleb128 0
	.uleb128 .LEHB22-.LFB3463
	.uleb128 .LEHE22-.LEHB22
	.uleb128 .L348-.LFB3463
	.uleb128 0x3
	.uleb128 .LEHB23-.LFB3463
	.uleb128 .LEHE23-.LEHB23
	.uleb128 .L349-.LFB3463
	.uleb128 0x3
	.uleb128 .LEHB24-.LFB3463
	.uleb128 .LEHE24-.LEHB24
	.uleb128 .L350-.LFB3463
	.uleb128 0x3
	.uleb128 .LEHB25-.LFB3463
	.uleb128 .LEHE25-.LEHB25
	.uleb128 .L351-.LFB3463
	.uleb128 0x3
	.uleb128 .LEHB26-.LFB3463
	.uleb128 .LEHE26-.LEHB26
	.uleb128 .L354-.LFB3463
	.uleb128 0
	.uleb128 .LEHB27-.LFB3463
	.uleb128 .LEHE27-.LEHB27
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB28-.LFB3463
	.uleb128 .LEHE28-.LEHB28
	.uleb128 .L354-.LFB3463
	.uleb128 0
	.uleb128 .LEHB29-.LFB3463
	.uleb128 .LEHE29-.LEHB29
	.uleb128 .L347-.LFB3463
	.uleb128 0
.LLSDACSE3463:
	.byte	0
	.byte	0
	.byte	0x1
	.byte	0x7d
	.align 4
	.long	_ZTISt12system_error
.LLSDATT3463:
	.section	.text._ZSt5asyncIRFivEJEESt6futureINSt9result_ofIFNSt5decayIT_E4typeEDpNS4_IT0_E4typeEEE4typeEESt6launchOS5_DpOS8_,"axG",@progbits,_ZSt5asyncIRFivEJEESt6futureINSt9result_ofIFNSt5decayIT_E4typeEDpNS4_IT0_E4typeEEE4typeEESt6launchOS5_DpOS8_,comdat
	.size	_ZSt5asyncIRFivEJEESt6futureINSt9result_ofIFNSt5decayIT_E4typeEDpNS4_IT0_E4typeEEE4typeEESt6launchOS5_DpOS8_, .-_ZSt5asyncIRFivEJEESt6futureINSt9result_ofIFNSt5decayIT_E4typeEDpNS4_IT0_E4typeEEE4typeEESt6launchOS5_DpOS8_
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB2998:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA2998
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	movl	$_Z9get_valuev, %edx
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movl	$1, %esi
	subq	$32, %rsp
	.cfi_def_cfa_offset 64
	leaq	16(%rsp), %rdi
.LEHB30:
	call	_ZSt5asyncIRFivEJEESt6futureINSt9result_ofIFNSt5decayIT_E4typeEDpNS4_IT0_E4typeEEE4typeEESt6launchOS5_DpOS8_
.LEHE30:
	movq	16(%rsp), %rbp
	testq	%rbp, %rbp
	je	.L422
	movq	0(%rbp), %rax
	movq	%rbp, %rdi
.LEHB31:
	call	*16(%rax)
	movl	16(%rbp), %edx
	leaq	16(%rbp), %rbx
	andl	$2147483647, %edx
	cmpl	$1, %edx
	je	.L393
	.p2align 4,,10
	.p2align 3
.L394:
	lock orl	$-2147483648, (%rbx)
	xorl	%r9d, %r9d
	xorl	%r8d, %r8d
	xorl	%ecx, %ecx
	orl	$-2147483648, %edx
	movq	%rbx, %rsi
	movq	%rbx, %rdi
	call	_ZNSt28__atomic_futex_unsigned_base19_M_futex_wait_untilEPjjbNSt6chrono8durationIlSt5ratioILl1ELl1EEEENS2_IlS3_ILl1ELl1000000000EEEE
	movl	(%rbx), %edx
	xorl	$1, %eax
	andl	$2147483647, %edx
	cmpl	$1, %edx
	sete	%cl
	orb	%cl, %al
	je	.L394
.L393:
	movq	8(%rbp), %rbx
	movq	%rsp, %rsi
	movq	$0, (%rsp)
	leaq	8(%rbx), %rbp
	movq	%rbp, %rdi
	call	_ZNSt15__exception_ptreqERKNS_13exception_ptrES2_
	movq	%rsp, %rdi
	movl	%eax, %r12d
	call	_ZNSt15__exception_ptr13exception_ptrD1Ev
	testb	%r12b, %r12b
	je	.L423
	movl	16(%rbx), %ebp
	movq	24(%rsp), %rbx
	pxor	%xmm0, %xmm0
	testq	%rbx, %rbx
	movaps	%xmm0, 16(%rsp)
	je	.L391
	movl	$_ZL28__gthrw___pthread_key_createPjPFvPvE, %eax
	testq	%rax, %rax
	je	.L400
	lock subl	$1, 8(%rbx)
	je	.L424
.L402:
	movq	24(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L391
	call	_ZNSt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE10_M_releaseEv
.L391:
	addq	$32, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 32
	movl	%ebp, %eax
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
.L400:
	.cfi_restore_state
	movl	8(%rbx), %eax
	leal	-1(%rax), %edx
	subl	$1, %eax
	movl	%edx, 8(%rbx)
	jne	.L402
	movq	(%rbx), %rax
	movq	%rbx, %rdi
	call	*16(%rax)
	movl	12(%rbx), %eax
	leal	-1(%rax), %edx
	movl	%edx, 12(%rbx)
.L410:
	subl	$1, %eax
	jne	.L402
	movq	(%rbx), %rax
	movq	%rbx, %rdi
	call	*24(%rax)
	jmp	.L402
.L424:
	movq	(%rbx), %rax
	movq	%rbx, %rdi
	call	*16(%rax)
	orl	$-1, %eax
	lock xaddl	%eax, 12(%rbx)
	jmp	.L410
.L422:
	movl	$3, %edi
	call	_ZSt20__throw_future_errori
.LEHE31:
.L411:
	movq	%rax, %rbx
.L399:
	movq	24(%rsp), %rdi
	pxor	%xmm0, %xmm0
	testq	%rdi, %rdi
	movaps	%xmm0, 16(%rsp)
	je	.L407
	call	_ZNSt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE10_M_releaseEv
.L407:
	movq	24(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L409
	call	_ZNSt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE10_M_releaseEv
.L409:
	movq	%rbx, %rdi
.LEHB32:
	call	_Unwind_Resume
.LEHE32:
.L423:
	movq	%rsp, %rdi
	movq	%rbp, %rsi
	call	_ZNSt15__exception_ptr13exception_ptrC1ERKS0_
	movq	%rsp, %rdi
.LEHB33:
	call	_ZSt17rethrow_exceptionNSt15__exception_ptr13exception_ptrE
.LEHE33:
.L412:
	movq	%rsp, %rdi
	movq	%rax, %rbx
	call	_ZNSt15__exception_ptr13exception_ptrD1Ev
	jmp	.L399
	.cfi_endproc
.LFE2998:
	.section	.gcc_except_table,"a",@progbits
.LLSDA2998:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE2998-.LLSDACSB2998
.LLSDACSB2998:
	.uleb128 .LEHB30-.LFB2998
	.uleb128 .LEHE30-.LEHB30
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB31-.LFB2998
	.uleb128 .LEHE31-.LEHB31
	.uleb128 .L411-.LFB2998
	.uleb128 0
	.uleb128 .LEHB32-.LFB2998
	.uleb128 .LEHE32-.LEHB32
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB33-.LFB2998
	.uleb128 .LEHE33-.LEHB33
	.uleb128 .L412-.LFB2998
	.uleb128 0
.LLSDACSE2998:
	.section	.text.startup
	.size	main, .-main
	.weak	_ZTSSt11_Mutex_baseILN9__gnu_cxx12_Lock_policyE2EE
	.section	.rodata._ZTSSt11_Mutex_baseILN9__gnu_cxx12_Lock_policyE2EE,"aG",@progbits,_ZTSSt11_Mutex_baseILN9__gnu_cxx12_Lock_policyE2EE,comdat
	.align 32
	.type	_ZTSSt11_Mutex_baseILN9__gnu_cxx12_Lock_policyE2EE, @object
	.size	_ZTSSt11_Mutex_baseILN9__gnu_cxx12_Lock_policyE2EE, 47
_ZTSSt11_Mutex_baseILN9__gnu_cxx12_Lock_policyE2EE:
	.string	"St11_Mutex_baseILN9__gnu_cxx12_Lock_policyE2EE"
	.weak	_ZTISt11_Mutex_baseILN9__gnu_cxx12_Lock_policyE2EE
	.section	.rodata._ZTISt11_Mutex_baseILN9__gnu_cxx12_Lock_policyE2EE,"aG",@progbits,_ZTISt11_Mutex_baseILN9__gnu_cxx12_Lock_policyE2EE,comdat
	.align 8
	.type	_ZTISt11_Mutex_baseILN9__gnu_cxx12_Lock_policyE2EE, @object
	.size	_ZTISt11_Mutex_baseILN9__gnu_cxx12_Lock_policyE2EE, 16
_ZTISt11_Mutex_baseILN9__gnu_cxx12_Lock_policyE2EE:
	.quad	_ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	_ZTSSt11_Mutex_baseILN9__gnu_cxx12_Lock_policyE2EE
	.weak	_ZTSSt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE
	.section	.rodata._ZTSSt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE,"aG",@progbits,_ZTSSt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE,comdat
	.align 32
	.type	_ZTSSt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE, @object
	.size	_ZTSSt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE, 52
_ZTSSt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE:
	.string	"St16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE"
	.weak	_ZTISt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE
	.section	.rodata._ZTISt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE,"aG",@progbits,_ZTISt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE,comdat
	.align 8
	.type	_ZTISt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE, @object
	.size	_ZTISt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE, 24
_ZTISt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE:
	.quad	_ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	_ZTSSt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE
	.quad	_ZTISt11_Mutex_baseILN9__gnu_cxx12_Lock_policyE2EE
	.weak	_ZTSSt19_Sp_make_shared_tag
	.section	.rodata._ZTSSt19_Sp_make_shared_tag,"aG",@progbits,_ZTSSt19_Sp_make_shared_tag,comdat
	.align 16
	.type	_ZTSSt19_Sp_make_shared_tag, @object
	.size	_ZTSSt19_Sp_make_shared_tag, 24
_ZTSSt19_Sp_make_shared_tag:
	.string	"St19_Sp_make_shared_tag"
	.weak	_ZTISt19_Sp_make_shared_tag
	.section	.rodata._ZTISt19_Sp_make_shared_tag,"aG",@progbits,_ZTISt19_Sp_make_shared_tag,comdat
	.align 8
	.type	_ZTISt19_Sp_make_shared_tag, @object
	.size	_ZTISt19_Sp_make_shared_tag, 16
_ZTISt19_Sp_make_shared_tag:
	.quad	_ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	_ZTSSt19_Sp_make_shared_tag
	.weak	_ZTSNSt13__future_base13_State_baseV2E
	.section	.rodata._ZTSNSt13__future_base13_State_baseV2E,"aG",@progbits,_ZTSNSt13__future_base13_State_baseV2E,comdat
	.align 32
	.type	_ZTSNSt13__future_base13_State_baseV2E, @object
	.size	_ZTSNSt13__future_base13_State_baseV2E, 35
_ZTSNSt13__future_base13_State_baseV2E:
	.string	"NSt13__future_base13_State_baseV2E"
	.weak	_ZTINSt13__future_base13_State_baseV2E
	.section	.rodata._ZTINSt13__future_base13_State_baseV2E,"aG",@progbits,_ZTINSt13__future_base13_State_baseV2E,comdat
	.align 8
	.type	_ZTINSt13__future_base13_State_baseV2E, @object
	.size	_ZTINSt13__future_base13_State_baseV2E, 16
_ZTINSt13__future_base13_State_baseV2E:
	.quad	_ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	_ZTSNSt13__future_base13_State_baseV2E
	.weak	_ZTSNSt13__future_base21_Async_state_commonV2E
	.section	.rodata._ZTSNSt13__future_base21_Async_state_commonV2E,"aG",@progbits,_ZTSNSt13__future_base21_Async_state_commonV2E,comdat
	.align 32
	.type	_ZTSNSt13__future_base21_Async_state_commonV2E, @object
	.size	_ZTSNSt13__future_base21_Async_state_commonV2E, 43
_ZTSNSt13__future_base21_Async_state_commonV2E:
	.string	"NSt13__future_base21_Async_state_commonV2E"
	.weak	_ZTINSt13__future_base21_Async_state_commonV2E
	.section	.rodata._ZTINSt13__future_base21_Async_state_commonV2E,"aG",@progbits,_ZTINSt13__future_base21_Async_state_commonV2E,comdat
	.align 8
	.type	_ZTINSt13__future_base21_Async_state_commonV2E, @object
	.size	_ZTINSt13__future_base21_Async_state_commonV2E, 24
_ZTINSt13__future_base21_Async_state_commonV2E:
	.quad	_ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	_ZTSNSt13__future_base21_Async_state_commonV2E
	.quad	_ZTINSt13__future_base13_State_baseV2E
	.weak	_ZTSNSt13__future_base7_ResultIiEE
	.section	.rodata._ZTSNSt13__future_base7_ResultIiEE,"aG",@progbits,_ZTSNSt13__future_base7_ResultIiEE,comdat
	.align 16
	.type	_ZTSNSt13__future_base7_ResultIiEE, @object
	.size	_ZTSNSt13__future_base7_ResultIiEE, 31
_ZTSNSt13__future_base7_ResultIiEE:
	.string	"NSt13__future_base7_ResultIiEE"
	.weak	_ZTINSt13__future_base7_ResultIiEE
	.section	.rodata._ZTINSt13__future_base7_ResultIiEE,"aG",@progbits,_ZTINSt13__future_base7_ResultIiEE,comdat
	.align 8
	.type	_ZTINSt13__future_base7_ResultIiEE, @object
	.size	_ZTINSt13__future_base7_ResultIiEE, 24
_ZTINSt13__future_base7_ResultIiEE:
	.quad	_ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	_ZTSNSt13__future_base7_ResultIiEE
	.quad	_ZTINSt13__future_base12_Result_baseE
	.weak	_ZTSNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE
	.section	.rodata._ZTSNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE,"aG",@progbits,_ZTSNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE,comdat
	.align 32
	.type	_ZTSNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE, @object
	.size	_ZTSNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE, 81
_ZTSNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE:
	.string	"NSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE"
	.weak	_ZTINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE
	.section	.rodata._ZTINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE,"aG",@progbits,_ZTINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE,comdat
	.align 8
	.type	_ZTINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE, @object
	.size	_ZTINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE, 24
_ZTINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE:
	.quad	_ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	_ZTSNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE
	.quad	_ZTINSt13__future_base21_Async_state_commonV2E
	.weak	_ZTSNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE
	.section	.rodata._ZTSNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE,"aG",@progbits,_ZTSNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE,comdat
	.align 32
	.type	_ZTSNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE, @object
	.size	_ZTSNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE, 79
_ZTSNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE:
	.string	"NSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE"
	.weak	_ZTINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE
	.section	.rodata._ZTINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE,"aG",@progbits,_ZTINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE,comdat
	.align 8
	.type	_ZTINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE, @object
	.size	_ZTINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE, 24
_ZTINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE:
	.quad	_ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	_ZTSNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE
	.quad	_ZTINSt13__future_base13_State_baseV2E
	.weak	_ZTSSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE
	.section	.rodata._ZTSSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE,"aG",@progbits,_ZTSSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE,comdat
	.align 32
	.type	_ZTSSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE, @object
	.size	_ZTSSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE, 146
_ZTSSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE:
	.string	"St23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE"
	.weak	_ZTISt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE
	.section	.rodata._ZTISt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE,"aG",@progbits,_ZTISt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE,comdat
	.align 8
	.type	_ZTISt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE, @object
	.size	_ZTISt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE, 24
_ZTISt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE:
	.quad	_ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	_ZTSSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE
	.quad	_ZTISt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE
	.weak	_ZTSSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE
	.section	.rodata._ZTSSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE,"aG",@progbits,_ZTSSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE,comdat
	.align 32
	.type	_ZTSSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE, @object
	.size	_ZTSSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE, 144
_ZTSSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE:
	.string	"St23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE"
	.weak	_ZTISt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE
	.section	.rodata._ZTISt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE,"aG",@progbits,_ZTISt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE,comdat
	.align 8
	.type	_ZTISt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE, @object
	.size	_ZTISt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE, 24
_ZTISt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE:
	.quad	_ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	_ZTSSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE
	.quad	_ZTISt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE2EE
	.weak	_ZTSNSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE
	.section	.rodata._ZTSNSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE,"aG",@progbits,_ZTSNSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE,comdat
	.align 32
	.type	_ZTSNSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE, @object
	.size	_ZTSNSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE, 134
_ZTSNSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE:
	.string	"NSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE"
	.weak	_ZTINSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE
	.section	.rodata._ZTINSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE,"aG",@progbits,_ZTINSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE,comdat
	.align 8
	.type	_ZTINSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE, @object
	.size	_ZTINSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE, 16
_ZTINSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE:
	.quad	_ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	_ZTSNSt13__future_base12_Task_setterISt10unique_ptrINS_7_ResultIiEENS_12_Result_base8_DeleterEENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE
	.weak	_ZTSNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEEE
	.section	.rodata._ZTSNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEEE,"aG",@progbits,_ZTSNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEEE,comdat
	.align 32
	.type	_ZTSNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEEE, @object
	.size	_ZTSNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEEE, 127
_ZTSNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEEE:
	.string	"NSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEEE"
	.weak	_ZTINSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEEE
	.section	.rodata._ZTINSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEEE,"aG",@progbits,_ZTINSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEEE,comdat
	.align 8
	.type	_ZTINSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEEE, @object
	.size	_ZTINSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEEE, 24
_ZTINSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEEE:
	.quad	_ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	_ZTSNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEEE
	.quad	_ZTINSt6thread6_StateE
	.weak	_ZTVNSt13__future_base13_State_baseV2E
	.section	.rodata._ZTVNSt13__future_base13_State_baseV2E,"aG",@progbits,_ZTVNSt13__future_base13_State_baseV2E,comdat
	.align 8
	.type	_ZTVNSt13__future_base13_State_baseV2E, @object
	.size	_ZTVNSt13__future_base13_State_baseV2E, 48
_ZTVNSt13__future_base13_State_baseV2E:
	.quad	0
	.quad	_ZTINSt13__future_base13_State_baseV2E
	.quad	_ZNSt13__future_base13_State_baseV2D1Ev
	.quad	_ZNSt13__future_base13_State_baseV2D0Ev
	.quad	_ZNSt13__future_base13_State_baseV217_M_complete_asyncEv
	.quad	_ZNKSt13__future_base13_State_baseV221_M_is_deferred_futureEv
	.weak	_ZTVNSt13__future_base21_Async_state_commonV2E
	.section	.rodata._ZTVNSt13__future_base21_Async_state_commonV2E,"aG",@progbits,_ZTVNSt13__future_base21_Async_state_commonV2E,comdat
	.align 8
	.type	_ZTVNSt13__future_base21_Async_state_commonV2E, @object
	.size	_ZTVNSt13__future_base21_Async_state_commonV2E, 48
_ZTVNSt13__future_base21_Async_state_commonV2E:
	.quad	0
	.quad	_ZTINSt13__future_base21_Async_state_commonV2E
	.quad	_ZNSt13__future_base21_Async_state_commonV2D1Ev
	.quad	_ZNSt13__future_base21_Async_state_commonV2D0Ev
	.quad	_ZNSt13__future_base21_Async_state_commonV217_M_complete_asyncEv
	.quad	_ZNKSt13__future_base13_State_baseV221_M_is_deferred_futureEv
	.weak	_ZTVNSt13__future_base7_ResultIiEE
	.section	.rodata._ZTVNSt13__future_base7_ResultIiEE,"aG",@progbits,_ZTVNSt13__future_base7_ResultIiEE,comdat
	.align 8
	.type	_ZTVNSt13__future_base7_ResultIiEE, @object
	.size	_ZTVNSt13__future_base7_ResultIiEE, 40
_ZTVNSt13__future_base7_ResultIiEE:
	.quad	0
	.quad	_ZTINSt13__future_base7_ResultIiEE
	.quad	_ZNSt13__future_base7_ResultIiE10_M_destroyEv
	.quad	_ZNSt13__future_base7_ResultIiED1Ev
	.quad	_ZNSt13__future_base7_ResultIiED0Ev
	.weak	_ZTVNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE
	.section	.rodata._ZTVNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE,"aG",@progbits,_ZTVNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE,comdat
	.align 8
	.type	_ZTVNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE, @object
	.size	_ZTVNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE, 48
_ZTVNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE:
	.quad	0
	.quad	_ZTINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE
	.quad	_ZNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED1Ev
	.quad	_ZNSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED0Ev
	.quad	_ZNSt13__future_base21_Async_state_commonV217_M_complete_asyncEv
	.quad	_ZNKSt13__future_base13_State_baseV221_M_is_deferred_futureEv
	.weak	_ZTVNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE
	.section	.rodata._ZTVNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE,"aG",@progbits,_ZTVNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE,comdat
	.align 8
	.type	_ZTVNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE, @object
	.size	_ZTVNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE, 48
_ZTVNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE:
	.quad	0
	.quad	_ZTINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEE
	.quad	_ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED1Ev
	.quad	_ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiED0Ev
	.quad	_ZNSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiE17_M_complete_asyncEv
	.quad	_ZNKSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiE21_M_is_deferred_futureEv
	.weak	_ZTVSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE
	.section	.rodata._ZTVSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE,"aG",@progbits,_ZTVSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE,comdat
	.align 8
	.type	_ZTVSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE, @object
	.size	_ZTVSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE, 56
_ZTVSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE:
	.quad	0
	.quad	_ZTISt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE
	.quad	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED1Ev
	.quad	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED0Ev
	.quad	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv
	.quad	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv
	.quad	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base17_Async_state_implINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info
	.weak	_ZTVSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE
	.section	.rodata._ZTVSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE,"aG",@progbits,_ZTVSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE,comdat
	.align 8
	.type	_ZTVSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE, @object
	.size	_ZTVSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE, 56
_ZTVSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE:
	.quad	0
	.quad	_ZTISt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE
	.quad	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED1Ev
	.quad	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EED0Ev
	.quad	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_disposeEv
	.quad	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE10_M_destroyEv
	.quad	_ZNSt23_Sp_counted_ptr_inplaceINSt13__future_base15_Deferred_stateINSt6thread8_InvokerISt5tupleIJPFivEEEEEiEESaIS9_ELN9__gnu_cxx12_Lock_policyE2EE14_M_get_deleterERKSt9type_info
	.weak	_ZTVNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEEE
	.section	.rodata._ZTVNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEEE,"aG",@progbits,_ZTVNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEEE,comdat
	.align 8
	.type	_ZTVNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEEE, @object
	.size	_ZTVNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEEE, 40
_ZTVNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEEE:
	.quad	0
	.quad	_ZTINSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEEE
	.quad	_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEED1Ev
	.quad	_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEED0Ev
	.quad	_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZNSt13__future_base17_Async_state_implINS1_IS2_IJPFivEEEEEiEC4EOS8_EUlvE_EEEEE6_M_runEv
	.weakref	_ZL28__gthrw___pthread_key_createPjPFvPvE,__pthread_key_create
	.weakref	_ZL20__gthrw_pthread_oncePiPFvvE,pthread_once
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC0:
	.quad	_ZNSt17_Function_handlerIFSt10unique_ptrINSt13__future_base12_Result_baseENS2_8_DeleterEEvENS1_12_Task_setterIS0_INS1_7_ResultIiEES3_ENSt6thread8_InvokerISt5tupleIJPFivEEEEEiEEE9_M_invokeERKSt9_Any_data
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC4:
	.quad	8391725880948585587
	.quad	8030606865272762997
	.ident	"GCC: (GNU) 7.0.0 20161204 (experimental)"
	.section	.note.GNU-stack,"",@progbits
