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
    li $s0,0
    li $s1,0
    li $s2,0
    li $t2,0
while:
    li $a0,100
    li $a1,0
    beq $t2,0,if1
    li $a1,1
if1:
    jal timeDone
    beq $v0,0,if2
    li $t2,1 # flag para saber que já passou 100 ms
    li $a1,0x0004000A
    addi $s0,$s0,1
    li $v0,PRINT_INT
    move $a0,$s0
    syscall
    li $v0,3
    li $a0,' '
    syscall
    rem $t0,$s0,2
    beq $t0,0,add2

cont:
    rem $t0,$s0,10
    beq $t0,0,add10

    j endwhile
    

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
    li $a0,'\n'
    syscall
    j endwhile

if2:
    li $t2,0
endwhile:
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