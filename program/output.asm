EXTERN putchar:PROC
.code

; [0]
FUNC proc
    push rbp
    mov rbp, rsp

; [1]
    push 32

; [2]
    pop rcx
    call putchar
    mov rcx, 10
    call putchar

; [3]
    push 100

; [4]
    pop rax
    pop rbp
    ret

; [5]
FUNC endp
end
