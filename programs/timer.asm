; Timer Example Program
; Demonstrates memory-mapped I/O timer and Fetch/Compute/Store cycles
; This program counts from 1 to 5 with delays between each count

; Initialize counter
; FETCH: Read instruction from memory at PC (0x0100)
; DECODE: Opcode=LOADI, Rd=R0, Immediate=0
; COMPUTE: No computation needed
; STORE: Write 0 to R0
start:
    LOADI R0, 0         ; R0 = 0 (counter)

; Initialize limit
; FETCH: Read instruction from memory at PC
; DECODE: Opcode=LOADI, Rd=R1, Immediate=5
; COMPUTE: No computation needed
; STORE: Write 5 to R1
    LOADI R1, 5         ; R1 = 5 (limit)

; Main loop - count from 1 to 5
loop:
    ; Increment counter
    ; FETCH: Read INC instruction
    ; DECODE: Opcode=INC, Rd=R0
    ; COMPUTE: ALU performs R0 = R0 + 1, updates flags
    ; STORE: Write result back to R0
    INC R0              ; R0 = R0 + 1

    ; Display current count (output to console)
    ; FETCH: Read ADDI instruction
    ; DECODE: Opcode=ADDI, Rd=R2, Rs=R0, Immediate='0'
    ; COMPUTE: ALU adds ASCII '0' (48) to R0 value
    ; STORE: Write result to R2
    LOADI R2, 48        ; R2 = ASCII '0'
    
    ; FETCH: Read ADD instruction
    ; DECODE: Opcode=ADD, Rd=R2, Rs1=R2, Rs2=R0
    ; COMPUTE: ALU performs R2 + R0
    ; STORE: Write result to R2 (now contains ASCII digit)
    ADD R2, R2, R0      ; R2 = '0' + counter (ASCII digit)
    
    ; FETCH: Read STORE instruction with address [0xFF01]
    ; DECODE: Opcode=STORE, Rs=R2, Address=0xFF01
    ; COMPUTE: Calculate memory address (0xFF01)
    ; STORE: Write R2 to memory-mapped console output
    STORE R2, [0xFF01]  ; Output digit to console

    ; Output newline
    ; FETCH: Read LOADI instruction
    ; DECODE: Opcode=LOADI, Rd=R3, Immediate=10
    ; COMPUTE: No computation
    ; STORE: Write 10 to R3
    LOADI R3, 10        ; R3 = newline character
    
    ; FETCH: Read STORE instruction
    ; DECODE: Opcode=STORE, Rs=R3, Address=0xFF01
    ; COMPUTE: Calculate address
    ; STORE: Write R3 (newline) to console
    STORE R3, [0xFF01]  ; Output newline

    ; Start timer for delay (100 cycles)
    ; FETCH: Read LOADI instruction
    ; DECODE: Opcode=LOADI, Rd=R4, Immediate=100
    ; COMPUTE: No computation
    ; STORE: Write 100 to R4
    LOADI R4, 100       ; R4 = 100 (timer value)
    
    ; FETCH: Read STORE instruction to timer control
    ; DECODE: Opcode=STORE, Rs=R4, Address=0xFF00
    ; COMPUTE: Calculate address
    ; STORE: Write timer value to memory-mapped timer (starts countdown)
    STORE R4, [0xFF00]  ; Start timer

; Wait for timer to expire
wait:
    ; FETCH: Read LOAD instruction from timer value register
    ; DECODE: Opcode=LOAD, Rd=R5, Address=0xFF03
    ; COMPUTE: Calculate address
    ; STORE: Read timer value from 0xFF03, write to R5
    LOAD R5, [0xFF03]   ; R5 = current timer value
    
    ; FETCH: Read CMPI instruction
    ; DECODE: Opcode=CMPI, Rs=R5, Immediate=0
    ; COMPUTE: ALU performs R5 - 0, updates flags (Z flag if zero)
    ; STORE: Update FLAGS register only
    CMPI R5, 0          ; Compare R5 with 0
    
    ; FETCH: Read JNZ instruction
    ; DECODE: Opcode=JNZ, Address=wait
    ; COMPUTE: Check Z flag, calculate target address if not zero
    ; STORE: Update PC to 'wait' if Z flag is clear, else continue
    JNZ wait            ; Loop while timer > 0

    ; Check if counter reached limit
    ; FETCH: Read CMP instruction
    ; DECODE: Opcode=CMP, Rs1=R0, Rs2=R1
    ; COMPUTE: ALU performs R0 - R1, updates flags
    ; STORE: Update FLAGS register only
    CMP R0, R1          ; Compare counter with limit
    
    ; FETCH: Read JNZ instruction
    ; DECODE: Opcode=JNZ, Address=loop
    ; COMPUTE: Check Z flag, calculate target if not zero
    ; STORE: Update PC to 'loop' if Z flag clear (not equal)
    JNZ loop            ; Continue if not equal

; Done - output completion message
    LOADI R2, 68        ; 'D'
    STORE R2, [0xFF01]
    LOADI R2, 111       ; 'o'
    STORE R2, [0xFF01]
    LOADI R2, 110       ; 'n'
    STORE R2, [0xFF01]
    LOADI R2, 101       ; 'e'
    STORE R2, [0xFF01]
    LOADI R2, 10        ; newline
    STORE R2, [0xFF01]

; Halt the CPU
; FETCH: Read HALT instruction
; DECODE: Opcode=HALT
; COMPUTE: Set halted flag
; STORE: CPU stops execution
    HALT

