#PORT VAI LER o valor no INPUT PORT
#LAT VAI ESCREVER o Valor no OUTPUT PORT
    .data
    .equ SFR_BASE_HI, 0xBF880000
    .equ PORTB, 0x6050
    .equ LATE, 0x6120
    .equ TRISE, 0x6100
    .equ TRISB, 0x6040
    .equ READ_CORE_TIMER,11
    .equ RESET_CORE_TIMER,12
    .text
    .globl main
main:
    addi $sp,$sp,-4
    sw $ra,0($sp)
    li $a0,1000

    li $s0,SFR_BASE_HI

    lw $t1,TRISE($s0)
    andi $t1,$t1,0xFFE1
    sw $t1,TRISE($s0)

    lw $t1,TRISB($s0)
    ori $t1,$t1,0x0001
    sw $t1,TRISB($s0)


while:
    lw $t1, PORTB($s0)
    andi $t1,$t1,0x0001
    add $t3,$t3,$t1 #COUNTER


    lw $t2, LATE($s0)
    andi $t2,$t2,0xFFE1 #Passa todo a 0 no que importa para copiar os valors lidos no input
    sll $t4,$t3,1
    or $t2,$t4,$t2
    #call delay

    jal delay

    sw $t2,LATE($s0)
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