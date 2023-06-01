/* int main(void)
{
int value;
while(1)
{
printStr("\nIntroduza um inteiro (sinal e módulo): ");
value = readInt10();
printStr("\nValor em base 10 (signed): ");
printInt10(value);
printStr("\nValor em base 2: ");
printInt(value, 2);
printStr("\nValor em base 16: ");
printInt(value, 16);
printStr("\nValor em base 10 (unsigned): ");
printInt(value, 10);
printStr("\nValor em base 10 (unsigned), formatado: ");
printInt(value, 10 | 5 << 16); // ver nota de rodapé 3
}
return 0;
} */ 

    .equ printStr,8
    .equ readInt10,5
    .equ printInt,6
    .data
str1:.asciiz "\nIntroduza um inteiro (sinal e módulo): "
str2:.asciiz "\nValor em base 10 (signed): "
str3:.asciiz "\nValor em base 2: "
str4:.asciiz "\nValor em base 16: "
str5:.asciiz "\nValor em base 10 (unsigned):  "
str6:.asciiz "\nValor em base 10 (unsigned), formatado: "
    .text
    .globl main

main: 
   la $a0,str1
   li $v0,8
   syscall
   li $v0,5
   syscall
   move $t0,$v0
   li $v0,8
   la $a0,str2
   syscall
    