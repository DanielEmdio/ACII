    .equ SFR_BASE_HI,0xBF88
    
    .equ TRISE,0x6100 # TRISE address: 0xBF886100
    .equ LATE, 0x6120 # LATE address: 0xBF886120
    
    .data
    .text
    .globl main

main:
    lui $t0,SFR_BASE_HI # $t0=0xBF880000
    
    #Configurar TRISE
    lw $t1,TRISE($t0)
    andi $t1,$t1,0xFFF0 
    sw $t1,TRISE($t0)
    li $t4,1

loop:
    li $v0,1 #inkey
    syscall 
    
    beqz $v0,loop

    blt $v0,0x30,one4all
    bgt $v0,0x33,one4all

    lw $t2,LATE($t0)
    andi $t2,$t2,0xFFF0
    andi $v0,$v0,0xF
    sll $v0,$t4,$v0 # não sei se é valido :(
    or $t2,$t2,$v0
    sw $t2,LATE($t0)
    j loop 

one4all:
    lw $t2,LATE($t0)
    ori $t2,$t2,0xF
    sw $t2,LATE($t0)
    li $a0,1000
    jal delay
    andi $t2,$t2,0xFFF0
    sw $t2,LATE($t0)
    j loop 



#------------------------------------------------

delay: 
    mul $t3,$a0,20000
    li $v0,12
    syscall
while:
    li $v0,11
    syscall
    blt $v0,$t3,while
    jr $ra #