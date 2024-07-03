org 0x7C00
bits 16

%define ENDL 0x0A

start:
    jmp main
;
; Print a character to the screen
; Params:
;   -  ds:si = pointer to string

puts:
    ; svae registers we will modify
    push si
    push ax    

.loop:
    lodsb        ; load next character in al
    or al, al    ; check if null terminator
    jz .done     ; if so, we're done

    mov ah, 0x0E    ; call bios interupt, tty mode INT 0x10
    int 0x10        ; print character in al

    jmp .loop

.done
    pop ax ; pop registers we modified in reverse order
    pop si
    ret


main:
    ; setup data segment
    mov ax, 0x0000 ; can't write to ds/es directly

    mov ds, ax
    mov es, ax

    ; setup stack
    mov ss, ax
    mov sp, 0x7C00 ; stack grows down from where bootloader is loaded

    ; print msg

    mov si, msg_hello
    call puts

    hlt

.halt
    jmp .halt


msg_hello:
    db "Hello, World!", ENDL, 0

times 510-($-$$) db 0
dw 0xAA55
