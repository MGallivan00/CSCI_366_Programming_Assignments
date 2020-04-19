 global  set_bit_elem
        global  get_bit_elem
        section .text

set_bit_elem:
        push rbp            ; save the base pointer on the stack (at rsp+8)
        mov rbp, rsp        ; set up the rbp for the bottom of this frame

        ; rdi contains array pointer
        ; rsi contains row width
        ; rdx contains row
        ; rcx contains col

        mov rax, rdx        ; get i (row)
        imul rax, rsi       ; compute row-width*row
        add rax, rcx        ; compute row-width*row + column
;-------------------------------------------------

        mov rcx, rax        ; index
        sar rcx, 3          ; byte_offset = rcx = index/8

        push rcx
        mov rcx, rax
        mov rbx, 1
        and rcx, 7
        shl rbx, cl

        pop rcx
        mov rax, [rdi + rcx]
        or rax, rbx         ; set the bit into (row, col)
        mov [rdi + rcx], rax

    .done:
        mov rsp, rbp        ; restore stack pointer to before we pushed parameters onto the stack
        pop rbp             ; remove rbp from the stack to restore rsp to initial value
        ret                 ; return value in rax


get_bit_elem2:
        push rbp            ; save the base pointer on the stack (at rsp+8)
        mov rbp, rsp        ; set up the rbp for the bottom of this frame
        add rdi, rbp
        mov rax, [rdi]
        setl al
        movzx rax, al
        mov rsp, rbp        ; restore stack pointer to before we pushed parameters onto the stack
        pop rbp             ; remove rbp from the stack to restore rsp to initial value
        ret                 ; return value in rax

get_bit_elem:
        push rbp            ; save the base pointer on the stack (at rsp+8)
        mov rbp, rsp        ; set up the rbp for the bottom of this frame

        ; rdi contains array pointer
        ; rsi contains row width
        ; rdx contains row
        ; rcx contains col

        mov rax, rdx        ; get i (row)
        imul rax, rsi       ; compute row-width*row
        add rax, rcx        ; compute row-width*row + column
;-------------------------------------------------

        mov rcx, rax        ; index
        sar rcx, 3          ; byte_offset = rcx = index/8

        push rax
        mov rax, [rdi + rcx]
        pop rcx

        mov rbx, 1
        and rcx, 7
        shl rbx, cl

        and rax, rbx        ; rax is 0 or not equal to 0
        test rax, rax
        je .done
        setl al
        movzx rax, al

    .done:
        mov rsp, rbp        ; restore stack pointer to before we pushed parameters onto the stack
        pop rbp             ; remove rbp from the stack to restore rsp to initial value
        ret                 ; return value in rax
