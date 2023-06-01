#include <detpic32.h>

int main()
{
    TRISB = TRISB | 0x000F;
    TRISE = TRISE & 0xFFF0;

    while(1){
        LATEbits.LATE0=PORTBbits.RB0;
        LATEbits.LATE1=PORTBbits.RB1;
        LATEbits.LATE2=PORTBbits.RB2;
        LATEbits.LATE3=PORTBbits.RB3;
    }
    return 0;
}
