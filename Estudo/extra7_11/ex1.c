#include <detpic32.h>

int val;
int onOff = 0;

void send2Display(int value) {
    static const char disp7codes[] = {0x3F ,0X06 ,0x5B ,0x4F ,0x66 ,0x6D ,0x7D ,0x07 ,0x7F ,0x6F ,0x77 ,0x7C ,0x39 ,0x5E ,0x79 ,0x71 ,0x3D};
    static char displayFlag = 0;

    char digit_low = value & 0x0F;
    char digit_high = value >> 4;
    if(onOff){
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
    }else{
        LATB = LATB & 0x80FF;
    }
}

void _int_(8) isr_T3(void){
    send2Display(val);
    IFS0bits.T2IF = 0;
}

int main(void){
    //Leds 
    TRISE = TRISE & 0xFFF0;
    LATE = LATE & 0xFFF0;
    
    //Displays
    TRISD = TRISD & 0xFF9F;
    TRISB = TRISB & 0x80FF;


    //timer 2
    T2CONbits.TCKPS = 2; // 1:32 prescaler (i.e Fout_presc = 625 KHz)
    PR2 = 49999; // Fout = 20MHz / (32 * (62499 + 1)) = 10 Hz
    TMR2 = 0; // Reset timer T2 count register
    T2CONbits.TON = 1; // Enable timer T2 (must be the last command of the timer configuration sequence

    // interrupion timer 2
    IPC2bits.T2IP = 1; // Interrupt priority (must be in range [1..6])
    IEC0bits.T2IE = 1; // Enable timer T2 interrupts
    IFS0bits.T2IF = 0; // Reset timer T2 interrupt flag

    EnableInterrupts();

    while(1){
        char input = inkey();
        if(input == 0){}
        else if(input < 0x30 || input > 0x33){
            val = 0xFF;
            LATE = LATE | 0x000F;
            resetCoreTimer();
            while(readCoreTimer() < 20000 * 1000);
            onOff = 0;
            LATE = LATE & 0xFFF0;
        }else{
            LATE = (LATE & 0xFFF0) | (1 << (input & 0xF));
            val = input & 0xF;
            onOff = 1;
        }
    }
    return 0;
}

