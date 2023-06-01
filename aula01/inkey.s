# int main(void)
# {
# char c;
# do {
# c = inkey();
# if( c != 0 )
# putChar( c );
# else
# putChar('.');
# } while( c != '\n' );
# return 0;
# }

    .equ inkey,1
    .equ putChar,3
    .data
    .text
    .globl main

main: 
    li $v0,1
    syscall
    beq $v0,0,else
    move $a0,$v0
    li $v0,putChar
    syscall
    j while
else:
    li $a0,'.'
    li $v0,putChar
    syscall
while:
    beq $a0,'\n',end
    j main
end:
    li $v0,0
    jr $ra