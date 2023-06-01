#include<detpic32.h>

int main(void){

    unsigned char N = 1;        // Number of samples
    volatile int aux;

    // Configure the A/D module and port RB4 as analog input
    TRISBbits.TRISB4 = 1;       // RB4 digital output disconnected
    TRISDbits.TRISD11 = 0;      // RD11 output digital
    AD1PCFGbits.PCFG4 = 0;      // RB4 configured as analog input (AN4)
    AD1CON1bits.SSRC = 7;       // Conversion trigger selection bits: in this
                                //  mode an internal counter ends sampling and
                                //  starts conversion (modos de seleção,7 é automátcio)
    AD1CON1bits.CLRASAM = 1;    // Stop conversions when the 1st A/D converter
                                //  interrupt is generated. At the same time,
                                //  hardware clears the ASAM bit 
    AD1CON3bits.SAMC = 16;      // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = N-1;     // Interrupt is generated after N samples
                                //  (replace N by the desired number of
                                //  consecutive samples)
    AD1CHSbits.CH0SA = 4;       // Selects AN4 as input for the A/D converter
    AD1CON1bits.ON = 1;         // Enable A/D converter

    while (1){
        // Start A/D conversion
        AD1CON1bits.ASAM = 1;     // Start conversion 
        LATDbits.LATD11 = 1;      // Set LATD11 (LATD11=1)

        // Wait while conversion not done (AD1IF == 0)
        while( IFS1bits.AD1IF == 0 );   // Wait while conversion not done

        LATDbits.LATD11 = 0;      // // Reset LATD11 (LATD11=0)      

        // Read conversion result (ADC1BUF0 value) and print it
        aux = ADC1BUF0;
        printInt(aux, 16 | 3 << 16);
        
        // Erase line
        printStr("\n");

        // Reset AD1IF
        IFS1bits.AD1IF = 0;     // Reset AD1IF
        
    }
    
    return 0;

}




// R : 3.32 mS