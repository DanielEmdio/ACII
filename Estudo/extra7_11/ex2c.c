#include <detpic32.h>

int count;
int freq = 10;

unsigned char toBcd(unsigned char value) {
    return ((value / 10) << 4) + (value % 10);
}

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

void send2Display(int value) {
    static const char disp7codes[] = {0x3F ,0X06 ,0x5B ,0x4F ,0x66 ,0x6D ,0x7D ,0x07 ,0x7F ,0x6F ,0x77 ,0x7C ,0x39 ,0x5E ,0x79 ,0x71 ,0x3D};
    static char displayFlag = 0;

    char digit_low = value & 0x0F;
    char digit_high = value >> 4;
    if (!displayFlag){
        // select display low
        LATD = (LATD & 0xFF9F) | 0x0020; // ... 1001 1111 | ... 0010 0000
        // set display low
        LATB = (LATB & 0x80FF) | (disp7codes[(unsigned)digit_low] << 8); // 1000 0000 1111 1111
    } else {
        // select display high
        LATD = (LATD & 0xFF9F) | 0x0040; // ... 1001 1111 | ... 0100 0000

        // set display high
        LATB = (LATB & 0x80FF) | (disp7codes[(unsigned)digit_high] << 8); // 1000 0000 1111 1111
    }
    displayFlag = !displayFlag;
}

void _int_(4) isr_T1(void){
    delay(((1/freq)*1000) - 100); 
    count++;
    if(count>99){count = 0;}
    IFS0bits.T1IF = 0;
}

void _int_(8) isr_T2(void){
    send2Display(toBcd(count));
    IFS0bits.T2IF = 0;
}

void _int_(27) isr_adc(void){
    printInt10(ADC1BUF0);
    putChar('1');
    freq = 1 + (ADC1BUF0/127);
    IFS1bits.AD1IF = 0; // Reset AD1IF flag
}

int main(void){
    //Displays
    TRISD = TRISD & 0xFF9F;
    TRISB = TRISB & 0x80FF;

    //timer 1
    T1CONbits.TCKPS = 2; // 1:32 prescaler (i.e Fout_presc = 625 KHz)
    PR1 = 31249; // Fout = 20MHz / (32 * (62499 + 1)) = 10 Hz
    TMR1 = 0; // Reset timer T2 count register
    T1CONbits.TON = 1; // Enable timer T2 (must be the last command of the timer configuration sequence

    // interrupion timer 1
    IPC1bits.T1IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T1IE = 1; // Enable timer T2 interrupts
    IFS0bits.T1IF = 0; // Reset timer T2 interrupt flag


    //timer 2
    T2CONbits.TCKPS = 3; // 1:32 prescaler (i.e Fout_presc = 625 KHz)
    PR2 = 49999; // Fout = 20MHz / (32 * (62499 + 1)) = 10 Hz
    TMR2 = 0; // Reset timer T2 count register
    T2CONbits.TON = 1; // Enable timer T2 (must be the last command of the timer configuration sequence

    // interrupion timer 2
    IPC2bits.T2IP = 3; // Interrupt priority (must be in range [1..6])
    IEC0bits.T2IE = 1; // Enable timer T2 interrupts
    IFS0bits.T2IF = 0; // Reset timer T2 interrupt flag

/*
    //ADC
    TRISBbits.TRISB4 = 1; // RBx digital output disconnected
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

    //ADC interrupt
    IPC6bits.AD1IP = 1;
    IFS1bits.AD1IF = 0;
    IEC1bits.AD1IE = 1;
*/
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
        delay(250);
        AD1CON1bits.ASAM = 1;
    }
    return 0;
}   
