    .equ READ_CORE_TIMER,11
    .equ RESET_CORE_TIMER,12
    .equ PRINT_INT,6
    .equ inkey,1
    .data
    .text
    .globl main
main:
    addiu $sp,$sp,-4
    sw $ra,0($sp)
while:
    li $a0,5000
    li $v0,inkey
    syscall
    beq $v0,'S',end
    move $a1,$v0
    jal timeDone
    move $a0,$v0
    li $a1,0x0004000A
    li $v0,PRINT_INT
    syscall
    li $v0,3
    li $a0,'\r'
    syscall
    j while

end:
    lw $ra,0($sp)
    addiu $sp,$sp,4
    jr $ra 


#------------------------------------------------------------------------------------------------

timeDone:
    li $t1,0 #unsigned int retValue = 0;
    ble $a1,0,else
    li $v0,RESET_CORE_TIMER
    syscall
    j return
else:
    li $v0,READ_CORE_TIMER
    syscall
    move $t0,$v0 #curCount
    mul $a0,$a0,20000
    ble $t0,$a0,return
    div $t1,$t0,20000
return:
    move $v0,$t1
    jr $ra