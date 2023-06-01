    .data
    .equ SFR_BASE_HI, 0xBF880000
    .equ PORTD, 0x60D0
    .equ LATE, 0x6120
    .equ TRISD, 0x60C0
    .equ TRISE, 0x6100
    .text
    .globl main
main:
    
    li $t0,SFR_BASE_HI

    lw $t1,TRISE($t0)
    andi $t1,$t1,0xFFFE
    sw $t1,TRISE($t0)

    lw $t1,TRISD($t0)
    ori $t1,$t1,0x0100
    sw $t1,TRISD($t0)


while:
    lw $t1, PORTD($t0)
    andi $t1,$t1,0x0100
    srl $t1,$t1,0x8
    lw $t2, LATE($t0)
    andi $t2,$t2,0xFFFE
    or $t2,$t1,$t2
    sw $t2,LATE($t0)
    j while
    
    jr $ra
