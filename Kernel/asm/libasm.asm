GLOBAL cpuVendor
GLOBAL printTimeASM
GLOBAL _get

section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

_get:
	mov rbp, [rsp]
	mov rsp, rdi
	mov [rsp],rbp
	mov rbp, rsp
	ret