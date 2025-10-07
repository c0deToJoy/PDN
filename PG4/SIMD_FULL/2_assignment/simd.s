	.file	"simd.c"
	.text
	.globl	max_pair_wise_diff
	.type	max_pair_wise_diff, @function
max_pair_wise_diff:
.LFB6440:
	.cfi_startproc
	endbr64
	testl	%edi, %edi
	jle	.L7
	movslq	%edi, %rdi
	leaq	0(,%rdi,4), %rcx
	movl	$0, %eax
	vxorps	%xmm3, %xmm3, %xmm3
	vmovss	.LC1(%rip), %xmm5
	vmovaps	%xmm3, %xmm4
	vmovaps	%xmm5, %xmm6
	jmp	.L6
.L9:
	vaddss	%xmm0, %xmm0, %xmm0
	vandps	%xmm6, %xmm1, %xmm1
	vdivss	%xmm1, %xmm0, %xmm0
.L3:
	vmaxss	%xmm3, %xmm0, %xmm3
	addq	$4, %rax
	cmpq	%rcx, %rax
	je	.L1
.L6:
	vmovss	(%rsi,%rax), %xmm0
	vmovss	(%rdx,%rax), %xmm2
	vaddss	%xmm2, %xmm0, %xmm1
	vsubss	%xmm2, %xmm0, %xmm0
	vandps	%xmm5, %xmm0, %xmm0
	vucomiss	%xmm4, %xmm1
	jp	.L9
	je	.L3
	jmp	.L9
.L7:
	vxorps	%xmm3, %xmm3, %xmm3
.L1:
	vmovaps	%xmm3, %xmm0
	ret
	.cfi_endproc
.LFE6440:
	.size	max_pair_wise_diff, .-max_pair_wise_diff
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC2:
	.string	"%s = [ "
.LC3:
	.string	" x, "
.LC4:
	.string	"%2.f, "
.LC5:
	.string	"]\n"
	.text
	.globl	print_8xfloat_mem
	.type	print_8xfloat_mem, @function
print_8xfloat_mem:
.LFB6441:
	.cfi_startproc
	endbr64
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	movq	%rdi, %rdx
	movq	%rsi, %rbp
	leaq	.LC2(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	movq	%rbp, %rbx
	addq	$32, %rbp
	movl	$0x00000000, %r12d
	leaq	.LC4(%rip), %r13
	leaq	.LC3(%rip), %r14
	jmp	.L15
.L18:
	vcvtss2sd	%xmm0, %xmm0, %xmm0
	movq	%r13, %rsi
	movl	$2, %edi
	movl	$1, %eax
	call	__printf_chk@PLT
.L14:
	addq	$4, %rbx
	cmpq	%rbp, %rbx
	je	.L20
.L15:
	vmovss	(%rbx), %xmm0
	vmovd	%r12d, %xmm1
	vcomiss	%xmm0, %xmm1
	jbe	.L18
	vcvtss2sd	%xmm0, %xmm0, %xmm0
	movq	%r14, %rsi
	movl	$2, %edi
	movl	$1, %eax
	call	__printf_chk@PLT
	jmp	.L14
.L20:
	leaq	.LC5(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE6441:
	.size	print_8xfloat_mem, .-print_8xfloat_mem
	.globl	print_float_mem
	.type	print_float_mem, @function
print_float_mem:
.LFB6442:
	.cfi_startproc
	endbr64
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	movl	%esi, %ebp
	movq	%rdx, %r12
	movq	%rdi, %rdx
	leaq	.LC2(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	testl	%ebp, %ebp
	jle	.L22
	movq	%r12, %rbx
	movslq	%ebp, %rbp
	leaq	(%r12,%rbp,4), %r12
	movl	$0x00000000, %ebp
	leaq	.LC4(%rip), %r13
	leaq	.LC3(%rip), %r14
	jmp	.L26
.L29:
	vcvtss2sd	%xmm0, %xmm0, %xmm0
	movq	%r13, %rsi
	movl	$2, %edi
	movl	$1, %eax
	call	__printf_chk@PLT
.L25:
	addq	$4, %rbx
	cmpq	%r12, %rbx
	je	.L22
.L26:
	vmovss	(%rbx), %xmm0
	vmovd	%ebp, %xmm1
	vcomiss	%xmm0, %xmm1
	jbe	.L29
	vcvtss2sd	%xmm0, %xmm0, %xmm0
	movq	%r14, %rsi
	movl	$2, %edi
	movl	$1, %eax
	call	__printf_chk@PLT
	jmp	.L25
.L22:
	leaq	.LC5(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE6442:
	.size	print_float_mem, .-print_float_mem
	.section	.rodata.str1.1
.LC6:
	.string	"%s[%2i:%2i] = [ "
.LC7:
	.string	"\n"
	.text
	.globl	print_float_mem_as_vects
	.type	print_float_mem_as_vects, @function
print_float_mem_as_vects:
.LFB6443:
	.cfi_startproc
	endbr64
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
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
	subq	$40, %rsp
	.cfi_def_cfa_offset 96
	movq	%rdi, (%rsp)
	testl	%esi, %esi
	jle	.L32
	movl	%esi, %eax
	movl	%edx, %r15d
	movslq	%edx, %rsi
	leaq	0(,%rsi,4), %rdx
	leaq	(%rcx,%rdx), %rbp
	negq	%rsi
	leaq	0(,%rsi,4), %rcx
	movq	%rcx, 24(%rsp)
	movl	$0, %r12d
	leaq	.LC4(%rip), %r13
	leaq	.LC3(%rip), %r14
	movq	%rdx, 8(%rsp)
	movl	%eax, 20(%rsp)
	jmp	.L38
.L44:
	vcvtss2sd	%xmm0, %xmm0, %xmm0
	movq	%r14, %rsi
	movl	$2, %edi
	movl	$1, %eax
	call	__printf_chk@PLT
.L36:
	addq	$4, %rbx
	cmpq	%rbp, %rbx
	je	.L33
.L37:
	vmovss	(%rbx), %xmm0
	vxorps	%xmm1, %xmm1, %xmm1
	vcomiss	%xmm0, %xmm1
	ja	.L44
	vcvtss2sd	%xmm0, %xmm0, %xmm0
	movq	%r13, %rsi
	movl	$2, %edi
	movl	$1, %eax
	call	__printf_chk@PLT
	jmp	.L36
.L33:
	leaq	.LC5(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	movq	8(%rsp), %rax
	addq	%rax, %rbp
	movl	20(%rsp), %eax
	cmpl	%eax, %r12d
	jge	.L32
.L38:
	movl	%r12d, %ecx
	addl	%r15d, %r12d
	movl	%r12d, %r8d
	movq	(%rsp), %rdx
	leaq	.LC6(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	testl	%r15d, %r15d
	jle	.L33
	movq	24(%rsp), %rax
	leaq	0(%rbp,%rax), %rbx
	jmp	.L37
.L32:
	leaq	.LC7(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	addq	$40, %rsp
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
	.cfi_endproc
.LFE6443:
	.size	print_float_mem_as_vects, .-print_float_mem_as_vects
	.globl	reference_reverse_8xfloat
	.type	reference_reverse_8xfloat, @function
reference_reverse_8xfloat:
.LFB6444:
	.cfi_startproc
	endbr64
	movq	%rdi, %rax
	addq	$28, %rsi
	addq	$32, %rdi
.L46:
#APP
# 169 "simd.c" 1
	
	 movl    $111,%ebx #IACA/OSACA START MARKER
	 .byte   100,103,144     #IACA/OSACA START MARKER
# 0 "" 2
#NO_APP
	vmovss	(%rax), %xmm0
	vmovss	%xmm0, (%rsi)
#APP
# 173 "simd.c" 1
	
	 movl    $222,%ebx #IACA/OSACA START MARKER
	 .byte   100,103,144     #IACA/OSACA START MARKER
# 0 "" 2
#NO_APP
	addq	$4, %rax
	subq	$4, %rsi
	cmpq	%rdi, %rax
	jne	.L46
	ret
	.cfi_endproc
.LFE6444:
	.size	reference_reverse_8xfloat, .-reference_reverse_8xfloat
	.globl	student_reverse_8xfloat
	.type	student_reverse_8xfloat, @function
student_reverse_8xfloat:
.LFB6445:
	.cfi_startproc
	endbr64
	vmovups	(%rdi), %ymm0
#APP
# 188 "simd.c" 1
	
	 movl    $111,%ebx #IACA/OSACA START MARKER
	 .byte   100,103,144     #IACA/OSACA START MARKER
# 0 "" 2
#NO_APP
	vmovdqa	.LC8(%rip), %ymm1
	vpermps	%ymm0, %ymm1, %ymm0
#APP
# 205 "simd.c" 1
	
	 movl    $222,%ebx #IACA/OSACA START MARKER
	 .byte   100,103,144     #IACA/OSACA START MARKER
# 0 "" 2
#NO_APP
	vmovups	%ymm0, (%rsi)
	ret
	.cfi_endproc
.LFE6445:
	.size	student_reverse_8xfloat, .-student_reverse_8xfloat
	.section	.rodata.str1.1
.LC17:
	.string	"test_reverse_8xfloat: "
.LC19:
	.string	"FAIL\n"
.LC20:
	.string	" a"
.LC21:
	.string	"bt"
.LC22:
	.string	"br"
.LC23:
	.string	"PASS\n"
	.text
	.globl	test_reverse_8xfloat
	.type	test_reverse_8xfloat, @function
test_reverse_8xfloat:
.LFB6446:
	.cfi_startproc
	endbr64
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	addq	$-128, %rsp
	.cfi_def_cfa_offset 160
	movq	%fs:40, %rax
	movq	%rax, 120(%rsp)
	xorl	%eax, %eax
	movl	$0x00000000, 16(%rsp)
	movl	$0x3f800000, 20(%rsp)
	movl	$0x40000000, 24(%rsp)
	movl	$0x40400000, 28(%rsp)
	movl	$0x40800000, 32(%rsp)
	movl	$0x40a00000, 36(%rsp)
	movl	$0x40c00000, 40(%rsp)
	movl	$0x40e00000, 44(%rsp)
	vmovss	.LC16(%rip), %xmm0
	vmovss	%xmm0, 48(%rsp)
	vmovss	%xmm0, 52(%rsp)
	vmovss	%xmm0, 56(%rsp)
	vmovss	%xmm0, 60(%rsp)
	vmovss	%xmm0, 64(%rsp)
	vmovss	%xmm0, 68(%rsp)
	vmovss	%xmm0, 72(%rsp)
	vmovss	%xmm0, 76(%rsp)
	leaq	48(%rsp), %rbx
	leaq	16(%rsp), %r12
	movq	%rbx, %rsi
	movq	%r12, %rdi
	call	reference_reverse_8xfloat
	leaq	80(%rsp), %rbp
	movq	%rbp, %rsi
	movq	%r12, %rdi
	call	student_reverse_8xfloat
	movq	%rbp, %rdx
	movq	%rbx, %rsi
	movl	$8, %edi
	call	max_pair_wise_diff
	vmovss	%xmm0, 12(%rsp)
	leaq	.LC17(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	vcvtss2sd	12(%rsp), %xmm0, %xmm0
	vcomisd	.LC18(%rip), %xmm0
	jbe	.L55
	leaq	.LC19(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	movq	%r12, %rsi
	leaq	.LC20(%rip), %rdi
	call	print_8xfloat_mem
	movq	%rbx, %rsi
	leaq	.LC21(%rip), %rdi
	call	print_8xfloat_mem
	movq	%rbp, %rsi
	leaq	.LC22(%rip), %rdi
	call	print_8xfloat_mem
	leaq	.LC7(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
.L49:
	movq	120(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L57
	subq	$-128, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 32
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
.L55:
	.cfi_restore_state
	leaq	.LC23(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	jmp	.L49
.L57:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE6446:
	.size	test_reverse_8xfloat, .-test_reverse_8xfloat
	.globl	reference_rotate_by_4_8xfloat
	.type	reference_rotate_by_4_8xfloat, @function
reference_rotate_by_4_8xfloat:
.LFB6447:
	.cfi_startproc
	endbr64
	movl	$0, %edx
.L59:
	vmovss	(%rdi,%rdx,4), %xmm0
	leal	4(%rdx), %eax
	movl	%eax, %ecx
	sarl	$31, %ecx
	shrl	$29, %ecx
	addl	%ecx, %eax
	andl	$7, %eax
	subl	%ecx, %eax
	cltq
	vmovss	%xmm0, (%rsi,%rax,4)
	addq	$1, %rdx
	cmpq	$8, %rdx
	jne	.L59
	ret
	.cfi_endproc
.LFE6447:
	.size	reference_rotate_by_4_8xfloat, .-reference_rotate_by_4_8xfloat
	.globl	student_rotate_by_4_8xfloat
	.type	student_rotate_by_4_8xfloat, @function
student_rotate_by_4_8xfloat:
.LFB6448:
	.cfi_startproc
	endbr64
	vbroadcastss	.LC16(%rip), %ymm0
	vmovups	%ymm0, (%rsi)
	ret
	.cfi_endproc
.LFE6448:
	.size	student_rotate_by_4_8xfloat, .-student_rotate_by_4_8xfloat
	.section	.rodata.str1.1
.LC25:
	.string	"test_rotate_by_4_8xfloat: "
	.text
	.globl	test_rotate_by_4_8xfloat
	.type	test_rotate_by_4_8xfloat, @function
test_rotate_by_4_8xfloat:
.LFB6449:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$136, %rsp
	.cfi_offset 3, -24
	movq	%fs:40, %rax
	movq	%rax, 120(%rsp)
	xorl	%eax, %eax
	movl	$0x00000000, 16(%rsp)
	movl	$0x3f800000, 20(%rsp)
	movl	$0x40000000, 24(%rsp)
	movl	$0x40400000, 28(%rsp)
	movl	$0x40800000, 32(%rsp)
	movl	$0x40a00000, 36(%rsp)
	movl	$0x40c00000, 40(%rsp)
	movl	$0x40e00000, 44(%rsp)
	vmovss	.LC16(%rip), %xmm0
	vmovss	%xmm0, 48(%rsp)
	vmovss	%xmm0, 52(%rsp)
	vmovss	%xmm0, 56(%rsp)
	vmovss	%xmm0, 60(%rsp)
	vmovss	%xmm0, 64(%rsp)
	vmovss	%xmm0, 68(%rsp)
	vmovss	%xmm0, 72(%rsp)
	vmovss	%xmm0, 76(%rsp)
	leaq	48(%rsp), %rbx
	leaq	16(%rsp), %rdi
	movq	%rbx, %rsi
	call	reference_rotate_by_4_8xfloat
	vbroadcastss	.LC16(%rip), %ymm0
	vmovups	%ymm0, 80(%rsp)
	leaq	80(%rsp), %rdx
	movq	%rbx, %rsi
	movl	$8, %edi
	call	max_pair_wise_diff
	vmovss	%xmm0, 12(%rsp)
	leaq	.LC25(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	vcvtss2sd	12(%rsp), %xmm0, %xmm0
	vcomisd	.LC18(%rip), %xmm0
	jbe	.L68
	leaq	.LC19(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	leaq	16(%rsp), %rsi
	leaq	.LC20(%rip), %rdi
	call	print_8xfloat_mem
	movq	%rbx, %rsi
	leaq	.LC21(%rip), %rdi
	call	print_8xfloat_mem
	leaq	80(%rsp), %rsi
	leaq	.LC22(%rip), %rdi
	call	print_8xfloat_mem
	leaq	.LC7(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
.L62:
	movq	120(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L70
	movq	-8(%rbp), %rbx
	leave
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
.L68:
	.cfi_restore_state
	leaq	.LC23(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	jmp	.L62
.L70:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE6449:
	.size	test_rotate_by_4_8xfloat, .-test_rotate_by_4_8xfloat
	.globl	reference_rotate_by_2_8xfloat
	.type	reference_rotate_by_2_8xfloat, @function
reference_rotate_by_2_8xfloat:
.LFB6450:
	.cfi_startproc
	endbr64
	movl	$0, %edx
.L72:
	vmovss	(%rdi,%rdx,4), %xmm0
	leal	6(%rdx), %eax
	movl	%eax, %ecx
	sarl	$31, %ecx
	shrl	$29, %ecx
	addl	%ecx, %eax
	andl	$7, %eax
	subl	%ecx, %eax
	cltq
	vmovss	%xmm0, (%rsi,%rax,4)
	addq	$1, %rdx
	cmpq	$8, %rdx
	jne	.L72
	ret
	.cfi_endproc
.LFE6450:
	.size	reference_rotate_by_2_8xfloat, .-reference_rotate_by_2_8xfloat
	.globl	student_rotate_by_2_8xfloat
	.type	student_rotate_by_2_8xfloat, @function
student_rotate_by_2_8xfloat:
.LFB6451:
	.cfi_startproc
	endbr64
	vbroadcastss	.LC16(%rip), %ymm0
	vmovups	%ymm0, (%rsi)
	ret
	.cfi_endproc
.LFE6451:
	.size	student_rotate_by_2_8xfloat, .-student_rotate_by_2_8xfloat
	.section	.rodata.str1.1
.LC26:
	.string	"test_rotate_by_2_8xfloat: "
	.text
	.globl	test_rotate_by_2_8xfloat
	.type	test_rotate_by_2_8xfloat, @function
test_rotate_by_2_8xfloat:
.LFB6452:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$136, %rsp
	.cfi_offset 3, -24
	movq	%fs:40, %rax
	movq	%rax, 120(%rsp)
	xorl	%eax, %eax
	movl	$0x00000000, 16(%rsp)
	movl	$0x3f800000, 20(%rsp)
	movl	$0x40000000, 24(%rsp)
	movl	$0x40400000, 28(%rsp)
	movl	$0x40800000, 32(%rsp)
	movl	$0x40a00000, 36(%rsp)
	movl	$0x40c00000, 40(%rsp)
	movl	$0x40e00000, 44(%rsp)
	vmovss	.LC16(%rip), %xmm0
	vmovss	%xmm0, 48(%rsp)
	vmovss	%xmm0, 52(%rsp)
	vmovss	%xmm0, 56(%rsp)
	vmovss	%xmm0, 60(%rsp)
	vmovss	%xmm0, 64(%rsp)
	vmovss	%xmm0, 68(%rsp)
	vmovss	%xmm0, 72(%rsp)
	vmovss	%xmm0, 76(%rsp)
	leaq	48(%rsp), %rbx
	leaq	16(%rsp), %rdi
	movq	%rbx, %rsi
	call	reference_rotate_by_2_8xfloat
	vbroadcastss	.LC16(%rip), %ymm0
	vmovups	%ymm0, 80(%rsp)
	leaq	80(%rsp), %rdx
	movq	%rbx, %rsi
	movl	$8, %edi
	call	max_pair_wise_diff
	vmovss	%xmm0, 12(%rsp)
	leaq	.LC26(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	vcvtss2sd	12(%rsp), %xmm0, %xmm0
	vcomisd	.LC18(%rip), %xmm0
	jbe	.L81
	leaq	.LC19(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	leaq	16(%rsp), %rsi
	leaq	.LC20(%rip), %rdi
	call	print_8xfloat_mem
	movq	%rbx, %rsi
	leaq	.LC21(%rip), %rdi
	call	print_8xfloat_mem
	leaq	80(%rsp), %rsi
	leaq	.LC22(%rip), %rdi
	call	print_8xfloat_mem
	leaq	.LC7(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
.L75:
	movq	120(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L83
	movq	-8(%rbp), %rbx
	leave
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
.L81:
	.cfi_restore_state
	leaq	.LC23(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	jmp	.L75
.L83:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE6452:
	.size	test_rotate_by_2_8xfloat, .-test_rotate_by_2_8xfloat
	.globl	reference_rotate_by_1_8xfloat
	.type	reference_rotate_by_1_8xfloat, @function
reference_rotate_by_1_8xfloat:
.LFB6453:
	.cfi_startproc
	endbr64
	movl	$0, %edx
.L85:
	vmovss	(%rdi,%rdx,4), %xmm0
	leal	7(%rdx), %eax
	movl	%eax, %ecx
	sarl	$31, %ecx
	shrl	$29, %ecx
	addl	%ecx, %eax
	andl	$7, %eax
	subl	%ecx, %eax
	cltq
	vmovss	%xmm0, (%rsi,%rax,4)
	addq	$1, %rdx
	cmpq	$8, %rdx
	jne	.L85
	ret
	.cfi_endproc
.LFE6453:
	.size	reference_rotate_by_1_8xfloat, .-reference_rotate_by_1_8xfloat
	.globl	student_rotate_by_1_8xfloat
	.type	student_rotate_by_1_8xfloat, @function
student_rotate_by_1_8xfloat:
.LFB6454:
	.cfi_startproc
	endbr64
	vbroadcastss	.LC16(%rip), %ymm0
	vmovups	%ymm0, (%rsi)
	ret
	.cfi_endproc
.LFE6454:
	.size	student_rotate_by_1_8xfloat, .-student_rotate_by_1_8xfloat
	.section	.rodata.str1.1
.LC27:
	.string	"test_rotate_by_1_8xfloat: "
	.text
	.globl	test_rotate_by_1_8xfloat
	.type	test_rotate_by_1_8xfloat, @function
test_rotate_by_1_8xfloat:
.LFB6455:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$136, %rsp
	.cfi_offset 3, -24
	movq	%fs:40, %rax
	movq	%rax, 120(%rsp)
	xorl	%eax, %eax
	movl	$0x00000000, 16(%rsp)
	movl	$0x3f800000, 20(%rsp)
	movl	$0x40000000, 24(%rsp)
	movl	$0x40400000, 28(%rsp)
	movl	$0x40800000, 32(%rsp)
	movl	$0x40a00000, 36(%rsp)
	movl	$0x40c00000, 40(%rsp)
	movl	$0x40e00000, 44(%rsp)
	vmovss	.LC16(%rip), %xmm0
	vmovss	%xmm0, 48(%rsp)
	vmovss	%xmm0, 52(%rsp)
	vmovss	%xmm0, 56(%rsp)
	vmovss	%xmm0, 60(%rsp)
	vmovss	%xmm0, 64(%rsp)
	vmovss	%xmm0, 68(%rsp)
	vmovss	%xmm0, 72(%rsp)
	vmovss	%xmm0, 76(%rsp)
	leaq	48(%rsp), %rbx
	leaq	16(%rsp), %rdi
	movq	%rbx, %rsi
	call	reference_rotate_by_1_8xfloat
	vbroadcastss	.LC16(%rip), %ymm0
	vmovups	%ymm0, 80(%rsp)
	leaq	80(%rsp), %rdx
	movq	%rbx, %rsi
	movl	$8, %edi
	call	max_pair_wise_diff
	vmovss	%xmm0, 12(%rsp)
	leaq	.LC27(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	vcvtss2sd	12(%rsp), %xmm0, %xmm0
	vcomisd	.LC18(%rip), %xmm0
	jbe	.L94
	leaq	.LC19(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	leaq	16(%rsp), %rsi
	leaq	.LC20(%rip), %rdi
	call	print_8xfloat_mem
	movq	%rbx, %rsi
	leaq	.LC21(%rip), %rdi
	call	print_8xfloat_mem
	leaq	80(%rsp), %rsi
	leaq	.LC22(%rip), %rdi
	call	print_8xfloat_mem
	leaq	.LC7(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
.L88:
	movq	120(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L96
	movq	-8(%rbp), %rbx
	leave
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
.L94:
	.cfi_restore_state
	leaq	.LC23(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	jmp	.L88
.L96:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE6455:
	.size	test_rotate_by_1_8xfloat, .-test_rotate_by_1_8xfloat
	.globl	reference_transpose_4x2_colmaj_8xfloat
	.type	reference_transpose_4x2_colmaj_8xfloat, @function
reference_transpose_4x2_colmaj_8xfloat:
.LFB6456:
	.cfi_startproc
	endbr64
	movl	$0, %eax
.L98:
	vmovss	(%rdi,%rax), %xmm0
	vmovss	%xmm0, (%rsi,%rax,2)
	vmovss	16(%rdi,%rax), %xmm0
	vmovss	%xmm0, 4(%rsi,%rax,2)
	addq	$4, %rax
	cmpq	$16, %rax
	jne	.L98
	ret
	.cfi_endproc
.LFE6456:
	.size	reference_transpose_4x2_colmaj_8xfloat, .-reference_transpose_4x2_colmaj_8xfloat
	.globl	student_transpose_4x2_colmaj_8xfloat
	.type	student_transpose_4x2_colmaj_8xfloat, @function
student_transpose_4x2_colmaj_8xfloat:
.LFB6457:
	.cfi_startproc
	endbr64
	vbroadcastss	.LC16(%rip), %ymm0
	vmovups	%ymm0, (%rsi)
	ret
	.cfi_endproc
.LFE6457:
	.size	student_transpose_4x2_colmaj_8xfloat, .-student_transpose_4x2_colmaj_8xfloat
	.globl	reference_rotate_by_1_16xfloat
	.type	reference_rotate_by_1_16xfloat, @function
reference_rotate_by_1_16xfloat:
.LFB6458:
	.cfi_startproc
	endbr64
	movl	$0, %edx
.L102:
	vmovss	(%rdi,%rdx,4), %xmm0
	leal	15(%rdx), %eax
	movl	%eax, %ecx
	sarl	$31, %ecx
	shrl	$28, %ecx
	addl	%ecx, %eax
	andl	$15, %eax
	subl	%ecx, %eax
	cltq
	vmovss	%xmm0, (%rsi,%rax,4)
	addq	$1, %rdx
	cmpq	$16, %rdx
	jne	.L102
	ret
	.cfi_endproc
.LFE6458:
	.size	reference_rotate_by_1_16xfloat, .-reference_rotate_by_1_16xfloat
	.globl	student_rotate_by_1_16xfloat
	.type	student_rotate_by_1_16xfloat, @function
student_rotate_by_1_16xfloat:
.LFB6459:
	.cfi_startproc
	endbr64
	vbroadcastss	.LC16(%rip), %ymm0
	vmovups	%ymm0, (%rsi)
	vmovups	%ymm0, 32(%rsi)
	ret
	.cfi_endproc
.LFE6459:
	.size	student_rotate_by_1_16xfloat, .-student_rotate_by_1_16xfloat
	.section	.rodata.str1.1
.LC36:
	.string	"test_rotate_by_1_16xfloat: "
	.text
	.globl	test_rotate_by_1_16xfloat
	.type	test_rotate_by_1_16xfloat, @function
test_rotate_by_1_16xfloat:
.LFB6460:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$232, %rsp
	.cfi_offset 3, -24
	movq	%fs:40, %rax
	movq	%rax, 216(%rsp)
	xorl	%eax, %eax
	movl	$0x00000000, 16(%rsp)
	movl	$0x3f800000, 20(%rsp)
	movl	$0x40000000, 24(%rsp)
	movl	$0x40400000, 28(%rsp)
	movl	$0x40800000, 32(%rsp)
	movl	$0x40a00000, 36(%rsp)
	movl	$0x40c00000, 40(%rsp)
	movl	$0x40e00000, 44(%rsp)
	movl	$0x41000000, 48(%rsp)
	movl	$0x41100000, 52(%rsp)
	movl	$0x41200000, 56(%rsp)
	movl	$0x41300000, 60(%rsp)
	movl	$0x41400000, 64(%rsp)
	movl	$0x41500000, 68(%rsp)
	movl	$0x41600000, 72(%rsp)
	movl	$0x41700000, 76(%rsp)
	vmovss	.LC16(%rip), %xmm0
	vmovss	%xmm0, 80(%rsp)
	vmovss	%xmm0, 84(%rsp)
	vmovss	%xmm0, 88(%rsp)
	vmovss	%xmm0, 92(%rsp)
	vmovss	%xmm0, 96(%rsp)
	vmovss	%xmm0, 100(%rsp)
	vmovss	%xmm0, 104(%rsp)
	vmovss	%xmm0, 108(%rsp)
	vmovss	%xmm0, 112(%rsp)
	vmovss	%xmm0, 116(%rsp)
	vmovss	%xmm0, 120(%rsp)
	vmovss	%xmm0, 124(%rsp)
	vmovss	%xmm0, 128(%rsp)
	vmovss	%xmm0, 132(%rsp)
	vmovss	%xmm0, 136(%rsp)
	vmovss	%xmm0, 140(%rsp)
	leaq	80(%rsp), %rbx
	leaq	16(%rsp), %rdi
	movq	%rbx, %rsi
	call	reference_rotate_by_1_16xfloat
	vbroadcastss	.LC16(%rip), %ymm0
	vmovups	%ymm0, 144(%rsp)
	vmovups	%ymm0, 176(%rsp)
	leaq	144(%rsp), %rdx
	movq	%rbx, %rsi
	movl	$16, %edi
	call	max_pair_wise_diff
	vmovss	%xmm0, 12(%rsp)
	leaq	.LC36(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	vcvtss2sd	12(%rsp), %xmm0, %xmm0
	vcomisd	.LC18(%rip), %xmm0
	jbe	.L111
	leaq	.LC19(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	leaq	16(%rsp), %rdx
	movl	$16, %esi
	leaq	.LC20(%rip), %rdi
	call	print_float_mem
	movq	%rbx, %rdx
	movl	$16, %esi
	leaq	.LC21(%rip), %rdi
	call	print_float_mem
	leaq	144(%rsp), %rdx
	movl	$16, %esi
	leaq	.LC22(%rip), %rdi
	call	print_float_mem
	leaq	.LC7(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
.L105:
	movq	216(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L113
	movq	-8(%rbp), %rbx
	leave
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
.L111:
	.cfi_restore_state
	leaq	.LC23(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	jmp	.L105
.L113:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE6460:
	.size	test_rotate_by_1_16xfloat, .-test_rotate_by_1_16xfloat
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC37:
	.string	"test_transpose_4x2_colmaj_8xfloat: "
	.text
	.globl	test_transpose_4x2_colmaj_8xfloat
	.type	test_transpose_4x2_colmaj_8xfloat, @function
test_transpose_4x2_colmaj_8xfloat:
.LFB6461:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$136, %rsp
	.cfi_offset 3, -24
	movq	%fs:40, %rax
	movq	%rax, 120(%rsp)
	xorl	%eax, %eax
	movl	$0x00000000, 16(%rsp)
	movl	$0x3f800000, 20(%rsp)
	movl	$0x40000000, 24(%rsp)
	movl	$0x40400000, 28(%rsp)
	movl	$0x40800000, 32(%rsp)
	movl	$0x40a00000, 36(%rsp)
	movl	$0x40c00000, 40(%rsp)
	movl	$0x40e00000, 44(%rsp)
	vmovss	.LC16(%rip), %xmm0
	vmovss	%xmm0, 48(%rsp)
	vmovss	%xmm0, 52(%rsp)
	vmovss	%xmm0, 56(%rsp)
	vmovss	%xmm0, 60(%rsp)
	vmovss	%xmm0, 64(%rsp)
	vmovss	%xmm0, 68(%rsp)
	vmovss	%xmm0, 72(%rsp)
	vmovss	%xmm0, 76(%rsp)
	leaq	48(%rsp), %rbx
	leaq	16(%rsp), %rdi
	movq	%rbx, %rsi
	call	reference_transpose_4x2_colmaj_8xfloat
	vbroadcastss	.LC16(%rip), %ymm0
	vmovups	%ymm0, 80(%rsp)
	leaq	80(%rsp), %rdx
	movq	%rbx, %rsi
	movl	$8, %edi
	call	max_pair_wise_diff
	vmovss	%xmm0, 12(%rsp)
	leaq	.LC37(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	vcvtss2sd	12(%rsp), %xmm0, %xmm0
	vcomisd	.LC18(%rip), %xmm0
	jbe	.L120
	leaq	.LC19(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	leaq	16(%rsp), %rsi
	leaq	.LC20(%rip), %rdi
	call	print_8xfloat_mem
	movq	%rbx, %rsi
	leaq	.LC21(%rip), %rdi
	call	print_8xfloat_mem
	leaq	80(%rsp), %rsi
	leaq	.LC22(%rip), %rdi
	call	print_8xfloat_mem
	leaq	.LC7(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
.L114:
	movq	120(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L122
	movq	-8(%rbp), %rbx
	leave
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
.L120:
	.cfi_restore_state
	leaq	.LC23(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	jmp	.L114
.L122:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE6461:
	.size	test_transpose_4x2_colmaj_8xfloat, .-test_transpose_4x2_colmaj_8xfloat
	.globl	reference_transpose_4x4_colmaj_16xfloat
	.type	reference_transpose_4x4_colmaj_16xfloat, @function
reference_transpose_4x4_colmaj_16xfloat:
.LFB6462:
	.cfi_startproc
	endbr64
	addq	$64, %rdi
	movl	$0, %ecx
.L124:
	leaq	-64(%rdi), %rax
	movq	%rsi, %rdx
.L125:
	vmovss	(%rax), %xmm0
	vmovss	%xmm0, (%rdx)
	addq	$16, %rax
	addq	$4, %rdx
	cmpq	%rdi, %rax
	jne	.L125
	addl	$1, %ecx
	addq	$16, %rsi
	addq	$4, %rdi
	cmpl	$4, %ecx
	jne	.L124
	ret
	.cfi_endproc
.LFE6462:
	.size	reference_transpose_4x4_colmaj_16xfloat, .-reference_transpose_4x4_colmaj_16xfloat
	.globl	student_transpose_4x4_colmaj_8xfloat
	.type	student_transpose_4x4_colmaj_8xfloat, @function
student_transpose_4x4_colmaj_8xfloat:
.LFB6463:
	.cfi_startproc
	endbr64
	vbroadcastss	.LC16(%rip), %ymm0
	vmovups	%ymm0, (%rsi)
	vmovups	%ymm0, 32(%rsi)
	ret
	.cfi_endproc
.LFE6463:
	.size	student_transpose_4x4_colmaj_8xfloat, .-student_transpose_4x4_colmaj_8xfloat
	.section	.rodata.str1.8
	.align 8
.LC38:
	.string	"test_transpose_4x4_colmaj_8xfloat: "
	.text
	.globl	test_transpose_4x4_colmaj_8xfloat
	.type	test_transpose_4x4_colmaj_8xfloat, @function
test_transpose_4x4_colmaj_8xfloat:
.LFB6464:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$232, %rsp
	.cfi_offset 3, -24
	movq	%fs:40, %rax
	movq	%rax, 216(%rsp)
	xorl	%eax, %eax
	movl	$0x00000000, 16(%rsp)
	movl	$0x3f800000, 20(%rsp)
	movl	$0x40000000, 24(%rsp)
	movl	$0x40400000, 28(%rsp)
	movl	$0x40800000, 32(%rsp)
	movl	$0x40a00000, 36(%rsp)
	movl	$0x40c00000, 40(%rsp)
	movl	$0x40e00000, 44(%rsp)
	movl	$0x41000000, 48(%rsp)
	movl	$0x41100000, 52(%rsp)
	movl	$0x41200000, 56(%rsp)
	movl	$0x41300000, 60(%rsp)
	movl	$0x41400000, 64(%rsp)
	movl	$0x41500000, 68(%rsp)
	movl	$0x41600000, 72(%rsp)
	movl	$0x41700000, 76(%rsp)
	vmovss	.LC16(%rip), %xmm0
	vmovss	%xmm0, 80(%rsp)
	vmovss	%xmm0, 84(%rsp)
	vmovss	%xmm0, 88(%rsp)
	vmovss	%xmm0, 92(%rsp)
	vmovss	%xmm0, 96(%rsp)
	vmovss	%xmm0, 100(%rsp)
	vmovss	%xmm0, 104(%rsp)
	vmovss	%xmm0, 108(%rsp)
	vmovss	%xmm0, 112(%rsp)
	vmovss	%xmm0, 116(%rsp)
	vmovss	%xmm0, 120(%rsp)
	vmovss	%xmm0, 124(%rsp)
	vmovss	%xmm0, 128(%rsp)
	vmovss	%xmm0, 132(%rsp)
	vmovss	%xmm0, 136(%rsp)
	vmovss	%xmm0, 140(%rsp)
	leaq	80(%rsp), %rbx
	leaq	16(%rsp), %rdi
	movq	%rbx, %rsi
	call	reference_transpose_4x4_colmaj_16xfloat
	vbroadcastss	.LC16(%rip), %ymm0
	vmovups	%ymm0, 144(%rsp)
	vmovups	%ymm0, 176(%rsp)
	leaq	144(%rsp), %rdx
	movq	%rbx, %rsi
	movl	$16, %edi
	call	max_pair_wise_diff
	vmovss	%xmm0, 12(%rsp)
	leaq	.LC38(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	vcvtss2sd	12(%rsp), %xmm0, %xmm0
	vcomisd	.LC18(%rip), %xmm0
	jbe	.L135
	leaq	.LC19(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	leaq	16(%rsp), %rdx
	movl	$16, %esi
	leaq	.LC20(%rip), %rdi
	call	print_float_mem
	movq	%rbx, %rdx
	movl	$16, %esi
	leaq	.LC21(%rip), %rdi
	call	print_float_mem
	leaq	144(%rsp), %rdx
	movl	$16, %esi
	leaq	.LC22(%rip), %rdi
	call	print_float_mem
	leaq	.LC7(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
.L129:
	movq	216(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L137
	movq	-8(%rbp), %rbx
	leave
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
.L135:
	.cfi_restore_state
	leaq	.LC23(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	jmp	.L129
.L137:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE6464:
	.size	test_transpose_4x4_colmaj_8xfloat, .-test_transpose_4x4_colmaj_8xfloat
	.globl	reference_transpose_8x4_colmaj_32xfloat
	.type	reference_transpose_8x4_colmaj_32xfloat, @function
reference_transpose_8x4_colmaj_32xfloat:
.LFB6465:
	.cfi_startproc
	endbr64
	subq	$-128, %rdi
	movl	$0, %ecx
	jmp	.L139
.L143:
	addl	$1, %ecx
	addq	$16, %rsi
	addq	$4, %rdi
	cmpl	$8, %ecx
	je	.L138
.L139:
	leaq	-128(%rdi), %rax
	movq	%rsi, %rdx
.L140:
	vmovss	(%rax), %xmm0
	vmovss	%xmm0, (%rdx)
	addq	$32, %rax
	addq	$4, %rdx
	cmpq	%rdi, %rax
	jne	.L140
	jmp	.L143
.L138:
	ret
	.cfi_endproc
.LFE6465:
	.size	reference_transpose_8x4_colmaj_32xfloat, .-reference_transpose_8x4_colmaj_32xfloat
	.globl	student_transpose_8x4_colmaj_8xfloat
	.type	student_transpose_8x4_colmaj_8xfloat, @function
student_transpose_8x4_colmaj_8xfloat:
.LFB6466:
	.cfi_startproc
	endbr64
	vbroadcastss	.LC16(%rip), %ymm0
	vmovups	%ymm0, (%rsi)
	vmovups	%ymm0, 32(%rsi)
	vmovups	%ymm0, 64(%rsi)
	vmovups	%ymm0, 96(%rsi)
	ret
	.cfi_endproc
.LFE6466:
	.size	student_transpose_8x4_colmaj_8xfloat, .-student_transpose_8x4_colmaj_8xfloat
	.section	.rodata.str1.8
	.align 8
.LC55:
	.string	"test_transpose_8x4_colmaj_8xfloat: "
	.text
	.globl	test_transpose_8x4_colmaj_8xfloat
	.type	test_transpose_8x4_colmaj_8xfloat, @function
test_transpose_8x4_colmaj_8xfloat:
.LFB6467:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$424, %rsp
	.cfi_offset 3, -24
	movq	%fs:40, %rax
	movq	%rax, 408(%rsp)
	xorl	%eax, %eax
	movl	$0x00000000, 16(%rsp)
	movl	$0x3f800000, 20(%rsp)
	movl	$0x40000000, 24(%rsp)
	movl	$0x40400000, 28(%rsp)
	movl	$0x40800000, 32(%rsp)
	movl	$0x40a00000, 36(%rsp)
	movl	$0x40c00000, 40(%rsp)
	movl	$0x40e00000, 44(%rsp)
	movl	$0x41000000, 48(%rsp)
	movl	$0x41100000, 52(%rsp)
	movl	$0x41200000, 56(%rsp)
	movl	$0x41300000, 60(%rsp)
	movl	$0x41400000, 64(%rsp)
	movl	$0x41500000, 68(%rsp)
	movl	$0x41600000, 72(%rsp)
	movl	$0x41700000, 76(%rsp)
	movl	$0x41800000, 80(%rsp)
	movl	$0x41880000, 84(%rsp)
	movl	$0x41900000, 88(%rsp)
	movl	$0x41980000, 92(%rsp)
	movl	$0x41a00000, 96(%rsp)
	movl	$0x41a80000, 100(%rsp)
	movl	$0x41b00000, 104(%rsp)
	movl	$0x41b80000, 108(%rsp)
	movl	$0x41c00000, 112(%rsp)
	movl	$0x41c80000, 116(%rsp)
	movl	$0x41d00000, 120(%rsp)
	movl	$0x41d80000, 124(%rsp)
	movl	$0x41e00000, 128(%rsp)
	movl	$0x41e80000, 132(%rsp)
	movl	$0x41f00000, 136(%rsp)
	movl	$0x41f80000, 140(%rsp)
	vmovss	.LC16(%rip), %xmm0
	vmovss	%xmm0, 144(%rsp)
	vmovss	%xmm0, 148(%rsp)
	vmovss	%xmm0, 152(%rsp)
	vmovss	%xmm0, 156(%rsp)
	vmovss	%xmm0, 160(%rsp)
	vmovss	%xmm0, 164(%rsp)
	vmovss	%xmm0, 168(%rsp)
	vmovss	%xmm0, 172(%rsp)
	vmovss	%xmm0, 176(%rsp)
	vmovss	%xmm0, 180(%rsp)
	vmovss	%xmm0, 184(%rsp)
	vmovss	%xmm0, 188(%rsp)
	vmovss	%xmm0, 192(%rsp)
	vmovss	%xmm0, 196(%rsp)
	vmovss	%xmm0, 200(%rsp)
	vmovss	%xmm0, 204(%rsp)
	vmovss	%xmm0, 208(%rsp)
	vmovss	%xmm0, 212(%rsp)
	vmovss	%xmm0, 216(%rsp)
	vmovss	%xmm0, 220(%rsp)
	vmovss	%xmm0, 224(%rsp)
	vmovss	%xmm0, 228(%rsp)
	vmovss	%xmm0, 232(%rsp)
	vmovss	%xmm0, 236(%rsp)
	vmovss	%xmm0, 240(%rsp)
	vmovss	%xmm0, 244(%rsp)
	vmovss	%xmm0, 248(%rsp)
	vmovss	%xmm0, 252(%rsp)
	vmovss	%xmm0, 256(%rsp)
	vmovss	%xmm0, 260(%rsp)
	vmovss	%xmm0, 264(%rsp)
	vmovss	%xmm0, 268(%rsp)
	leaq	144(%rsp), %rbx
	leaq	16(%rsp), %rdi
	movq	%rbx, %rsi
	call	reference_transpose_8x4_colmaj_32xfloat
	vbroadcastss	.LC16(%rip), %ymm0
	vmovups	%ymm0, 272(%rsp)
	vmovups	%ymm0, 304(%rsp)
	vmovups	%ymm0, 336(%rsp)
	vmovups	%ymm0, 368(%rsp)
	leaq	272(%rsp), %rdx
	movq	%rbx, %rsi
	movl	$32, %edi
	call	max_pair_wise_diff
	vmovss	%xmm0, 12(%rsp)
	leaq	.LC55(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	vcvtss2sd	12(%rsp), %xmm0, %xmm0
	vcomisd	.LC18(%rip), %xmm0
	jbe	.L151
	leaq	.LC19(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	leaq	16(%rsp), %rcx
	movl	$8, %edx
	movl	$32, %esi
	leaq	.LC20(%rip), %rdi
	call	print_float_mem_as_vects
	movq	%rbx, %rcx
	movl	$8, %edx
	movl	$32, %esi
	leaq	.LC21(%rip), %rdi
	call	print_float_mem_as_vects
	leaq	272(%rsp), %rcx
	movl	$8, %edx
	movl	$32, %esi
	leaq	.LC22(%rip), %rdi
	call	print_float_mem_as_vects
	leaq	.LC7(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
.L145:
	movq	408(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L153
	movq	-8(%rbp), %rbx
	leave
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
.L151:
	.cfi_restore_state
	leaq	.LC23(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	jmp	.L145
.L153:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE6467:
	.size	test_transpose_8x4_colmaj_8xfloat, .-test_transpose_8x4_colmaj_8xfloat
	.globl	reference_gather_at_stride_8xfloat
	.type	reference_gather_at_stride_8xfloat, @function
reference_gather_at_stride_8xfloat:
.LFB6468:
	.cfi_startproc
	endbr64
	movq	%rdi, %rax
	subq	$-128, %rdi
.L155:
	vmovss	(%rax), %xmm0
	vmovss	%xmm0, (%rsi)
	addq	$16, %rax
	addq	$4, %rsi
	cmpq	%rdi, %rax
	jne	.L155
	ret
	.cfi_endproc
.LFE6468:
	.size	reference_gather_at_stride_8xfloat, .-reference_gather_at_stride_8xfloat
	.globl	student_gather_at_stride_8xfloat
	.type	student_gather_at_stride_8xfloat, @function
student_gather_at_stride_8xfloat:
.LFB6469:
	.cfi_startproc
	endbr64
	vbroadcastss	.LC16(%rip), %ymm0
	vmovups	%ymm0, (%rsi)
	ret
	.cfi_endproc
.LFE6469:
	.size	student_gather_at_stride_8xfloat, .-student_gather_at_stride_8xfloat
	.section	.rodata.str1.8
	.align 8
.LC56:
	.string	"test_gather_at_stride_8xfloat: "
	.text
	.globl	test_gather_at_stride_8xfloat
	.type	test_gather_at_stride_8xfloat, @function
test_gather_at_stride_8xfloat:
.LFB6470:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$232, %rsp
	.cfi_offset 3, -24
	movq	%fs:40, %rax
	movq	%rax, 216(%rsp)
	xorl	%eax, %eax
	movl	$0x00000000, 80(%rsp)
	movl	$0x3f800000, 84(%rsp)
	movl	$0x40000000, 88(%rsp)
	movl	$0x40400000, 92(%rsp)
	movl	$0x40800000, 96(%rsp)
	movl	$0x40a00000, 100(%rsp)
	movl	$0x40c00000, 104(%rsp)
	movl	$0x40e00000, 108(%rsp)
	movl	$0x41000000, 112(%rsp)
	movl	$0x41100000, 116(%rsp)
	movl	$0x41200000, 120(%rsp)
	movl	$0x41300000, 124(%rsp)
	movl	$0x41400000, 128(%rsp)
	movl	$0x41500000, 132(%rsp)
	movl	$0x41600000, 136(%rsp)
	movl	$0x41700000, 140(%rsp)
	movl	$0x41800000, 144(%rsp)
	movl	$0x41880000, 148(%rsp)
	movl	$0x41900000, 152(%rsp)
	movl	$0x41980000, 156(%rsp)
	movl	$0x41a00000, 160(%rsp)
	movl	$0x41a80000, 164(%rsp)
	movl	$0x41b00000, 168(%rsp)
	movl	$0x41b80000, 172(%rsp)
	movl	$0x41c00000, 176(%rsp)
	movl	$0x41c80000, 180(%rsp)
	movl	$0x41d00000, 184(%rsp)
	movl	$0x41d80000, 188(%rsp)
	movl	$0x41e00000, 192(%rsp)
	movl	$0x41e80000, 196(%rsp)
	movl	$0x41f00000, 200(%rsp)
	movl	$0x41f80000, 204(%rsp)
	vmovss	.LC16(%rip), %xmm0
	vmovss	%xmm0, 16(%rsp)
	vmovss	%xmm0, 20(%rsp)
	vmovss	%xmm0, 24(%rsp)
	vmovss	%xmm0, 28(%rsp)
	vmovss	%xmm0, 32(%rsp)
	vmovss	%xmm0, 36(%rsp)
	vmovss	%xmm0, 40(%rsp)
	vmovss	%xmm0, 44(%rsp)
	leaq	16(%rsp), %rbx
	leaq	80(%rsp), %rdi
	movq	%rbx, %rsi
	call	reference_gather_at_stride_8xfloat
	vbroadcastss	.LC16(%rip), %ymm0
	vmovups	%ymm0, 48(%rsp)
	leaq	48(%rsp), %rdx
	movq	%rbx, %rsi
	movl	$8, %edi
	call	max_pair_wise_diff
	vmovss	%xmm0, 12(%rsp)
	leaq	.LC56(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	vcvtss2sd	12(%rsp), %xmm0, %xmm0
	vcomisd	.LC18(%rip), %xmm0
	jbe	.L164
	leaq	.LC19(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	leaq	80(%rsp), %rcx
	movl	$8, %edx
	movl	$32, %esi
	leaq	.LC20(%rip), %rdi
	call	print_float_mem_as_vects
	movq	%rbx, %rcx
	movl	$8, %edx
	movl	$8, %esi
	leaq	.LC21(%rip), %rdi
	call	print_float_mem_as_vects
	leaq	48(%rsp), %rcx
	movl	$8, %edx
	movl	$8, %esi
	leaq	.LC22(%rip), %rdi
	call	print_float_mem_as_vects
	leaq	.LC7(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
.L158:
	movq	216(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L166
	movq	-8(%rbp), %rbx
	leave
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
.L164:
	.cfi_restore_state
	leaq	.LC23(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	jmp	.L158
.L166:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE6470:
	.size	test_gather_at_stride_8xfloat, .-test_gather_at_stride_8xfloat
	.globl	reference_scatter_at_stride_8xfloat
	.type	reference_scatter_at_stride_8xfloat, @function
reference_scatter_at_stride_8xfloat:
.LFB6471:
	.cfi_startproc
	endbr64
	movq	%rdi, %rax
	addq	$32, %rdi
.L168:
	vmovss	(%rax), %xmm0
	vmovss	%xmm0, (%rsi)
	addq	$4, %rax
	addq	$16, %rsi
	cmpq	%rdi, %rax
	jne	.L168
	ret
	.cfi_endproc
.LFE6471:
	.size	reference_scatter_at_stride_8xfloat, .-reference_scatter_at_stride_8xfloat
	.globl	student_scatter_at_stride_8xfloat
	.type	student_scatter_at_stride_8xfloat, @function
student_scatter_at_stride_8xfloat:
.LFB6472:
	.cfi_startproc
	endbr64
	vbroadcastss	.LC16(%rip), %ymm0
	vmovups	%ymm0, (%rsi)
	vmovups	%ymm0, 32(%rsi)
	vmovups	%ymm0, 64(%rsi)
	vmovups	%ymm0, 96(%rsi)
	ret
	.cfi_endproc
.LFE6472:
	.size	student_scatter_at_stride_8xfloat, .-student_scatter_at_stride_8xfloat
	.section	.rodata.str1.8
	.align 8
.LC57:
	.string	"test_scatter_at_stride_8xfloat: "
	.text
	.globl	test_scatter_at_stride_8xfloat
	.type	test_scatter_at_stride_8xfloat, @function
test_scatter_at_stride_8xfloat:
.LFB6473:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$328, %rsp
	.cfi_offset 3, -24
	movq	%fs:40, %rax
	movq	%rax, 312(%rsp)
	xorl	%eax, %eax
	movl	$0x00000000, 16(%rsp)
	movl	$0x3f800000, 20(%rsp)
	movl	$0x40000000, 24(%rsp)
	movl	$0x40400000, 28(%rsp)
	movl	$0x40800000, 32(%rsp)
	movl	$0x40a00000, 36(%rsp)
	movl	$0x40c00000, 40(%rsp)
	movl	$0x40e00000, 44(%rsp)
	vmovss	.LC16(%rip), %xmm0
	vmovss	%xmm0, 48(%rsp)
	vmovss	%xmm0, 52(%rsp)
	vmovss	%xmm0, 56(%rsp)
	vmovss	%xmm0, 60(%rsp)
	vmovss	%xmm0, 64(%rsp)
	vmovss	%xmm0, 68(%rsp)
	vmovss	%xmm0, 72(%rsp)
	vmovss	%xmm0, 76(%rsp)
	vmovss	%xmm0, 80(%rsp)
	vmovss	%xmm0, 84(%rsp)
	vmovss	%xmm0, 88(%rsp)
	vmovss	%xmm0, 92(%rsp)
	vmovss	%xmm0, 96(%rsp)
	vmovss	%xmm0, 100(%rsp)
	vmovss	%xmm0, 104(%rsp)
	vmovss	%xmm0, 108(%rsp)
	vmovss	%xmm0, 112(%rsp)
	vmovss	%xmm0, 116(%rsp)
	vmovss	%xmm0, 120(%rsp)
	vmovss	%xmm0, 124(%rsp)
	vmovss	%xmm0, 128(%rsp)
	vmovss	%xmm0, 132(%rsp)
	vmovss	%xmm0, 136(%rsp)
	vmovss	%xmm0, 140(%rsp)
	vmovss	%xmm0, 144(%rsp)
	vmovss	%xmm0, 148(%rsp)
	vmovss	%xmm0, 152(%rsp)
	vmovss	%xmm0, 156(%rsp)
	vmovss	%xmm0, 160(%rsp)
	vmovss	%xmm0, 164(%rsp)
	vmovss	%xmm0, 168(%rsp)
	vmovss	%xmm0, 172(%rsp)
	leaq	48(%rsp), %rbx
	leaq	16(%rsp), %rdi
	movq	%rbx, %rsi
	call	reference_scatter_at_stride_8xfloat
	vbroadcastss	.LC16(%rip), %ymm0
	vmovups	%ymm0, 176(%rsp)
	vmovups	%ymm0, 208(%rsp)
	vmovups	%ymm0, 240(%rsp)
	vmovups	%ymm0, 272(%rsp)
	leaq	176(%rsp), %rdx
	movq	%rbx, %rsi
	movl	$32, %edi
	call	max_pair_wise_diff
	vmovss	%xmm0, 12(%rsp)
	leaq	.LC57(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	vcvtss2sd	12(%rsp), %xmm0, %xmm0
	vcomisd	.LC18(%rip), %xmm0
	jbe	.L177
	leaq	.LC19(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	leaq	16(%rsp), %rcx
	movl	$8, %edx
	movl	$8, %esi
	leaq	.LC20(%rip), %rdi
	call	print_float_mem_as_vects
	movq	%rbx, %rcx
	movl	$8, %edx
	movl	$32, %esi
	leaq	.LC21(%rip), %rdi
	call	print_float_mem_as_vects
	leaq	176(%rsp), %rcx
	movl	$8, %edx
	movl	$32, %esi
	leaq	.LC22(%rip), %rdi
	call	print_float_mem_as_vects
	leaq	.LC7(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
.L171:
	movq	312(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L179
	movq	-8(%rbp), %rbx
	leave
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
.L177:
	.cfi_restore_state
	leaq	.LC23(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	jmp	.L171
.L179:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE6473:
	.size	test_scatter_at_stride_8xfloat, .-test_scatter_at_stride_8xfloat
	.globl	reference_matvec_8x8_colmaj_64xfloat
	.type	reference_matvec_8x8_colmaj_64xfloat, @function
reference_matvec_8x8_colmaj_64xfloat:
.LFB6474:
	.cfi_startproc
	endbr64
	movl	$0, %r9d
	leaq	32(%rsi), %r8
.L181:
	movq	%rsi, %rax
	movq	%rdi, %rcx
.L182:
	vmovss	(%rcx), %xmm0
	vmulss	(%rax), %xmm0, %xmm0
	vaddss	(%rdx), %xmm0, %xmm0
	vmovss	%xmm0, (%rdx)
	addq	$32, %rcx
	addq	$4, %rax
	cmpq	%r8, %rax
	jne	.L182
	addl	$1, %r9d
	addq	$4, %rdi
	addq	$4, %rdx
	cmpl	$8, %r9d
	jne	.L181
	ret
	.cfi_endproc
.LFE6474:
	.size	reference_matvec_8x8_colmaj_64xfloat, .-reference_matvec_8x8_colmaj_64xfloat
	.globl	student_matvec_8x8_colmaj_8xfloat
	.type	student_matvec_8x8_colmaj_8xfloat, @function
student_matvec_8x8_colmaj_8xfloat:
.LFB6475:
	.cfi_startproc
	endbr64
	vxorps	%xmm0, %xmm0, %xmm0
	vmovups	%ymm0, (%rdx)
	ret
	.cfi_endproc
.LFE6475:
	.size	student_matvec_8x8_colmaj_8xfloat, .-student_matvec_8x8_colmaj_8xfloat
	.section	.rodata.str1.8
	.align 8
.LC58:
	.string	"test_matvec_8x8_colmaj_8xfloat: "
	.section	.rodata.str1.1
.LC59:
	.string	" x"
.LC60:
	.string	"yt"
.LC61:
	.string	"yr"
	.text
	.globl	test_matvec_8x8_colmaj_8xfloat
	.type	test_matvec_8x8_colmaj_8xfloat, @function
test_matvec_8x8_colmaj_8xfloat:
.LFB6476:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$392, %rsp
	.cfi_offset 3, -24
	vxorps	%xmm1, %xmm1, %xmm1
	movq	%fs:40, %rax
	movq	%rax, 376(%rsp)
	xorl	%eax, %eax
	movl	$0x41000000, 16(%rsp)
	movl	$0x40e00000, 20(%rsp)
	movl	$0x40c00000, 24(%rsp)
	movl	$0x40a00000, 28(%rsp)
	movl	$0x40800000, 32(%rsp)
	movl	$0x40400000, 36(%rsp)
	movl	$0x40000000, 40(%rsp)
	movl	$0x3f800000, 44(%rsp)
	vpxor	%xmm0, %xmm0, %xmm0
	vmovdqu	%ymm0, 48(%rsp)
	vmovdqu	%ymm0, 80(%rsp)
	leaq	112(%rsp), %rsi
	movl	$65, %ecx
.L187:
	leal	-64(%rcx), %eax
	movq	%rsi, %rdx
.L188:
	vcvtsi2ssl	%eax, %xmm1, %xmm0
	vmovss	%xmm0, (%rdx)
	addl	$8, %eax
	addq	$32, %rdx
	cmpl	%ecx, %eax
	jne	.L188
	addq	$4, %rsi
	addl	$1, %ecx
	cmpl	$73, %ecx
	jne	.L187
	leaq	48(%rsp), %rbx
	leaq	16(%rsp), %rsi
	leaq	112(%rsp), %rdi
	movq	%rbx, %rdx
	call	reference_matvec_8x8_colmaj_64xfloat
	leaq	80(%rsp), %rdx
	movq	%rbx, %rsi
	movl	$8, %edi
	call	max_pair_wise_diff
	vmovss	%xmm0, 12(%rsp)
	leaq	.LC58(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	vcvtss2sd	12(%rsp), %xmm0, %xmm0
	vcomisd	.LC18(%rip), %xmm0
	jbe	.L196
	leaq	.LC19(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	leaq	112(%rsp), %rcx
	movl	$8, %edx
	movl	$64, %esi
	leaq	.LC20(%rip), %rdi
	call	print_float_mem_as_vects
	leaq	16(%rsp), %rcx
	movl	$8, %edx
	movl	$8, %esi
	leaq	.LC59(%rip), %rdi
	call	print_float_mem_as_vects
	movq	%rbx, %rcx
	movl	$8, %edx
	movl	$8, %esi
	leaq	.LC60(%rip), %rdi
	call	print_float_mem_as_vects
	leaq	80(%rsp), %rcx
	movl	$8, %edx
	movl	$8, %esi
	leaq	.LC61(%rip), %rdi
	call	print_float_mem_as_vects
	leaq	.LC7(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
.L186:
	movq	376(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L198
	movq	-8(%rbp), %rbx
	leave
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
.L196:
	.cfi_restore_state
	leaq	.LC23(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	jmp	.L186
.L198:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE6476:
	.size	test_matvec_8x8_colmaj_8xfloat, .-test_matvec_8x8_colmaj_8xfloat
	.section	.rodata.str1.1
.LC63:
	.string	"01: "
.LC64:
	.string	"02: "
.LC65:
	.string	"03: "
.LC66:
	.string	"04: "
.LC67:
	.string	"05: "
.LC68:
	.string	"06: "
.LC69:
	.string	"07: "
.LC70:
	.string	"08: "
.LC71:
	.string	"09: "
.LC72:
	.string	"10: "
.LC73:
	.string	"11: "
	.text
	.globl	main
	.type	main, @function
main:
.LFB6477:
	.cfi_startproc
	endbr64
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	leaq	.LC63(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	movl	$0, %eax
	call	test_reverse_8xfloat
	leaq	.LC64(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	movl	$0, %eax
	call	test_rotate_by_4_8xfloat
	leaq	.LC65(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	movl	$0, %eax
	call	test_rotate_by_2_8xfloat
	leaq	.LC66(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	movl	$0, %eax
	call	test_rotate_by_1_8xfloat
	leaq	.LC67(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	movl	$0, %eax
	call	test_transpose_4x2_colmaj_8xfloat
	leaq	.LC68(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	movl	$0, %eax
	call	test_rotate_by_1_16xfloat
	leaq	.LC69(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	movl	$0, %eax
	call	test_transpose_4x4_colmaj_8xfloat
	leaq	.LC70(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	movl	$0, %eax
	call	test_transpose_8x4_colmaj_8xfloat
	leaq	.LC71(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	movl	$0, %eax
	call	test_gather_at_stride_8xfloat
	leaq	.LC72(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	movl	$0, %eax
	call	test_scatter_at_stride_8xfloat
	leaq	.LC73(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	movl	$0, %eax
	call	test_matvec_8x8_colmaj_8xfloat
	movl	$0, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE6477:
	.size	main, .-main
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC1:
	.long	2147483647
	.long	0
	.long	0
	.long	0
	.section	.rodata.cst32,"aM",@progbits,32
	.align 32
.LC8:
	.long	7
	.long	6
	.long	5
	.long	4
	.long	3
	.long	2
	.long	1
	.long	0
	.section	.rodata.cst4,"aM",@progbits,4
	.align 4
.LC16:
	.long	-1082130432
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC18:
	.long	-1598689907
	.long	1051772663
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
