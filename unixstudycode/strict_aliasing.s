	.file	"strict_aliasing.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%x\n"
	.text
	.p2align 4,,15
.globl main
	.type	main, @function
main:
.LFB13:
	subq	$24, %rsp
.LCFI0:
	movl	$305419896, %esi
	movl	$.LC0, %edi
	movzwl	22(%rsp), %eax
	movzwl	20(%rsp), %edx
	movl	$305419896, 20(%rsp)
	movw	%ax, 20(%rsp)
	xorl	%eax, %eax
	movw	%dx, 22(%rsp)
	call	printf
	xorl	%eax, %eax
	addq	$24, %rsp
	ret
.LFE13:
	.size	main, .-main
	.section	.eh_frame,"a",@progbits
.Lframe1:
	.long	.LECIE1-.LSCIE1
.LSCIE1:
	.long	0x0
	.byte	0x1
	.string	"zR"
	.uleb128 0x1
	.sleb128 -8
	.byte	0x10
	.uleb128 0x1
	.byte	0x3
	.byte	0xc
	.uleb128 0x7
	.uleb128 0x8
	.byte	0x90
	.uleb128 0x1
	.align 8
.LECIE1:
.LSFDE1:
	.long	.LEFDE1-.LASFDE1
.LASFDE1:
	.long	.LASFDE1-.Lframe1
	.long	.LFB13
	.long	.LFE13-.LFB13
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB13
	.byte	0xe
	.uleb128 0x20
	.align 8
.LEFDE1:
	.ident	"GCC: (GNU) 4.1.2 20080704 (Red Hat 4.1.2-46)"
	.section	.note.GNU-stack,"",@progbits
