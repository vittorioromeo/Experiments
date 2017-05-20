	.text
	.file	"./uniqueptr.d"
	.section	.text._Dmain,"axG",@progbits,_Dmain,comdat
	.globl	_Dmain
	.p2align	4, 0x90
	.type	_Dmain,@function
_Dmain:
	.cfi_startproc
	pushq	%rax
.Lcfi0:
	.cfi_def_cfa_offset 16
	movq	_D24TypeInfo_S9uniqueptr3Foo6__initZ@GOTPCREL(%rip), %rdi
	movq	(%rdi), %rax
	callq	*88(%rax)
	xorl	%eax, %eax
	popq	%rcx
	retq
.Lfunc_end0:
	.size	_Dmain, .Lfunc_end0-_Dmain
	.cfi_endproc

	.section	.text._D9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr28__T6__ctorHTS9uniqueptr3FooZ6__ctorMFNaNbNcNiNfPS9uniqueptr3FooZS9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr,"axG",@progbits,_D9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr28__T6__ctorHTS9uniqueptr3FooZ6__ctorMFNaNbNcNiNfPS9uniqueptr3FooZS9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr,comdat
	.weak	_D9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr28__T6__ctorHTS9uniqueptr3FooZ6__ctorMFNaNbNcNiNfPS9uniqueptr3FooZS9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr
	.p2align	4, 0x90
	.type	_D9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr28__T6__ctorHTS9uniqueptr3FooZ6__ctorMFNaNbNcNiNfPS9uniqueptr3FooZS9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr,@function
_D9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr28__T6__ctorHTS9uniqueptr3FooZ6__ctorMFNaNbNcNiNfPS9uniqueptr3FooZS9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr:
	.cfi_startproc
	movq	%rsi, (%rdi)
	movq	%rdi, %rax
	retq
.Lfunc_end1:
	.size	_D9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr28__T6__ctorHTS9uniqueptr3FooZ6__ctorMFNaNbNcNiNfPS9uniqueptr3FooZS9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr, .Lfunc_end1-_D9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr28__T6__ctorHTS9uniqueptr3FooZ6__ctorMFNaNbNcNiNfPS9uniqueptr3FooZS9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr
	.cfi_endproc

	.section	.text._D9uniqueptr16MallocFreePolicy28__T5allocTS9uniqueptr3FooTiZ5allocFNbNiiZPS9uniqueptr3Foo,"axG",@progbits,_D9uniqueptr16MallocFreePolicy28__T5allocTS9uniqueptr3FooTiZ5allocFNbNiiZPS9uniqueptr3Foo,comdat
	.weak	_D9uniqueptr16MallocFreePolicy28__T5allocTS9uniqueptr3FooTiZ5allocFNbNiiZPS9uniqueptr3Foo
	.p2align	4, 0x90
	.type	_D9uniqueptr16MallocFreePolicy28__T5allocTS9uniqueptr3FooTiZ5allocFNbNiiZPS9uniqueptr3Foo,@function
_D9uniqueptr16MallocFreePolicy28__T5allocTS9uniqueptr3FooTiZ5allocFNbNiiZPS9uniqueptr3Foo:
	.cfi_startproc
	pushq	%rbx
.Lcfi1:
	.cfi_def_cfa_offset 16
.Lcfi2:
	.cfi_offset %rbx, -16
	movl	%edi, %ebx
	movl	$4, %edi
	callq	malloc@PLT
	movl	%ebx, (%rax)
	popq	%rbx
	retq
.Lfunc_end2:
	.size	_D9uniqueptr16MallocFreePolicy28__T5allocTS9uniqueptr3FooTiZ5allocFNbNiiZPS9uniqueptr3Foo, .Lfunc_end2-_D9uniqueptr16MallocFreePolicy28__T5allocTS9uniqueptr3FooTiZ5allocFNbNiiZPS9uniqueptr3Foo
	.cfi_endproc

	.section	.text._D9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr6__dtorMFNbNiZv,"axG",@progbits,_D9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr6__dtorMFNbNiZv,comdat
	.weak	_D9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr6__dtorMFNbNiZv
	.p2align	4, 0x90
	.type	_D9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr6__dtorMFNbNiZv,@function
_D9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr6__dtorMFNbNiZv:
	.cfi_startproc
	pushq	%r14
.Lcfi3:
	.cfi_def_cfa_offset 16
	pushq	%rbx
.Lcfi4:
	.cfi_def_cfa_offset 24
	pushq	%rax
.Lcfi5:
	.cfi_def_cfa_offset 32
.Lcfi6:
	.cfi_offset %rbx, -24
.Lcfi7:
	.cfi_offset %r14, -16
	movq	%rdi, %r14
	movq	(%r14), %rbx
	testq	%rbx, %rbx
	je	.LBB3_2
	movq	_D24TypeInfo_S9uniqueptr3Foo6__initZ@GOTPCREL(%rip), %rdi
	movq	(%rdi), %rax
	callq	*88(%rax)
	movq	%rbx, %rdi
	callq	free@PLT
	movq	$0, (%r14)
.LBB3_2:
	addq	$8, %rsp
	popq	%rbx
	popq	%r14
	retq
.Lfunc_end3:
	.size	_D9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr6__dtorMFNbNiZv, .Lfunc_end3-_D9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr6__dtorMFNbNiZv
	.cfi_endproc

	.section	.text._D9uniqueptr16MallocFreePolicy28__T7deallocTS9uniqueptr3FooZ7deallocFNbNiPS9uniqueptr3FooZv,"axG",@progbits,_D9uniqueptr16MallocFreePolicy28__T7deallocTS9uniqueptr3FooZ7deallocFNbNiPS9uniqueptr3FooZv,comdat
	.weak	_D9uniqueptr16MallocFreePolicy28__T7deallocTS9uniqueptr3FooZ7deallocFNbNiPS9uniqueptr3FooZv
	.p2align	4, 0x90
	.type	_D9uniqueptr16MallocFreePolicy28__T7deallocTS9uniqueptr3FooZ7deallocFNbNiPS9uniqueptr3FooZv,@function
_D9uniqueptr16MallocFreePolicy28__T7deallocTS9uniqueptr3FooZ7deallocFNbNiPS9uniqueptr3FooZv:
	.cfi_startproc
	pushq	%rbx
.Lcfi8:
	.cfi_def_cfa_offset 16
.Lcfi9:
	.cfi_offset %rbx, -16
	movq	%rdi, %rbx
	movq	_D24TypeInfo_S9uniqueptr3Foo6__initZ@GOTPCREL(%rip), %rdi
	movq	(%rdi), %rax
	callq	*88(%rax)
	movq	%rbx, %rdi
	popq	%rbx
	jmp	free@PLT
.Lfunc_end4:
	.size	_D9uniqueptr16MallocFreePolicy28__T7deallocTS9uniqueptr3FooZ7deallocFNbNiPS9uniqueptr3FooZv, .Lfunc_end4-_D9uniqueptr16MallocFreePolicy28__T7deallocTS9uniqueptr3FooZ7deallocFNbNiPS9uniqueptr3FooZv
	.cfi_endproc

	.section	.text._D9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr8opAssignMFNbNcNiNjS9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtrZS9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr,"axG",@progbits,_D9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr8opAssignMFNbNcNiNjS9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtrZS9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr,comdat
	.weak	_D9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr8opAssignMFNbNcNiNjS9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtrZS9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr
	.p2align	4, 0x90
	.type	_D9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr8opAssignMFNbNcNiNjS9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtrZS9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr,@function
_D9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr8opAssignMFNbNcNiNjS9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtrZS9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr:
	.cfi_startproc
	pushq	%r14
.Lcfi10:
	.cfi_def_cfa_offset 16
	pushq	%rbx
.Lcfi11:
	.cfi_def_cfa_offset 24
	pushq	%rax
.Lcfi12:
	.cfi_def_cfa_offset 32
.Lcfi13:
	.cfi_offset %rbx, -24
.Lcfi14:
	.cfi_offset %r14, -16
	movq	%rdi, %rbx
	movq	(%rbx), %r14
	movq	32(%rsp), %rax
	movq	%rax, (%rbx)
	testq	%r14, %r14
	je	.LBB5_2
	movq	_D24TypeInfo_S9uniqueptr3Foo6__initZ@GOTPCREL(%rip), %rdi
	movq	(%rdi), %rax
	callq	*88(%rax)
	movq	%r14, %rdi
	callq	free@PLT
.LBB5_2:
	movq	%rbx, %rax
	addq	$8, %rsp
	popq	%rbx
	popq	%r14
	retq
.Lfunc_end5:
	.size	_D9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr8opAssignMFNbNcNiNjS9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtrZS9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr, .Lfunc_end5-_D9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr8opAssignMFNbNcNiNjS9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtrZS9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr
	.cfi_endproc

	.section	.text._D6object28__T7destroyTS9uniqueptr3FooZ7destroyFNaNbNiNfKS9uniqueptr3FooZv,"axG",@progbits,_D6object28__T7destroyTS9uniqueptr3FooZ7destroyFNaNbNiNfKS9uniqueptr3FooZv,comdat
	.weak	_D6object28__T7destroyTS9uniqueptr3FooZ7destroyFNaNbNiNfKS9uniqueptr3FooZv
	.p2align	4, 0x90
	.type	_D6object28__T7destroyTS9uniqueptr3FooZ7destroyFNaNbNiNfKS9uniqueptr3FooZv,@function
_D6object28__T7destroyTS9uniqueptr3FooZ7destroyFNaNbNiNfKS9uniqueptr3FooZv:
	.cfi_startproc
	pushq	%rbx
.Lcfi15:
	.cfi_def_cfa_offset 16
.Lcfi16:
	.cfi_offset %rbx, -16
	movq	%rdi, %rbx
	movq	_D24TypeInfo_S9uniqueptr3Foo6__initZ@GOTPCREL(%rip), %rdi
	movq	(%rdi), %rax
	callq	*88(%rax)
	testq	%rdx, %rdx
	je	.LBB6_1
	movl	(%rdx), %eax
	jmp	.LBB6_3
.LBB6_1:
	xorl	%eax, %eax
.LBB6_3:
	movl	%eax, (%rbx)
	popq	%rbx
	retq
.Lfunc_end6:
	.size	_D6object28__T7destroyTS9uniqueptr3FooZ7destroyFNaNbNiNfKS9uniqueptr3FooZv, .Lfunc_end6-_D6object28__T7destroyTS9uniqueptr3FooZ7destroyFNaNbNiNfKS9uniqueptr3FooZv
	.cfi_endproc

	.section	.text._D6object38__T16_destructRecurseTS9uniqueptr3FooZ16_destructRecurseFNaNbNiNfKS9uniqueptr3FooZv,"axG",@progbits,_D6object38__T16_destructRecurseTS9uniqueptr3FooZ16_destructRecurseFNaNbNiNfKS9uniqueptr3FooZv,comdat
	.weak	_D6object38__T16_destructRecurseTS9uniqueptr3FooZ16_destructRecurseFNaNbNiNfKS9uniqueptr3FooZv
	.p2align	4, 0x90
	.type	_D6object38__T16_destructRecurseTS9uniqueptr3FooZ16_destructRecurseFNaNbNiNfKS9uniqueptr3FooZv,@function
_D6object38__T16_destructRecurseTS9uniqueptr3FooZ16_destructRecurseFNaNbNiNfKS9uniqueptr3FooZv:
	.cfi_startproc
	retq
.Lfunc_end7:
	.size	_D6object38__T16_destructRecurseTS9uniqueptr3FooZ16_destructRecurseFNaNbNiNfKS9uniqueptr3FooZv, .Lfunc_end7-_D6object38__T16_destructRecurseTS9uniqueptr3FooZ16_destructRecurseFNaNbNiNfKS9uniqueptr3FooZv
	.cfi_endproc

	.section	.text._D6object28__T7destroyTS9uniqueptr3FooZ7destroyFKS9uniqueptr3FooZ9__lambda2MFNaNbNiNeZv,"axG",@progbits,_D6object28__T7destroyTS9uniqueptr3FooZ7destroyFKS9uniqueptr3FooZ9__lambda2MFNaNbNiNeZv,comdat
	.weak	_D6object28__T7destroyTS9uniqueptr3FooZ7destroyFKS9uniqueptr3FooZ9__lambda2MFNaNbNiNeZv
	.p2align	4, 0x90
	.type	_D6object28__T7destroyTS9uniqueptr3FooZ7destroyFKS9uniqueptr3FooZ9__lambda2MFNaNbNiNeZv,@function
_D6object28__T7destroyTS9uniqueptr3FooZ7destroyFKS9uniqueptr3FooZ9__lambda2MFNaNbNiNeZv:
	.cfi_startproc
	pushq	%rbx
.Lcfi17:
	.cfi_def_cfa_offset 16
.Lcfi18:
	.cfi_offset %rbx, -16
	movq	(%rdi), %rbx
	movq	_D24TypeInfo_S9uniqueptr3Foo6__initZ@GOTPCREL(%rip), %rdi
	movq	(%rdi), %rax
	callq	*88(%rax)
	testq	%rdx, %rdx
	je	.LBB8_1
	movl	(%rdx), %eax
	movl	%eax, (%rbx)
	popq	%rbx
	retq
.LBB8_1:
	movl	$0, (%rbx)
	popq	%rbx
	retq
.Lfunc_end8:
	.size	_D6object28__T7destroyTS9uniqueptr3FooZ7destroyFKS9uniqueptr3FooZ9__lambda2MFNaNbNiNeZv, .Lfunc_end8-_D6object28__T7destroyTS9uniqueptr3FooZ7destroyFKS9uniqueptr3FooZ9__lambda2MFNaNbNiNeZv
	.cfi_endproc

	.section	.text._D3std4conv30__T7emplaceTS9uniqueptr3FooTiZ7emplaceFNaNbNiNfPS9uniqueptr3FooiZPS9uniqueptr3Foo,"axG",@progbits,_D3std4conv30__T7emplaceTS9uniqueptr3FooTiZ7emplaceFNaNbNiNfPS9uniqueptr3FooiZPS9uniqueptr3Foo,comdat
	.weak	_D3std4conv30__T7emplaceTS9uniqueptr3FooTiZ7emplaceFNaNbNiNfPS9uniqueptr3FooiZPS9uniqueptr3Foo
	.p2align	4, 0x90
	.type	_D3std4conv30__T7emplaceTS9uniqueptr3FooTiZ7emplaceFNaNbNiNfPS9uniqueptr3FooiZPS9uniqueptr3Foo,@function
_D3std4conv30__T7emplaceTS9uniqueptr3FooTiZ7emplaceFNaNbNiNfPS9uniqueptr3FooiZPS9uniqueptr3Foo:
	.cfi_startproc
	movl	%edi, (%rsi)
	movq	%rsi, %rax
	retq
.Lfunc_end9:
	.size	_D3std4conv30__T7emplaceTS9uniqueptr3FooTiZ7emplaceFNaNbNiNfPS9uniqueptr3FooiZPS9uniqueptr3Foo, .Lfunc_end9-_D3std4conv30__T7emplaceTS9uniqueptr3FooTiZ7emplaceFNaNbNiNfPS9uniqueptr3FooiZPS9uniqueptr3Foo
	.cfi_endproc

	.section	.text._D3std10functional94__T7forwardS79_D9uniqueptr16MallocFreePolicy28__T5allocTS9uniqueptr3FooTiZ5allocFiZ8_param_0iZ8__T3fwdZ3fwdMFNaNbNdNiNfZi,"axG",@progbits,_D3std10functional94__T7forwardS79_D9uniqueptr16MallocFreePolicy28__T5allocTS9uniqueptr3FooTiZ5allocFiZ8_param_0iZ8__T3fwdZ3fwdMFNaNbNdNiNfZi,comdat
	.weak	_D3std10functional94__T7forwardS79_D9uniqueptr16MallocFreePolicy28__T5allocTS9uniqueptr3FooTiZ5allocFiZ8_param_0iZ8__T3fwdZ3fwdMFNaNbNdNiNfZi
	.p2align	4, 0x90
	.type	_D3std10functional94__T7forwardS79_D9uniqueptr16MallocFreePolicy28__T5allocTS9uniqueptr3FooTiZ5allocFiZ8_param_0iZ8__T3fwdZ3fwdMFNaNbNdNiNfZi,@function
_D3std10functional94__T7forwardS79_D9uniqueptr16MallocFreePolicy28__T5allocTS9uniqueptr3FooTiZ5allocFiZ8_param_0iZ8__T3fwdZ3fwdMFNaNbNdNiNfZi:
	.cfi_startproc
	movl	(%rdi), %eax
	retq
.Lfunc_end10:
	.size	_D3std10functional94__T7forwardS79_D9uniqueptr16MallocFreePolicy28__T5allocTS9uniqueptr3FooTiZ5allocFiZ8_param_0iZ8__T3fwdZ3fwdMFNaNbNdNiNfZi, .Lfunc_end10-_D3std10functional94__T7forwardS79_D9uniqueptr16MallocFreePolicy28__T5allocTS9uniqueptr3FooTiZ5allocFiZ8_param_0iZ8__T3fwdZ3fwdMFNaNbNdNiNfZi
	.cfi_endproc

	.section	.text._D3std9algorithm8mutation11__T4moveTiZ4moveFNaNbNiNfKiZi,"axG",@progbits,_D3std9algorithm8mutation11__T4moveTiZ4moveFNaNbNiNfKiZi,comdat
	.weak	_D3std9algorithm8mutation11__T4moveTiZ4moveFNaNbNiNfKiZi
	.p2align	4, 0x90
	.type	_D3std9algorithm8mutation11__T4moveTiZ4moveFNaNbNiNfKiZi,@function
_D3std9algorithm8mutation11__T4moveTiZ4moveFNaNbNiNfKiZi:
	.cfi_startproc
	movl	(%rdi), %eax
	retq
.Lfunc_end11:
	.size	_D3std9algorithm8mutation11__T4moveTiZ4moveFNaNbNiNfKiZi, .Lfunc_end11-_D3std9algorithm8mutation11__T4moveTiZ4moveFNaNbNiNfKiZi
	.cfi_endproc

	.section	.text._D3std9algorithm8mutation23__T15trustedMoveImplTiZ15trustedMoveImplFNaNbNiNeKiZi,"axG",@progbits,_D3std9algorithm8mutation23__T15trustedMoveImplTiZ15trustedMoveImplFNaNbNiNeKiZi,comdat
	.weak	_D3std9algorithm8mutation23__T15trustedMoveImplTiZ15trustedMoveImplFNaNbNiNeKiZi
	.p2align	4, 0x90
	.type	_D3std9algorithm8mutation23__T15trustedMoveImplTiZ15trustedMoveImplFNaNbNiNeKiZi,@function
_D3std9algorithm8mutation23__T15trustedMoveImplTiZ15trustedMoveImplFNaNbNiNeKiZi:
	.cfi_startproc
	movl	(%rdi), %eax
	retq
.Lfunc_end12:
	.size	_D3std9algorithm8mutation23__T15trustedMoveImplTiZ15trustedMoveImplFNaNbNiNeKiZi, .Lfunc_end12-_D3std9algorithm8mutation23__T15trustedMoveImplTiZ15trustedMoveImplFNaNbNiNeKiZi
	.cfi_endproc

	.section	.text._D3std9algorithm8mutation15__T8moveImplTiZ8moveImplFNaNbNiKiZi,"axG",@progbits,_D3std9algorithm8mutation15__T8moveImplTiZ8moveImplFNaNbNiKiZi,comdat
	.weak	_D3std9algorithm8mutation15__T8moveImplTiZ8moveImplFNaNbNiKiZi
	.p2align	4, 0x90
	.type	_D3std9algorithm8mutation15__T8moveImplTiZ8moveImplFNaNbNiKiZi,@function
_D3std9algorithm8mutation15__T8moveImplTiZ8moveImplFNaNbNiKiZi:
	.cfi_startproc
	movl	(%rdi), %eax
	retq
.Lfunc_end13:
	.size	_D3std9algorithm8mutation15__T8moveImplTiZ8moveImplFNaNbNiKiZi, .Lfunc_end13-_D3std9algorithm8mutation15__T8moveImplTiZ8moveImplFNaNbNiKiZi
	.cfi_endproc

	.section	.text._D3std9algorithm8mutation19__T11moveEmplaceTiZ11moveEmplaceFNaNbNiKiKiZv,"axG",@progbits,_D3std9algorithm8mutation19__T11moveEmplaceTiZ11moveEmplaceFNaNbNiKiKiZv,comdat
	.weak	_D3std9algorithm8mutation19__T11moveEmplaceTiZ11moveEmplaceFNaNbNiKiKiZv
	.p2align	4, 0x90
	.type	_D3std9algorithm8mutation19__T11moveEmplaceTiZ11moveEmplaceFNaNbNiKiKiZv,@function
_D3std9algorithm8mutation19__T11moveEmplaceTiZ11moveEmplaceFNaNbNiKiKiZv:
	.cfi_startproc
	movl	(%rsi), %eax
	movl	%eax, (%rdi)
	retq
.Lfunc_end14:
	.size	_D3std9algorithm8mutation19__T11moveEmplaceTiZ11moveEmplaceFNaNbNiKiKiZv, .Lfunc_end14-_D3std9algorithm8mutation19__T11moveEmplaceTiZ11moveEmplaceFNaNbNiKiKiZv
	.cfi_endproc

	.section	.text._D3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFNaNbNiNfKS9uniqueptr3FooKiZv,"axG",@progbits,_D3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFNaNbNiNfKS9uniqueptr3FooKiZv,comdat
	.weak	_D3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFNaNbNiNfKS9uniqueptr3FooKiZv
	.p2align	4, 0x90
	.type	_D3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFNaNbNiNfKS9uniqueptr3FooKiZv,@function
_D3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFNaNbNiNfKS9uniqueptr3FooKiZv:
	.cfi_startproc
	movl	$0, (%rsi)
	movl	(%rdi), %eax
	movl	%eax, (%rsi)
	retq
.Lfunc_end15:
	.size	_D3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFNaNbNiNfKS9uniqueptr3FooKiZv, .Lfunc_end15-_D3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFNaNbNiNfKS9uniqueptr3FooKiZv
	.cfi_endproc

	.section	.text._D3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1S6__ctorMFNaNbNcNiNfKiZS3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1S,"axG",@progbits,_D3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1S6__ctorMFNaNbNcNiNfKiZS3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1S,comdat
	.weak	_D3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1S6__ctorMFNaNbNcNiNfKiZS3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1S
	.p2align	4, 0x90
	.type	_D3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1S6__ctorMFNaNbNcNiNfKiZS3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1S,@function
_D3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1S6__ctorMFNaNbNcNiNfKiZS3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1S:
	.cfi_startproc
	movl	(%rsi), %eax
	movl	%eax, (%rdi)
	movq	%rdi, %rax
	retq
.Lfunc_end16:
	.size	_D3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1S6__ctorMFNaNbNcNiNfKiZS3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1S, .Lfunc_end16-_D3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1S6__ctorMFNaNbNcNiNfKiZS3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1S
	.cfi_endproc

	.section	.text._D3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ9__lambda6MFNaNbNiNeZPS3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1S,"axG",@progbits,_D3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ9__lambda6MFNaNbNiNeZPS3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1S,comdat
	.weak	_D3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ9__lambda6MFNaNbNiNeZPS3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1S
	.p2align	4, 0x90
	.type	_D3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ9__lambda6MFNaNbNiNeZPS3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1S,@function
_D3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ9__lambda6MFNaNbNiNeZPS3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1S:
	.cfi_startproc
	movq	(%rdi), %rax
	retq
.Lfunc_end17:
	.size	_D3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ9__lambda6MFNaNbNiNeZPS3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1S, .Lfunc_end17-_D3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ9__lambda6MFNaNbNiNeZPS3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1S
	.cfi_endproc

	.section	.text._D3std4conv121__T18emplaceInitializerTS3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1SZ18emplaceInitializerFNaNbNiNeKS3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1SZv,"axG",@progbits,_D3std4conv121__T18emplaceInitializerTS3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1SZ18emplaceInitializerFNaNbNiNeKS3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1SZv,comdat
	.weak	_D3std4conv121__T18emplaceInitializerTS3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1SZ18emplaceInitializerFNaNbNiNeKS3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1SZv
	.p2align	4, 0x90
	.type	_D3std4conv121__T18emplaceInitializerTS3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1SZ18emplaceInitializerFNaNbNiNeKS3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1SZv,@function
_D3std4conv121__T18emplaceInitializerTS3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1SZ18emplaceInitializerFNaNbNiNeKS3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1SZv:
	.cfi_startproc
	movl	$0, (%rdi)
	retq
.Lfunc_end18:
	.size	_D3std4conv121__T18emplaceInitializerTS3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1SZ18emplaceInitializerFNaNbNiNeKS3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1SZv, .Lfunc_end18-_D3std4conv121__T18emplaceInitializerTS3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1SZ18emplaceInitializerFNaNbNiNeKS3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1SZv
	.cfi_endproc

	.section	.text.ldc.register_dso,"ax",@progbits
	.hidden	ldc.register_dso
	.weak	ldc.register_dso
	.p2align	4, 0x90
	.type	ldc.register_dso,@function
ldc.register_dso:
	.cfi_startproc
	movb	ldc.dso_initialized(%rip), %al
	xorb	%dil, %al
	testb	$1, %al
	jne	.LBB19_2
	pushq	%rbp
.Lcfi19:
	.cfi_def_cfa_offset 16
.Lcfi20:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
.Lcfi21:
	.cfi_def_cfa_register %rbp
	notb	%dil
	andb	$1, %dil
	movb	%dil, ldc.dso_initialized(%rip)
	movq	%rsp, %rax
	leaq	-48(%rax), %rdi
	movq	%rdi, %rsp
	movq	$1, -48(%rax)
	leaq	ldc.dso_slot(%rip), %rcx
	movq	%rcx, -40(%rax)
	leaq	__stop___minfo(%rip), %rcx
	movd	%rcx, %xmm0
	leaq	__start___minfo(%rip), %rcx
	movd	%rcx, %xmm1
	punpcklqdq	%xmm0, %xmm1
	movdqu	%xmm1, -32(%rax)
	movq	%rsi, -16(%rax)
	callq	_d_dso_registry@PLT
	movq	%rbp, %rsp
	popq	%rbp
.LBB19_2:
	retq
.Lfunc_end19:
	.size	ldc.register_dso, .Lfunc_end19-ldc.register_dso
	.cfi_endproc

	.section	.text.ldc.dso_ctor.9uniqueptr,"ax",@progbits
	.hidden	ldc.dso_ctor.9uniqueptr
	.weak	ldc.dso_ctor.9uniqueptr
	.p2align	4, 0x90
	.type	ldc.dso_ctor.9uniqueptr,@function
ldc.dso_ctor.9uniqueptr:
	.cfi_startproc
	movq	_D9uniqueptr11__moduleRefZ@GOTPCREL(%rip), %rsi
	xorl	%edi, %edi
	jmp	ldc.register_dso
.Lfunc_end20:
	.size	ldc.dso_ctor.9uniqueptr, .Lfunc_end20-ldc.dso_ctor.9uniqueptr
	.cfi_endproc

	.section	.text.ldc.dso_dtor.9uniqueptr,"ax",@progbits
	.hidden	ldc.dso_dtor.9uniqueptr
	.weak	ldc.dso_dtor.9uniqueptr
	.p2align	4, 0x90
	.type	ldc.dso_dtor.9uniqueptr,@function
ldc.dso_dtor.9uniqueptr:
	.cfi_startproc
	movq	_D9uniqueptr11__moduleRefZ@GOTPCREL(%rip), %rsi
	movl	$1, %edi
	jmp	ldc.register_dso
.Lfunc_end21:
	.size	ldc.dso_dtor.9uniqueptr, .Lfunc_end21-ldc.dso_dtor.9uniqueptr
	.cfi_endproc

	.section	.text.main,"axG",@progbits,main,comdat
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:
	.cfi_startproc
	movq	_Dmain@GOTPCREL(%rip), %rdx
	jmp	_d_run_main@PLT
.Lfunc_end22:
	.size	main, .Lfunc_end22-main
	.cfi_endproc

	.type	_D9uniqueptr16MallocFreePolicy6__initZ,@object
	.section	.rodata._D9uniqueptr16MallocFreePolicy6__initZ,"aG",@progbits,_D9uniqueptr16MallocFreePolicy6__initZ,comdat
	.globl	_D9uniqueptr16MallocFreePolicy6__initZ
_D9uniqueptr16MallocFreePolicy6__initZ:
	.zero	1
	.size	_D9uniqueptr16MallocFreePolicy6__initZ, 1

	.type	_D9uniqueptr3Foo6__initZ,@object
	.section	.rodata._D9uniqueptr3Foo6__initZ,"aG",@progbits,_D9uniqueptr3Foo6__initZ,comdat
	.globl	_D9uniqueptr3Foo6__initZ
	.p2align	2
_D9uniqueptr3Foo6__initZ:
	.zero	4
	.size	_D9uniqueptr3Foo6__initZ, 4

	.type	_D24TypeInfo_S9uniqueptr3Foo6__initZ,@object
	.section	.data._D24TypeInfo_S9uniqueptr3Foo6__initZ,"aGw",@progbits,_D24TypeInfo_S9uniqueptr3Foo6__initZ,comdat
	.weak	_D24TypeInfo_S9uniqueptr3Foo6__initZ
	.p2align	4
_D24TypeInfo_S9uniqueptr3Foo6__initZ:
	.quad	_D15TypeInfo_Struct6__vtblZ
	.quad	0
	.quad	13
	.quad	.L.str.1
	.quad	4
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.long	0
	.zero	4
	.quad	0
	.quad	0
	.long	4
	.zero	4
	.quad	_D10TypeInfo_i6__initZ
	.quad	0
	.quad	0
	.size	_D24TypeInfo_S9uniqueptr3Foo6__initZ, 136

	.type	.L.str.1,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str.1:
	.asciz	"uniqueptr.Foo"
	.size	.L.str.1, 14

	.type	_D9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr6__initZ,@object
	.section	.rodata._D9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr6__initZ,"aG",@progbits,_D9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr6__initZ,comdat
	.weak	_D9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr6__initZ
	.p2align	3
_D9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr6__initZ:
	.zero	8
	.size	_D9uniqueptr60__T9UniquePtrTS9uniqueptr3FooTS9uniqueptr16MallocFreePolicyZ9UniquePtr6__initZ, 8

	.type	_D3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1S6__initZ,@object
	.section	.rodata._D3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1S6__initZ,"aG",@progbits,_D3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1S6__initZ,comdat
	.weak	_D3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1S6__initZ
	.p2align	2
_D3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1S6__initZ:
	.zero	4
	.size	_D3std4conv50__T10emplaceRefTS9uniqueptr3FooTS9uniqueptr3FooTiZ10emplaceRefFKS9uniqueptr3FooKiZ1S6__initZ, 4

	.type	_D9uniqueptr12__ModuleInfoZ,@object
	.section	.data._D9uniqueptr12__ModuleInfoZ,"aGw",@progbits,_D9uniqueptr12__ModuleInfoZ,comdat
	.globl	_D9uniqueptr12__ModuleInfoZ
	.p2align	4
_D9uniqueptr12__ModuleInfoZ:
	.long	2147483652
	.long	0
	.asciz	"uniqueptr"
	.zero	2
	.size	_D9uniqueptr12__ModuleInfoZ, 20

	.type	_D9uniqueptr11__moduleRefZ,@object
	.section	__minfo,"aw",@progbits
	.weak	_D9uniqueptr11__moduleRefZ
	.p2align	3
_D9uniqueptr11__moduleRefZ:
	.quad	_D9uniqueptr12__ModuleInfoZ
	.size	_D9uniqueptr11__moduleRefZ, 8

	.hidden	__start___minfo
	.hidden	__stop___minfo
	.hidden	ldc.dso_slot
	.type	ldc.dso_slot,@object
	.section	.bss.ldc.dso_slot,"aw",@nobits
	.weak	ldc.dso_slot
	.p2align	3
ldc.dso_slot:
	.quad	0
	.size	ldc.dso_slot, 8

	.hidden	ldc.dso_initialized
	.type	ldc.dso_initialized,@object
	.section	.bss.ldc.dso_initialized,"aw",@nobits
	.weak	ldc.dso_initialized
ldc.dso_initialized:
	.byte	0
	.size	ldc.dso_initialized, 1

	.section	.ctors,"aw",@progbits
	.p2align	3
	.quad	ldc.dso_ctor.9uniqueptr
	.section	.dtors,"aw",@progbits
	.p2align	3
	.quad	ldc.dso_dtor.9uniqueptr
	.type	_d_execBssBegAddr,@object
	.section	.data._d_execBssBegAddr,"aw",@progbits
	.globl	_d_execBssBegAddr
	.p2align	3
_d_execBssBegAddr:
	.quad	__bss_start
	.size	_d_execBssBegAddr, 8

	.type	_d_execBssEndAddr,@object
	.section	.data._d_execBssEndAddr,"aw",@progbits
	.globl	_d_execBssEndAddr
	.p2align	3
_d_execBssEndAddr:
	.quad	_end
	.size	_d_execBssEndAddr, 8

	.weak	__bss_start
	.weak	_end

	.ident	"ldc version 1.2.0"
	.section	".note.GNU-stack","",@progbits
