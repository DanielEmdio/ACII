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
    andi $t1,$t1,0xFFF0 
    sw $t1,TRISE($t0)

loop:
    lw $t1,PORTB($t0)
    andi $t1,$t1,0x000F
    lw $t2,LATE($t0)
    andi $t2,$t2,0xFFF0
    or $t2,$t2,$t1
    sw $t2,LATE($t0)
    j loop 

    jr $ra

