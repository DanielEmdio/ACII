#include <detpic32.h>

void _int_(4) isr_T1(void)
{
    // print character '1'
    putChar('1');
    // Reset T1IF flag
    IFS0bits.T1IF = 0;
}

void _int_(12) isr_T3(void)
{
    // print character '3'
    putChar('3');
    // Reset T3IF flag
    IFS0bits.T3IF = 0;
}

int main(void)
{
    // Configure Timers T1 and T3 with interrupts enabled)
    T1CONbits.TCKPS = 2;// 1:32 prescaler (i.e. fout_presc = 625 KHz)
    PR1 = 62499;        // Fout = 20MHz / (32 * (62499 + 1)) = 10 Hz
    TMR1 = 0;           // Clear timer T1 count register

    IPC1bits.T1IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T1IE = 1; // Enable timer T3 interrupts
    IFS0bits.T1IF = 0; // Reset timer T3 interrupt flag

    T3CONbits.TCKPS = 4;// 1:32 prescaler (i.e. fout_presc = 625 KHz)
    PR3 = 49999;        // Fout = 20MHz / (32 * (62499 + 1)) = 10 Hz
    TMR3 = 0;           // Clear timer T1 count register

    IPC3bits.T3IP = 1; // Interrupt priority (must be in range [1..6])
    IEC0bits.T3IE = 1; // Enable timer T3 interrupts
    IFS0bits.T3IF = 0; // Reset timer T3 interrupt flag



    T1CONbits.TON = 1;  // Enable timer T1 (must be the last command of the
                        // timer configuration sequence)
    T3CONbits.TON = 1; 
    // Reset T1IF and T3IF flags
    IFS0bits.T3IF = 0;
    IFS0bits.T1IF = 0;

    EnableInterrupts(); // Global Interrupt Enable
    while(1);
    return 0;
}
