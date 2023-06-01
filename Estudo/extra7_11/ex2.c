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
    switch(freq){
        case 2:
            delay(400);
        break;

        case 4:
            delay(150);
        break;

        case 6:
            delay(67);
        break;

        case 8:
            delay(25);
        break;

        case 10:
        break;
    }
    count++;
    if(count>99){count = 0;}
    IFS0bits.T1IF = 0;
}

void _int_(8) isr_T2(void){
    send2Display(toBcd(count));
    IFS0bits.T2IF = 0;
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
    IPC1bits.T1IP = 1; // Interrupt priority (must be in range [1..6])
    IEC0bits.T1IE = 1; // Enable timer T2 interrupts
    IFS0bits.T1IF = 0; // Reset timer T2 interrupt flag


    //timer 2
    T2CONbits.TCKPS = 3; // 1:32 prescaler (i.e Fout_presc = 625 KHz)
    PR2 = 49999; // Fout = 20MHz / (32 * (62499 + 1)) = 10 Hz
    TMR2 = 0; // Reset timer T2 count register
    T2CONbits.TON = 1; // Enable timer T2 (must be the last command of the timer configuration sequence

    // interrupion timer 2
    IPC2bits.T2IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T2IE = 1; // Enable timer T2 interrupts
    IFS0bits.T2IF = 0; // Reset timer T2 interrupt flag

    EnableInterrupts();

    int input;
    int is_new = 0x34;
    //char k;


    while(1){
        input = inkey();
        if((input >= 0x30) & (input <= 0x34) & (input != is_new)){
            is_new = input;
            printStr("Nova frequência: ");
            printInt10(input & 0xF);
            putChar('\n');

            //calculo
            freq = 2 * (1 + (input & 0xF));
            
            /*
            //timer 1
            if(freq < 6){
                T1CONbits.TCKPS = 3; // 1:32 prescaler (i.e Fout_presc = 625 KHz)
                k = 256;
            }else{
                T1CONbits.TCKPS = 2; // 1:32 prescaler (i.e Fout_presc = 625 KHz)
                k = 64;
            }
            PR1 = (20000000/k*freq) -1; // Fout = 20MHz / (32 * (62499 + 1)) = 10 Hz
            TMR1 = 0; // Reset timer T2 count register
            T1CONbits.TON = 1; // Enable timer T2 (must be the last command of the timer configuration sequence
            IFS0bits.T1IF = 0; // Reset timer T2 interrupt flag
            */
        }
    }
    return 0;
}

