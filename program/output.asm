.code

; [0]
FUNC proc
push rbp
mov rbp, rsp

; [1]
push rcx

; [2]
push 100

; [3]
pop rbx
pop rax
add rax, rbx
push rax

; [4]
push 2

; [5]
pop rbx
pop rax
add rax, rbx
push rax

; [6]
push r8

; [7]
pop rbx
pop rax
add rax, rbx
push rax

; [8]
pop rax
pop rbp
ret

; [9]
FUNC endp
end
