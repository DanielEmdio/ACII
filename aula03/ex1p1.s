    .data
    .equ SFR_BASE_HI, 0xBF880000
    .equ PORTB, 0x6050
    .equ LATE, 0x6120
    .equ TRISE, 0x6100
    .equ TRISB, 0x6040
    .text
    .globl main
main:
    
    li $t0,SFR_BASE_HI

    lw $t1,TRISE($t0)
    andi $t1,$t1,0xFFFE
    sw $t1,TRISE($t0)

    lw $t1,TRISB($t0)
    ori $t1,$t1,0x0001
    sw $t1,TRISB($t0)


while:
    lw $t1, PORTB($t0)
    andi $t1,$t1,0x0001
    lw $t2, LATE($t0)
    andi $t2,$t2,0xFFFE
    or $t2,$t1,$t2
    sw $t2,LATE($t0)
    j while
    
    jr $ra
