org 0x7C00
bits 16

%define ENDL 0x0A

;
; FAT12 HEADER
; https://www.youtube.com/watch?v=srbnMNk7K7k 
;

jmp short start
nop

bdb_oem:                        db "MSWIN4.1" ; 8 bytes
bdb_bytes_per_sector:           dw 512 ; bytes per sector
bdb_sectors_per_cluster:        db 1 ; sectors per cluster
bdb_reserved_sectors:           dw 1 ; reserved sectors
dbb_fat_count:                  db 2 ; number of FATs
bdb_dir_entries:                dw 0E0h ; number of directory entries
bdb_total_sectors:              dw 2880 ; total number of sectors    2880 *512 = 1.44MB
bdb_media_descriptor:           db 0F0h ; media descriptor
bdb_sectors_per_fat:            dw 9 ; sectors per FAT
bdb_sectors_per_track:          dw 18 ; sectors per track
bdb_heads:                      dw 2 ; number of heads
bdb_hidden_sectors:             dd 0 ; number of hidden sectors
bdb_large_sectors_count:        dd 0 ; number of large sectors

; extended boot record

ebr_drive_number:               db 0 ; drive number
ebr_reserved:                   db 0 ; reserved
ebr_ext_boot_signature:         db 29h ; extended boot signature
ebr_volume_id:                  dd 12h, 34h 56h, 78h ; volume id

ebr_volume_label:               db "CHIP8OS    " ; volume label
ebr_file_system_type:           db "FAT12   " ; file system type




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
