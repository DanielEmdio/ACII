#include <detpic32.h>

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

void send2displays(unsigned char value)
{
    static const char display7Scodes[] = {0x3F, 0x06, 0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
    static char displayFlag = 0; // static variable: doesn't loose its
    // value between calls to function
    char digit_low = value & 0x0F;
    char digit_high = value >> 4;
    char val;
    if (displayFlag == 0){
        LATD = (LATD & 0xFF1F) | 0x0020;
        val = display7Scodes[digit_low];
        LATB = ((LATB & 0x80FF) | (val<<8));
        
    }
    else{
        LATD = (LATD & 0xFF1F) | 0x0040;
        val = display7Scodes[digit_high];
        LATB = ((LATB & 0x80FF) | (val<<8)); 
    }
    displayFlag = !displayFlag; 
}

unsigned char toBcd(unsigned char value)
{
    return ((value / 10) << 4) + (value % 10);
}

int main(void)
{
    unsigned char N = 4;        // Number of samples

    // Configure the A/D module and port RB4 as analog input
    TRISB = (TRISB & 0x8000) | 0x00FF;
    TRISD = (TRISD & 0xFF80) | 0x001F;
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
    int V;
    int sum;

    while(1)
    {
        delay(10);
        int *p = (int *)(&ADC1BUF0);
        AD1CON1bits.ASAM = 1;     // Start conversion 
        while( IFS1bits.AD1IF == 0 );   // Wait while conversion not done
        for(; p <= (int *)(&ADC1BUF3); p+=4 ) {
            sum += *p; 
        }
        sum = sum/4;
        V=(sum*33+511)/1023;
        sum = 0;
        V = toBcd(V);

        int i = 0;
        do
        {
            send2displays(V);
            delay(2);
        } while(++i < 100);

        IFS1bits.AD1IF = 0;     // Reset AD1IF
    }
    return 0;
}