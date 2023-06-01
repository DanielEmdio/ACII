#include <detpic32.h>

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

int main(void)
{
    unsigned char segment;
    TRISB = (TRISB & 0x8000) | 0x00FF;
    TRISD = (TRISD & 0xFF80) | 0x001F;
    LATD = (LATD & 0xFF9F) | 0x0020;
    int i;
    while(1)
    {
        segment = 0;
        for(i=0; i < 8; i++)
        {
            LATB = (LATB & 0x0000 | segment<<8);
            delay(500);
            segment = segment << 1;
            segment += 1; 
        }
        // toggle display selection
        //LATB = (LATB & 0x0000);
    }
    return 0;
}
