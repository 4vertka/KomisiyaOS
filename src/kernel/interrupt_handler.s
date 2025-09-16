.section .text

.global keyboard_interrupt_handler
.extern keyboard_handler

keyboard_interrupt_handler:
    pusha
    
    call keyboard_handler
    
    movb $0x20, %al
    outb %al, $0x20
    
    popa
    
    iret
