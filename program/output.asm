.code

FUNC proc

; [0]
push 100

; [1]
push 2

; [2]
pop rbx
pop rax
add rax, rbx
push rax

; [3]
pop rax
ret
FUNC endp

end
