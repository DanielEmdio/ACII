#include <detpic32.h>

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

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}


int main(void)
{
    TRISB = (TRISB & 0x8000) | 0x00FF;
    TRISD = (TRISD & 0xFF80) | 0x001F;
    while(1)
    {
        send2displays(0x15);
        delay(1);
    }
}