#include <detpic32.h>

int count;
int freq = 10;

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

void _int_(8) isr_T2(void){
    send2Display(toBcd(count));
    IFS0bits.T2IF = 0;
}

int main(void){

    //timer 2
    T2CONbits.TCKPS = 3; // 1:32 prescaler (i.e Fout_presc = 625 KHz)
    PR2 = 49999; // Fout = 20MHz / (32 * (62499 + 1)) = 10 Hz
    TMR2 = 0; // Reset timer T2 count register
    T2CONbits.TON = 1; // Enable timer T2 (must be the last command of the timer configuration sequence

    // interrupion timer 2
    IPC2bits.T2IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T2IE = 1; // Enable timer T2 interrupts
    IFS0bits.T2IF = 0; // Reset timer T2 interrupt flag

    TRISBbits.TRISB4   = 1;  // RB4 digital output disconnected
    AD1PCFGbits.PCFG4  = 0;  // RB4 configured  as analog input

    AD1CON1bits.SSRC    = 7; // convertion trigger selection (this mode uses a counter to end sampling and start convertion)
    AD1CON1bits.CLRASAM = 1; // stop convertion when 1st A/D converter interrupt is generated

    AD1CON3bits.SAMC   = 16; // sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI   = 1 - 1; // 4 consecutive sample

    AD1CHSbits.CH0SA   = 4;  // set analog input channel 4 (0 to 15)
    AD1CON1bits.ON     = 1;  // enable A/D converter
    IPC6bits.AD1IP     = 1; // configure priority of A/D interrupts
    IEC1bits.AD1IE     = 1;

    EnableInterrupts();

    while(1){
        
    }
    return 0;
}
