; AES-256 ECB Mode Implementation using AES-NI
; -------------------------------------------
; Exports:
;   void aes256_key_expansion(const uint8_t* key, uint8_t* round_keys_out)
;   void aes256_encrypt_block(uint8_t* block, const uint8_t* round_keys)
;   void aes256_decrypt_block(uint8_t* block, const uint8_t* round_keys)

section .text
global _aes256_key_expansion
global _aes256_encrypt_block
global _aes256_decrypt_block
global _aes256_invert_keys

; Round constants for AES key expansion
section .rodata
align 16
round_constants:
    db 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    db 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    db 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    db 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    db 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    db 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    db 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

section .text
; ----------------------------------------------------------------------
; void aes256_key_expansion(const uint8_t* key, uint8_t* round_keys_out)
; rdi = input key (32 bytes)
; rsi = output buffer (240 bytes)
_aes256_key_expansion:
    push rbx
    movdqu xmm0, [rdi]           ; Load first 128 bits
    movdqu xmm1, [rdi + 16]      ; Load second 128 bits
    movdqu [rsi], xmm0           ; Store rk[0]
    movdqu [rsi + 16], xmm1      ; Store rk[1]

    mov rcx, 7                   ; 7 iterations (2 round keys per round)
    mov rdx, rsi                 ; rdx = pointer to round keys buffer
    lea rbx, [rel round_constants] ; Load address of round constants

.keyloop:
    ; First half of the round
    movdqu xmm4, [rbx]           ; Load round constant
    aeskeygenassist xmm2, xmm1, 0x1  ; Generate round key assist
    pshufd xmm2, xmm2, 0xff     ; Broadcast last dword to all positions
    pxor xmm2, xmm4             ; Apply round constant
    call expand_aes256_round
    add rdx, 32
    add rbx, 16                  ; Move to next round constant
    dec rcx
    jnz .keyloop
    pop rbx
    ret

; Expands two round keys per AES-256 round
; Inputs:
;   xmm0 = prev even round key
;   xmm1 = prev odd round key
;   xmm2 = assist value from keygenassist with round constant
; Output:
;   writes to [rdx + 32] and [rdx + 48]
expand_aes256_round:
    ; Generate even round key using temp registers to preserve values
    movdqa xmm3, xmm0           ; Save a copy of previous even round key
    pslldq xmm3, 4              ; Shift left by 4 bytes
    pxor xmm0, xmm3             ; XOR with original
    pslldq xmm3, 4              ; Shift left by 4 more bytes
    pxor xmm0, xmm3             ; XOR with original
    pslldq xmm3, 4              ; Shift left by 4 more bytes
    pxor xmm0, xmm3             ; XOR with original
    pxor xmm0, xmm2             ; XOR with round constant and rotword(subbytes())
    movdqu [rdx + 32], xmm0     ; Store new even round key

    ; Generate odd round key
    aeskeygenassist xmm2, xmm0, 0x00  ; Get assist from new even round key
    pshufd xmm2, xmm2, 0xaa     ; Broadcast third dword to all positions
    movdqa xmm3, xmm1           ; Save a copy of previous odd round key
    pslldq xmm3, 4              ; Shift left by 4 bytes
    pxor xmm1, xmm3             ; XOR with original
    pslldq xmm3, 4              ; Shift left by 4 more bytes
    pxor xmm1, xmm3             ; XOR with original
    pslldq xmm3, 4              ; Shift left by 4 more bytes
    pxor xmm1, xmm3             ; XOR with original
    pxor xmm1, xmm2             ; XOR with transform of new even key
    movdqu [rdx + 48], xmm1     ; Store new odd round key
    ret

; ----------------------------------------------------------------------
; void aes256_encrypt_block(uint8_t* block, const uint8_t* round_keys)
; rdi = pointer to 16-byte block
; rsi = pointer to 240-byte round keys
_aes256_encrypt_block:
    push rbx
    
    ; Load plaintext and first round key
    movdqu xmm0, [rdi]           ; Load input block
    movdqu xmm1, [rsi]           ; Load first round key
    pxor xmm0, xmm1             ; Initial AddRoundKey
    
    ; Do 13 full rounds
    mov rbx, 16                  ; Start with round 1 key offset
.encrypt_loop:
    movdqu xmm1, [rsi + rbx]    ; Load round key
    aesenc xmm0, xmm1           ; Do the round
    add rbx, 16                 ; Move to next round key
    cmp rbx, 224               ; Check if we've done 13 rounds
    jne .encrypt_loop
    
    ; Final round
    movdqu xmm1, [rsi + 14*16]   ; Load last round key
    aesenclast xmm0, xmm1       ; Do final round
    movdqu [rdi], xmm0          ; Store result
    
    pop rbx
    ret

; ----------------------------------------------------------------------
; void aes256_decrypt_block(uint8_t* block, const uint8_t* round_keys)
; rdi = pointer to 16-byte block
; rsi = pointer to 240-byte round keys
_aes256_decrypt_block:
    push rbx
    
    ; Load ciphertext and first round key
    movdqu xmm0, [rdi]           ; Load input block
    movdqu xmm1, [rsi]           ; Load first round key (inverted)
    pxor xmm0, xmm1             ; Initial AddRoundKey
    
    ; Do 13 full rounds
    mov rbx, 16                  ; Start with round 1 key offset
.decrypt_loop:
    movdqu xmm1, [rsi + rbx]    ; Load round key
    aesdec xmm0, xmm1           ; Do the round
    add rbx, 16                 ; Move to next round key
    cmp rbx, 224               ; Check if we've done 13 rounds
    jne .decrypt_loop
    
    ; Final round
    movdqu xmm1, [rsi + 14*16]   ; Load last round key
    aesdeclast xmm0, xmm1       ; Do final round
    movdqu [rdi], xmm0          ; Store result
    
    pop rbx
    ret

_aes256_invert_keys:
    push rbx
    push r12
    push r13
    
    ; Save parameters
    mov rbx, rdi    ; dec_keys
    mov r12, rsi    ; enc_keys
    
    ; Copy first and last round keys without transformation
    movdqu xmm0, [r12]              ; First encryption round key
    movdqu [rbx + 224], xmm0        ; Becomes last decryption round key
    
    movdqu xmm0, [r12 + 224]        ; Last encryption round key
    movdqu [rbx], xmm0              ; Becomes first decryption round key
    
    ; Transform and reverse the middle round keys
    mov rcx, 13                     ; Number of middle rounds
    mov r13, 208                    ; Start offset for decryption keys (224 - 16)
    mov rdi, 16                     ; Start offset for encryption keys
    
.invert_loop:
    movdqu xmm0, [r12 + rdi]        ; Load encryption round key
    aesimc xmm0, xmm0               ; Inverse mix columns
    movdqu [rbx + r13], xmm0        ; Store in reverse order
    add rdi, 16                     ; Next encryption key
    sub r13, 16                     ; Previous position in decryption schedule
    dec rcx
    jnz .invert_loop
    
    pop r13
    pop r12
    pop rbx
    ret
