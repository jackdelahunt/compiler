.code

; [0]
FUNC proc
    push rbp
    mov rbp, rsp

; [1]
    push rcx

; [2]
    push 200

; [3]
    pop rax
    pop rbx
    cmp rax, rbx
    mov rax, 0
    setz al
    push rax

; [4]
    pop rbx
    cmp rbx, 0
    je label_if

; [5]
    push 101

; [6]
    pop rax
    pop rbp
    ret

; [7]
label_if:

; [8]
    push 0

; [9]
    pop rax
    pop rbp
    ret

; [10]
FUNC endp
end
