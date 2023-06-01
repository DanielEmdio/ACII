#include <detpic32.h>

volatile int dc;



void delay(unsigned int ms){
	resetCoreTimer();
	while(readCoreTimer() < 20000*ms);
}

unsigned char to_bcd(unsigned char val){
    return ((val/10) << 4) + (val % 10);
}

void send2Display(int value) {
    static const char disp7codes[] = {0x3F ,0X06 ,0x5B ,0x4F ,0x66 ,0x6D ,0x7D ,0x07 ,0x7F ,0x6F ,0x77 ,0x7C ,0x39 ,0x5E ,0x79 ,0x71 ,0x3D};
    static char displayFlag = 0;

    char digit_low = value & 0x0F;
    char digit_high = value >> 4;

    if (!displayFlag)
    {
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

void putc(char byte2send){
    while(U2STAbits.UTXBF == 1);
    U2TXREG = byte2send;
}


void _int_(8) isr_t2(void){
    send2Display(to_bcd(dc));
    LATE = ((LATE & 0xFF00) | dc);
    IFS0bits.T2IF = 0; // Reset timer T2 interrupt flag
}

void _int_(12) isr_T3(void){
    LATDbits.LATD0 = !LATDbits.LATD0;
    IFS0bits.T3IF = 0; // Reset timer T2 interrupt flag
}

void _int_(32) isr_uart2(void){
    static char first = 0;
    static char flag = 0; 
    if(IFS1bits.U2RXIF == 1){
        char bruh =  U2RXREG;
        if(flag == 0){
            first = bruh & 0xF;
            putc(first);
            flag = 1;
        }else{
            putc(bruh & 0xF);
            dc = (first * 10) + (bruh & 0xF);
            flag = 0;
        }
        IFS1bits.U2RXIF == 0;
    }
}

int main(void){
    //leds
    TRISE = TRISE & 0xFF00;
    //switch
    TRISB = TRISB & 0x0001;
    //display e ponta
    TRISD = TRISD & 0xFF9E;
    TRISB = TRISB & 0x80FF;

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
    AD1CON2bits.SMPI = 3; // Interrupt is generated after N samples
    // (replace N by the desired number of
    // consecutive samples)
    AD1CHSbits.CH0SA = 4; // replace x by the desired input
    // analog channel (0 to 15)
    AD1CON1bits.ON = 1; // Enable A/D converterh

    //timer 2
    T2CONbits.TCKPS = 2; // 1:32 prescaler (i.e Fout_presc = 625 KHz)
    PR2 = 49999; // Fout = 20MHz / (32 * (62499 + 1)) = 10 Hz
    TMR2 = 0; // Reset timer T2 count register
    T2CONbits.TON = 1;
    //timer 2 interrupt
    IPC2bits.T2IP = 1; // Interrupt priority (must be in range [1..6])
    IEC0bits.T2IE = 1; // Enable timer T2 interrupts
    IFS0bits.T2IF = 0; // Reset timer T2 interrupt flag

    //timer 3
    T2CONbits.TCKPS = 0; // 1:32 prescaler (i.e Fout_presc = 625 KHz)
    PR2 = 19999; // Fout = 20MHz / (32 * (62499 + 1)) = 10 Hz
    TMR2 = 0; // Reset timer T2 count register
    T2CONbits.TON = 1; // Enable timer T2 (must be the last command of the
    // timer configuration sequence)
    OC1CONbits.OCM = 6; // PWM mode on OCx; fault pin disabled
    OC1CONbits.OCTSEL =0;// Use timer T2 as the time base for PWM generation
    OC1RS = 10000; // Ton constant
    OC1CONbits.ON = 1; // Enable OC1 module
    //timer 3 interrupt
    IPC3bits.T3IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T3IE = 1; // Enable timer T2 interrupts
    IFS0bits.T3IF = 0; // Reset timer T2 interrupt flag

    //uart2
    U2MODEbits.ON = 0;
    
    U2MODEbits.BRGH = 0;
    U2BRG = ((PBCLK + 8*9600)/(16*9600))-1;
    U2MODEbits.PDSEL = 2;
    U2MODEbits.STSEL = 1;

    U2STAbits.URXEN = 1;
    U2STAbits.UTXEN = 1;

    //interrupt
    U2STAbits.URXISEL = 0;

    IEC1bits.U2RXIE = 1;
    IEC1bits.U2TXIE = 0;
    IPC8bits.U2IP = 1;
    IFS1bits.U2RXIF = 0;


    U2MODEbits.ON = 1;


    EnableInterrupts();

    int average = 0;
    int i = 0;

    while(1){
        if(PORTB & 0x1){
            IEC1bits.U2RXIE = 1;
        }
        else{
            IEC1bits.U2RXIE = 0;
            AD1CON1bits.ASAM = 1;
            while( IFS1bits.AD1IF == 0 );
            int *p = (int *)(&ADC1BUF0);
            for(i = 0; i < 4; i++){
                average += p[i*4];
            }
            average /= 4;
            dc = ((99*average)/1363); //the fuck 1363??????
            IFS1bits.AD1IF = 0;
        }
        OC1RS = (20000 * dc)/100;
    }
}
