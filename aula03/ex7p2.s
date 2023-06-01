#PORT VAI LER o valor no INPUT PORT
#LAT VAI ESCREVER o Valor no OUTPUT PORT
# E = EXIT
# B = Batata

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
    li $a0,1500 #1,5 secs
    

    li $s0,SFR_BASE_HI #endereço base

    lw $t1,TRISE($s0) #define os ports de output com 0
    andi $t1,$t1,0xFFE1 #define os ports de output com 0, neste caso queremos ter a certeza que só  os leds 1 2 3 e 4 estão a 0
    sw $t1,TRISE($s0) #define os ports de output com 0

    lw $t1,TRISB($s0) #define o port de input com 1
    ori $t1,$t1,0x0002 #define os ports de input com 1, neste caso garantimos que o segundo switch tem 1 
    sw $t1,TRISB($s0) #define o port de input com 1

while:
    lw $t1, PORTB($s0) # lê o que está no port B
    andi $t1,$t1,0x0002 #nos só queremos o segundo switch então vamos garantir que os outros todos devolvem 0.
    andi $t3,$t3,0xF

    bgtz $t1,left #esquerda ou direita
right:
    andi $t4,$t3,0x1
    bgtz $t4,zeroTime
    srl $t3,$t3,1
    addi $t3,$t3,0x8
    j cont
zeroTime:
    srl $t3,$t3,1
    j cont 

left:
    bgt $t3,0x7,zeroTimebutLeft
    sll $t3,$t3,1
    addi $t3,$t3,1
    j cont
zeroTimebutLeft:
    sll $t3,$t3,1
    
cont:
    lw $t2, LATE($s0) # vamos colocar o valor do anel nos 4 leds
    andi $t2,$t2,0xFFE1 #Passa todo a 0 no que importa para copiar corretamente os valors lidos no input
    sll $t4,$t3,1
    or $t2,$t4,$t2 # agora os leds têm o valor do anel
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