#PORT VAI LER o valor no INPUT PORT
#LAT VAI ESCREVER o Valor no OUTPUT PORT
    .data
    .equ SFR_BASE_HI, 0xBF880000
    .equ LATE, 0x6120
    .equ TRISE, 0x6100
    .equ LATD, 0x60E0
    .equ TRISD, 0x60C0
    .equ READ_CORE_TIMER,11
    .equ RESET_CORE_TIMER,12
    .text
    .globl main
main:
    addi $sp,$sp,-4
    sw $ra,0($sp)
    li $a0,500
    li $t3,0 #val = 0

    li $s0,SFR_BASE_HI

    lw $t1,TRISE($s0)
    andi $t1,$t1,0xFFFE
    sw $t1,TRISE($s0)

    lw $t1,TRISD($s0)
    andi $t1,$t1,0xFFFE
    sw $t1,TRISD($s0)

while:
    lw $t2,LATE($s0)
    andi $t2,$t2,0xFFFE #Passa todo a 0 no que importa para copiar os valors lidos no input
    or $t2,$t3,$t2
    sw $t2,LATE($s0)

    lw $t2,LATD($s0)
    andi $t2,$t2,0xFFFE #Passa todo a 0 no que importa para copiar os valors lidos no input
    or $t2,$t3,$t2
    sw $t2,LATD($s0)

    jal delay
    xor $t3,$t3,1
    j while
    


    lw $ra,0($sp)
    addi $sp,$sp,4
    jr $ra



# ---------------------------------------------------

delay: 
    mul $t0,$a0,20000
    li $v0,RESET_CORE_TIMER
    syscall
wh:
    li $v0,READ_CORE_TIMER
    syscall
    blt $v0,$t0,wh
    jr $ra #