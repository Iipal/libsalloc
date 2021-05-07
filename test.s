	.text
	.intel_syntax noprefix
	.file	"test.c"
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3                               # -- Begin function main
.LCPI0_0:
	.quad	42                              # 0x2a
	.text
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	mov	ecx, offset main.buffer+272
	cmp	rcx, offset main.buffer+4096
	jbe	.LBB0_2
# %bb.1:
	xor	edx, edx
	jmp	.LBB0_3
.LBB0_2:
	mov	edx, offset main.buffer
	mov	rax, qword ptr [rip + main.buffer]
	and	eax, 7
	lea	rsi, [rax + 256]
	mov	qword ptr [rip + main.buffer], rsi
	mov	rsi, qword ptr [rip + main.buffer+264]
	and	esi, 7
	lea	rdi, [rsi + 256]
	mov	qword ptr [rip + main.buffer+264], rdi
	or	al, 1
	mov	byte ptr [rip + main.buffer], al
	or	sil, 1
	mov	byte ptr [rip + main.buffer+264], sil
.LBB0_3:
	add	rdx, 8
	xor	eax, eax
	cmp	rcx, offset main.buffer+4096
	cmovbe	rax, rdx
	vbroadcastsd	ymm0, qword ptr [rip + .LCPI0_0] # ymm0 = [42,42,42,42]
	vmovups	ymmword ptr [rax + 2016], ymm0
	vmovups	ymmword ptr [rax + 1984], ymm0
	vmovups	ymmword ptr [rax + 1952], ymm0
	vmovups	ymmword ptr [rax + 1920], ymm0
	vmovups	ymmword ptr [rax + 1888], ymm0
	vmovups	ymmword ptr [rax + 1856], ymm0
	vmovups	ymmword ptr [rax + 1824], ymm0
	vmovups	ymmword ptr [rax + 1792], ymm0
	vmovups	ymmword ptr [rax + 1760], ymm0
	vmovups	ymmword ptr [rax + 1728], ymm0
	vmovups	ymmword ptr [rax + 1696], ymm0
	vmovups	ymmword ptr [rax + 1664], ymm0
	vmovups	ymmword ptr [rax + 1632], ymm0
	vmovups	ymmword ptr [rax + 1600], ymm0
	vmovups	ymmword ptr [rax + 1568], ymm0
	vmovups	ymmword ptr [rax + 1536], ymm0
	vmovups	ymmword ptr [rax + 1504], ymm0
	vmovups	ymmword ptr [rax + 1472], ymm0
	vmovups	ymmword ptr [rax + 1440], ymm0
	vmovups	ymmword ptr [rax + 1408], ymm0
	vmovups	ymmword ptr [rax + 1376], ymm0
	vmovups	ymmword ptr [rax + 1344], ymm0
	vmovups	ymmword ptr [rax + 1312], ymm0
	vmovups	ymmword ptr [rax + 1280], ymm0
	vmovups	ymmword ptr [rax + 1248], ymm0
	vmovups	ymmword ptr [rax + 1216], ymm0
	vmovups	ymmword ptr [rax + 1184], ymm0
	vmovups	ymmword ptr [rax + 1152], ymm0
	vmovups	ymmword ptr [rax + 1120], ymm0
	vmovups	ymmword ptr [rax + 1088], ymm0
	vmovups	ymmword ptr [rax + 1056], ymm0
	vmovups	ymmword ptr [rax + 1024], ymm0
	vmovups	ymmword ptr [rax + 992], ymm0
	vmovups	ymmword ptr [rax + 960], ymm0
	vmovups	ymmword ptr [rax + 928], ymm0
	vmovups	ymmword ptr [rax + 896], ymm0
	vmovups	ymmword ptr [rax + 864], ymm0
	vmovups	ymmword ptr [rax + 832], ymm0
	vmovups	ymmword ptr [rax + 800], ymm0
	vmovups	ymmword ptr [rax + 768], ymm0
	vmovups	ymmword ptr [rax + 736], ymm0
	vmovups	ymmword ptr [rax + 704], ymm0
	vmovups	ymmword ptr [rax + 672], ymm0
	vmovups	ymmword ptr [rax + 640], ymm0
	vmovups	ymmword ptr [rax + 608], ymm0
	vmovups	ymmword ptr [rax + 576], ymm0
	vmovups	ymmword ptr [rax + 544], ymm0
	vmovups	ymmword ptr [rax + 512], ymm0
	vmovups	ymmword ptr [rax + 480], ymm0
	vmovups	ymmword ptr [rax + 448], ymm0
	vmovups	ymmword ptr [rax + 416], ymm0
	vmovups	ymmword ptr [rax + 384], ymm0
	vmovups	ymmword ptr [rax + 352], ymm0
	vmovups	ymmword ptr [rax + 320], ymm0
	vmovups	ymmword ptr [rax + 288], ymm0
	vmovups	ymmword ptr [rax + 256], ymm0
	vmovups	ymmword ptr [rax + 224], ymm0
	vmovups	ymmword ptr [rax + 192], ymm0
	vmovups	ymmword ptr [rax + 160], ymm0
	vmovups	ymmword ptr [rax + 128], ymm0
	vmovups	ymmword ptr [rax + 96], ymm0
	vmovups	ymmword ptr [rax + 64], ymm0
	vmovups	ymmword ptr [rax + 32], ymm0
	vmovups	ymmword ptr [rax], ymm0
	xor	eax, eax
	vzeroupper
	ret
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.type	main.buffer,@object             # @main.buffer
	.local	main.buffer
	.comm	main.buffer,4096,16
	.ident	"Ubuntu clang version 12.0.0-1~oibaf~f"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym main.buffer
