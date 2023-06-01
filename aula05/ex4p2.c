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

void send2leds(unsigned char value)
{
    LATE = ((LATE & 0xFF00) | value);
}

unsigned char toBcd(unsigned char value)
{
    return ((value / 10) << 4) + (value % 10);
}

int main(void)
{
    TRISB = (TRISB & 0x80FF) | 0x0001;
    TRISCbits.TRISC14 = 0; 
    TRISD = TRISD & 0xFF9F;
    TRISE = TRISE & 0xFF00;
    char counter = 0;
    unsigned char flag = 0;
    unsigned char T = 0;
    int timer = 0;
    while(1)
    {
        int i = 0;
        unsigned char var = toBcd(counter);
        unsigned char upDown = (PORTB & 0x0001);
        if(upDown == 1){
            counter += 1;
            T = 100;
        }else{
            counter -= 1;
            T = 250;
        }
        do
        {
            send2displays(var);
            send2leds(var);
            delay(2);
            timer = (timer + 2) * flag;
            printInt10(timer);
            putChar('\n'); 
        }while(++i < T);

        if (timer > 5000){
            timer = 0;
            flag = 0;
            LATCbits.LATC14 = 0;
        }

        if(counter > 59 ){
            counter = 0;
            flag = 1;
            LATCbits.LATC14 = 1;
        }

        if (counter < 0){
            counter = 59;
            flag = 1;
            LATCbits.LATC14 = 1;
        }
    }
    return 0;
}