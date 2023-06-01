.equ SFR_BASE_HI,0xBF88
    
    .equ TRISB,0x6040 # TRISB address: 0xBF886040
    .equ PORTB,0x6050 # PORTB address: 0xBF886050

    .equ TRISE,0x6100 # TRISE address: 0xBF886100
    .equ LATE, 0x6120 # LATE address: 0xBF886120
    
    .data
    .text
    .globl main

main:
    lui $t0,SFR_BASE_HI # $t0=0xBF880000
    
    #Configurar TRISB
    lw $t1,TRISB($t0) 
    ori $t1,$t1,0x000F 
    sw $t1,TRISB($t0)

    #Configurar TRISE
    lw $t1,TRISE($t0)
    andi $t1,$t1,0xFF0F 
    sw $t1,TRISE($t0)

loop:
    lw $t1,PORTB($t0)
    andi $t3,$t1,0x0001
    sll $t3,$t3,3
    andi $t4,$t1,0x0002
    sll $t4,$t4,1
    ori $t3,$t3,$t4
    andi $t4,$t1,0x0004
    srl $t4,$t4,1
    ori $t3,$t3,$t4
    andi $t4,$t1,0x0008
    srl $t4,$t4,3
    ori $t3,$t3,$t4
    sll $t3,$t3,4

    lw $t2,LATE($t0)
    andi $t2,$t2,0xFF0F
    ori $t2,$t2,$t3
    
    sw $t2,LATE($t0)
    j loop 

    jr $ra

