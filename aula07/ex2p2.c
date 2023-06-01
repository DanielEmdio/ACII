#include <detpic32.h>
// Interrupt Handler
void _int_(27) isr_adc(void) // Replace VECTOR by the A/D vector // number - see "PIC32 family data // sheet" (pages 74-76)
{
    volatile int adc_value;
    LATDbits.LATD11 = 0; // Reset RD11 (LATD11 = 0)
    adc_value = ADC1BUF0;
    AD1CON1bits.ASAM = 1; // Start A/D conversion
    LATDbits.LATD11 = 1; // Set RD11 (LATD11 = 1)
    IFS1bits.AD1IF = 0; // Reset AD1IF flag
    LATDbits.LATD11 = 1;
}


int main(void)
{
    // Configure all (digital I/O, analog input, A/D module)

    // Configure the A/D module and port RB4 as analog input
    TRISDbits.TRISD11 = 0;      // RD11 output digital
    TRISBbits.TRISB4 = 1;       // RB4 digital output disconnected
    AD1PCFGbits.PCFG4 = 0;      // RB4 configured as analog input (AN4)
    AD1CON1bits.SSRC = 7;       // Conversion trigger selection bits: in this
                                //  mode an internal counter ends sampling and
                                //  starts conversion (modos de seleção,7 é automátcio)
    AD1CON1bits.CLRASAM = 1;    // Stop conversions when the 1st A/D converter
                                //  interrupt is generated. At the same time,
                                //  hardware clears the ASAM bit 
    AD1CON3bits.SAMC = 16;      // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = 0;     // Interrupt is generated after N samples
                                //  (replace N by the desired number of
                                //  consecutive samples)
    AD1CHSbits.CH0SA = 4;       // Selects AN4 as input for the A/D converter
    AD1CON1bits.ON = 1;         // Enable A/D converter

    IPC6bits.AD1IP = 2; // configure priority of A/D interrupts
    IFS1bits.AD1IF = 0; // clear A/D interrupt flag
    IEC1bits.AD1IE = 1; // enable A/D interrupts

    // Configure interrupt system
    EnableInterrupts(); // Global Interrupt Enable
    // Start A/D conversion
    AD1CON1bits.ASAM = 1;
    while(1){
        LATDbits.LATD11 = 0;
    }
    return 0;
}

// tempo = 840 ns

