#PORT VAI LER o valor no INPUT PORT
#LAT VAI ESCREVER o Valor no OUTPUT PORT
    .data
    .equ SFR_BASE_HI, 0xBF880000
    .equ PORTB, 0x6050
    .equ LATE, 0x6120
    .equ TRISE, 0x6100
    .equ TRISB, 0x6040
    .text
    .globl main
main:
    addi $sp,$sp,-4
    sw $ra,0($sp)

    li $s0,SFR_BASE_HI

    lw $t1,TRISE($s0)
    andi $t1,$t1,0xFFC3
    sw $t1,TRISE($s0)

    lw $t1,TRISB($s0)
    ori $t1,$t1,0x000F
    sw $t1,TRISB($s0)


while:
    lw $t1, PORTB($s0)
    andi $t1,$t1,0x000F
    #RB3\,RB2,RB1,RB0\
    addi $t1,$t1,0x8
    andi $t1,$t1,0x000F #já negamos RB3
    andi $t3,$t1,0x1
    bgtz $t3,if
    addi $t1,$t1,1
    j cont
if:
    sub $t1,$t1,1

cont:
    lw $t2, LATE($s0)
    andi $t2,$t2,0xFFC3 #Passa todo a 0 no que importa para copiar os valors lidos no input
    sll $t3,$t1,2
    or $t2,$t3,$t2
    sw $t2,LATE($s0)
    j while
    

    lw $ra,0($sp)
    addi $sp,$sp,4
    jr $ra


