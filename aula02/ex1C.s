    .equ READ_CORE_TIMER,11
    .equ RESET_CORE_TIMER,12
    .equ PUT_CHAR,3
    .equ PRINT_INT,6
    .data
    .text
    .globl main
main: 
    li $t0,0 # counter=0
    li $a1,0x0004000A
while: # while (1) {
    li $v0,RESET_CORE_TIMER #
    syscall # resetCoreTimer()
read:
    li $v0,READ_CORE_TIMER
    syscall
    bge $v0,2000000,while
    move $a0,$t0
    li $v0,PRINT_INT
    syscall
    addi $t0,$t0,1
    li $v0,PUT_CHAR
    li $a0,'\r'
    syscall
    j read
endwhile:
    jr $ra #