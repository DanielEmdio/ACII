#include <detpic32.h>

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}


int main(void)
{
    TRISE = TRISE & 0xFFF0;

    While(1){
        char key = getChar();
        switch (key)
        {
        case '0':
            LATEbits.LATE0=1;
            LATEbits.LATE1=0;
            LATEbits.LATE2=0;
            LATEbits.LATE3=0;
            break;
        case '1':
            LATEbits.LATE0=0;
            LATEbits.LATE1=1;
            LATEbits.LATE2=0;
            LATEbits.LATE3=0;
            break;
        case '2':
            LATEbits.LATE0=0;
            LATEbits.LATE1=0;
            LATEbits.LATE2=1;
            LATEbits.LATE3=0;
            break;
        case '3':
            LATEbits.LATE0=0;
            LATEbits.LATE1=0;
            LATEbits.LATE2=0;
            LATEbits.LATE3=1;
            break;
        default:
            LATEbits.LATE0=1;
            LATEbits.LATE1=1;
            LATEbits.LATE2=1;
            LATEbits.LATE3=1;
            delay(1000);
            LATEbits.LATE0=0;
            LATEbits.LATE1=0;
            LATEbits.LATE2=0;
            LATEbits.LATE3=0;
            break;
        }
    }
    return 0;
}
