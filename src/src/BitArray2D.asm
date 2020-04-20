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

        ; basically the array is 8 times more than we expect from the rows since it holds bytes (8 bytes = 1 bit)
        ; so we grab the byte and set the bit to 1
        ; index = 1/8(row * row width + col)
        ; byte_offset = index / 8
        ; bit off_set = index - (byte_offset * 8)
        ; mask = 1 << (8 - bit_offset - 1)


        mov rax, rdx        ; row
        imul rax, rsi       ; width
        add rax, rcx        ; column

        mov rcx, rax        ; copy index into rcx
        sar rcx, 3          ; byte_offset = index/8

        push rcx            ; save the byte_offset to the stack
        mov rcx, rax        ; restore rcx to the index
        mov rbx, 1          ; move 1 into a new register (prep the bit mask)
        and rcx, 7          ; grab the last 3 bits (remainder from byte_offset division)
                            ; these last 3 will have 1's (111 = 7) so anding will put the
                            ; remainder into rcx
        shl rbx, cl         ; shift the 1 by the remainder (in the upper register of rcx)
                            ; positions the bit mask

        pop rcx             ; pop the index into rcx
        ; or rbx, [rdi + rcx]        ; grab proper bit from memory and set to 1
        ; mov [rdi + rcx], rax       ; return the bit into memory

        setl al             ; creates boolean
        movzx rax, al       ; moves boolean into returned values

        mov rsp, rbp        ; restore stack pointer to before we pushed parameters onto the stack
        pop rbp             ; remove rbp from the stack to restore rsp to initial value
        ret                 ; return value in rax


get_bit_elem:
        push rbp            ; save the base pointer on the stack (at rsp+8)
        mov rbp, rsp        ; set up the rbp for the bottom of this frame
       ; add rdi, rbp
       ; mov rax, [rdi]     ; breaking with exit code 139 when above line is removed
        setl al             ; this is a test driver to see why rdi was breaking
        movzx rax, al
        mov rsp, rbp        ; restore stack pointer to before we pushed parameters onto the stack
        pop rbp             ; remove rbp from the stack to restore rsp to initial value
        ret                 ; return value in rax

get_bit_elem2:
        push rbp            ; save the base pointer on the stack (at rsp+8)
        mov rbp, rsp        ; set up the rbp for the bottom of this frame

        ; rdi contains array pointer
        ; rsi contains row width
        ; rdx contains row
        ; rcx contains col

        mov rax, rdx        ; this is a copy of set_bit_element
        imul rax, rsi
        add rax, rcx

        mov rcx, rax
        sar rcx, 3

       ;push rax
       ;mov rax, [rdi + rcx]    ; this grabs the element from memory
       ;pop rcx

        mov rbx, 1          ; this is a copy of set_bit_element
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
