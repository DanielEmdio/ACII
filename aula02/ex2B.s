    .equ READ_CORE_TIMER,11
    .equ RESET_CORE_TIMER,12
    .data
STR:.asciiz "Done"
    .text
    .globl main
main: 
    addiu $sp,$sp,-4
    sw $ra,0($sp)

    li $a0,1000
    jal delay
    li $v0,8
    la $a0,STR
    syscall

    lw $ra,0($sp)
    addiu $sp,$sp,4

    jr $ra 

delay: 
    mul $t0,$a0,20000
    li $v0,RESET_CORE_TIMER
    syscall
while:
    li $v0,READ_CORE_TIMER
    syscall
    blt $v0,$t0,while
    jr $ra #
