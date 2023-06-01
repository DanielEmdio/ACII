#include <detpic32.h>

int main(void)
{
    TRISB = TRISB | 0x000F;
    TRISE = TRISE & 0xFF0F;

    While(1){
        LATEbits.LATE4=PORTBbits.RB3;
        LATEbits.LATE5=PORTBbits.RB2;
        LATEbits.LATE6=PORTBbits.RB1;
        LATEbits.LATE7=PORTBbits.RB0;
    }
    return 0;
}
