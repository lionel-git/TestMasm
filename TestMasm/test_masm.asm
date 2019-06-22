PUBLIC add_int_asm
PUBLIC strlen_asm
PUBLIC test1_asm
PUBLIC test2_asm
PUBLIC test_crc_asm

_DATA SEGMENT	
	myval db 'a'
_DATA ENDS

_TEXT SEGMENT

add_int_asm PROC
    push rbp ; Push stack pointer onto stack
    mov rbp, rsp ; Set base pointer to stack pointer

    add rdx, rcx ; Add first two args, store result in rdx
    mov rax, rdx ; Move result from rdx into rax

    mov rsp, rbp ; Restore stack pointer
    pop rbp ; Restore base pointer from stack

    ret
add_int_asm ENDP

strlen_asm PROC
    push rbp ; Push stack pointer onto stack
    mov rbp, rsp ; Set base pointer to stack pointer

    xor rax, rax ; Set rax to 0
    strlen_loop:
    cmp byte ptr [rcx], 0 ; Check for null byte
    je strlen_exit ; Exit loop if this is null byte
    inc rax ; Add to current count
    inc rcx ; Next byte
    jmp strlen_loop ; Next character

    strlen_exit:
    mov rsp, rbp ; Restore stack pointer
    pop rbp ; Restore base pointer from stack

    ret
strlen_asm ENDP

; rcx = input ? 
; rax = output
test1_asm PROC
    push rbp ; Push stack pointer onto stack
    mov rbp, rsp ; Set base pointer to stack pointer

	mov rax, rcx
	inc rax

    mov rsp, rbp ; Restore stack pointer
    pop rbp ; Restore base pointer from stack
	ret
test1_asm ENDP

rdtsc_asm PROC
	rdtsc
	shl rdx, 32
	or rax,rdx
	ret
rdtsc_asm ENDP

; rcx = input ? 
; rax = output
test2_asm PROC
	xor rax, rax
	
	;MOVNTDQA xmm1, xmm2
	vmovdqu xmm0, xmmword ptr[rdx]
	vpmovsxbd ymm0, xmm0

	aesenc xmm1, xmm2
	
	rdrand rax

	; rdseed rax ; not supported
	;VPCOMPRESSB xmm1, xmm2 ; avx-512 test


	ret
test2_asm ENDP

; rcx = input
test_crc_asm PROC
	xor eax, eax
	CRC32 eax, myval
	
	xor eax, eax
	CRC32 eax, byte ptr [rcx + 0] ; 1
	CRC32 eax, byte ptr [rcx + 1]
	CRC32 eax, byte ptr [rcx + 2]
	CRC32 eax, byte ptr [rcx + 3]
	CRC32 eax, byte ptr [rcx + 4]
	CRC32 eax, byte ptr [rcx + 5]
	CRC32 eax, byte ptr [rcx + 6]
	CRC32 eax, byte ptr [rcx + 7]
	CRC32 eax, byte ptr [rcx + 8] ; 9
	ret
test_crc_asm ENDP

_TEXT ENDS

END
