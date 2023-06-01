    .equ READ_CORE_TIMER,11
    .equ RESET_CORE_TIMER,12
    .equ PRINT_INT,6
    .equ inkey,1
    .data
STR:.asciiz "Done"
    .text
    .globl main
main: 
    addiu $sp,$sp,-4
    sw $ra,0($sp)
    li $a1,0x0004000A
    li $t1,1
wh:
    li $v0,3
    li $a0,'\r'
    syscall

    li $v0,inkey
    syscall

    beq $v0,'S',stop
rstart:
    bne $v0,'A',nodub
    mul $t1,$t1,2

nodub:
    bne $v0,'N',NRstar
    li $t1,1

NRstar:
    move $a0,$t1
    jal delay

    li $v0,PRINT_INT
    move $a0,$s0
    syscall
    li $v0,3
    li $a0,' '
    syscall
    addiu $s0,$s0,1

    rem $t0,$s0,2
    beq $t0,0,add2

cont:
    rem $t0,$s0,10
    beq $t0,0,add10

    j wh
    

add2:
    addiu $s1,$s1,1
    li $v0,PRINT_INT
    move $a0,$s1
    syscall
    li $v0,3
    li $a0,' '
    syscall
    j cont
add10:
    addiu $s2,$s2,1
    li $v0,PRINT_INT
    move $a0,$s2
    syscall
    li $v0,3
    li $a0,' '
    syscall
    j wh

stop:
    li $v0,inkey
    syscall
    beq $v0,'R',rstart
    j stop

end:
    lw $ra,0($sp)
    addiu $sp,$sp,4

    jr $ra 







#------------------------------------------------------------------------------------------------

delay: 
    mul $t0,$a0,2000000
    li $v0,RESET_CORE_TIMER
    syscall
while:
    li $v0,READ_CORE_TIMER
    syscall
    blt $v0,$t0,while
    jr $ra #