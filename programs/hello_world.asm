; Hello, World! Program
; Outputs "Hello, World!" to the console using memory-mapped I/O

start:
    ; Output 'H'
    LOADI R0, 72
    STORE R0, [0xFF01]

    ; Output 'e'
    LOADI R0, 101
    STORE R0, [0xFF01]

    ; Output 'l'
    LOADI R0, 108
    STORE R0, [0xFF01]

    ; Output 'l'
    LOADI R0, 108
    STORE R0, [0xFF01]

    ; Output 'o'
    LOADI R0, 111
    STORE R0, [0xFF01]

    ; Output ','
    LOADI R0, 44
    STORE R0, [0xFF01]

    ; Output ' '
    LOADI R0, 32
    STORE R0, [0xFF01]

    ; Output 'W'
    LOADI R0, 87
    STORE R0, [0xFF01]

    ; Output 'o'
    LOADI R0, 111
    STORE R0, [0xFF01]

    ; Output 'r'
    LOADI R0, 114
    STORE R0, [0xFF01]

    ; Output 'l'
    LOADI R0, 108
    STORE R0, [0xFF01]

    ; Output 'd'
    LOADI R0, 100
    STORE R0, [0xFF01]

    ; Output '!'
    LOADI R0, 33
    STORE R0, [0xFF01]

    ; Output newline
    LOADI R0, 10
    STORE R0, [0xFF01]

    ; Halt
    HALT

