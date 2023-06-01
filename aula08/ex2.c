#include <detpic32.h>

void _int_(12) isr_T3(void) // Replace VECTOR by the timer T3
{
    putChar('.');
    IFS0bits.T3IF = 0;
}

int main(void)
{
    T3CONbits.TCKPS = 7;// 1:32 prescaler (i.e. fout_presc = 625 KHz)
    PR2 = 39062;        // Fout = 20MHz / (32 * (62499 + 1)) = 10 Hz
    TMR2 = 0;           // Clear timer T2 count register

    IPC3bits.T3IP = 1; // Interrupt priority (must be in range [1..6])
    IEC0bits.T3IE = 1; // Enable timer T3 interrupts
    IFS0bits.T3IF = 0; // Reset timer T3 interrupt flag

    T3CONbits.TON = 1;  // Enable timer T2 (must be the last command of the
                        // timer configuration sequence)

    EnableInterrupts();
    while(1);
    return 0;
}
