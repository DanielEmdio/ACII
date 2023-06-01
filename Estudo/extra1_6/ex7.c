#include <detpic32.h>

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

int main(void)
{
    char T;
    TRISB = TRISB | 0x1F;
    AD1PCFGbits.PCFG4= 0; // RBx configured as analog input
    AD1CON1bits.SSRC = 7; // Conversion trigger selection bits: in this
    // mode an internal counter ends sampling and
    // starts conversion
    AD1CON1bits.CLRASAM = 1; // Stop conversions when the 1st A/D converter
    // interrupt is generated. At the same time,
    // hardware clears the ASAM bit
    AD1CON3bits.SAMC = 16; // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = 0; // Interrupt is generated after N samples
    // (replace N by the desired number of
    // consecutive samples)
    AD1CHSbits.CH0SA = 4; // replace x by the desired input
    // analog channel (0 to 15)
    AD1CON1bits.ON = 1; // Enable A/D converter
    // This must the last command of the A/D
    // configuration sequence
    TRISB = TRISB | 0xF;
    while(1){
        AD1CON1bits.ASAM = 1;     // Start conversion 
        while( IFS1bits.AD1IF == 0 );   // Wait while conversion not done
        if(ADC1BUF0 < 205){
            T = 1000;
        }else if (ADC1BUF0 < 410){
            T = 500;
        }else if (ADC1BUF0 < 615){
            T = 333;
        }else if (ADC1BUF0 < 820){
            T = 250;
        }else{
            T = 200;
        }
        printStr("DS");
        putChar('=')
        putChar(PORTB & 0xF);
        delay(T);
        IFS1bits.AD1IF = 0;
    }
}