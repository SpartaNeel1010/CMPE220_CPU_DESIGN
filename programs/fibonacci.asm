; Fibonacci Sequence Program
; Calculates and displays the first 10 Fibonacci numbers
; Due to 8-bit limitations, we output a marker for numbers >= 10

start:
    ; Initialize Fibonacci sequence
    LOADI R0, 0         ; R0 = fib(n-2) = 0
    LOADI R1, 1         ; R1 = fib(n-1) = 1  
    LOADI R3, 10        ; R3 = counter (we want 10 numbers)
    LOADI R4, 0         ; R4 = current iteration count

    ; Output message "Fib: "
    LOADI R5, 70        ; 'F'
    STORE R5, [0xFF01]
    LOADI R5, 105       ; 'i'
    STORE R5, [0xFF01]
    LOADI R5, 98        ; 'b'
    STORE R5, [0xFF01]
    LOADI R5, 58        ; ':'
    STORE R5, [0xFF01]
    LOADI R5, 32        ; ' '
    STORE R5, [0xFF01]

    ; Output first number (0)
    LOADI R5, 48        ; ASCII '0'
    STORE R5, [0xFF01]
    LOADI R5, 32        ; ' '
    STORE R5, [0xFF01]
    
    ; Increment counter
    INC R4

; Main Fibonacci loop
fib_loop:
    ; Check if we've output enough numbers
    CMP R4, R3          ; Compare count with limit
    JZ done             ; If equal, we're done

    ; Check if R1 < 10 for single digit output
    CMPI R1, 10
    JC single_digit     ; If R1 < 10, output single digit
    
    ; For numbers >= 10, output "##"
    LOADI R5, 35        ; '#'
    STORE R5, [0xFF01]
    LOADI R5, 35        ; '#'
    STORE R5, [0xFF01]
    JMP output_done

single_digit:
    ; Convert to ASCII digit (add 48)
    LOADI R5, 48        ; ASCII '0'
    ADD R5, R5, R1      ; R5 = '0' + R1
    STORE R5, [0xFF01]  ; Output digit

output_done:
    ; Output space separator
    LOADI R5, 32        ; ' '
    STORE R5, [0xFF01]

    ; Calculate next Fibonacci: R2 = R0 + R1
    ADD R2, R0, R1      ; R2 = R0 + R1
    
    ; Update for next iteration: R0 = R1, R1 = R2
    LOADI R0, 0         ; Clear R0
    ADD R0, R0, R1      ; R0 = R1 (old fib(n-1))
    LOADI R1, 0         ; Clear R1  
    ADD R1, R1, R2      ; R1 = R2 (new fib(n))

    ; Increment counter
    INC R4              ; R4++

    ; Loop
    JMP fib_loop

done:
    ; Output newline
    LOADI R5, 10
    STORE R5, [0xFF01]

    ; Output completion message "Done!"
    LOADI R5, 68        ; 'D'
    STORE R5, [0xFF01]
    LOADI R5, 111       ; 'o'
    STORE R5, [0xFF01]
    LOADI R5, 110       ; 'n'
    STORE R5, [0xFF01]
    LOADI R5, 101       ; 'e'
    STORE R5, [0xFF01]
    LOADI R5, 33        ; '!'
    STORE R5, [0xFF01]
    LOADI R5, 10        ; newline
    STORE R5, [0xFF01]

    ; Halt
    HALT
