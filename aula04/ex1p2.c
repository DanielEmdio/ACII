#include <detpic32.h>

int main(void)
{
    // Configure port RE as output
    TRISB = (TRISB & 0x8000) | 0x00FF;
    TRISD = (TRISD & 0xFF80) | 0x001F;
    LATD = (LATD & 0xFF1F) | 0x0020;
    unsigned char input;
    while(1)
    {
        input = getChar();
        if (input > 'G'){
            input -= 0X20; 
        }
        switch (input)
        {
        case 'A':
            LATBbits.LATB8 = !LATBbits.LATB8;
            break;
        case 'B':
            LATBbits.LATB9 = !LATBbits.LATB9;
            break;
        case 'C':
            LATBbits.LATB10 = !LATBbits.LATB10;
            break;
        case 'D':
            LATBbits.LATB11 = !LATBbits.LATB11;
            break;
        case 'E':
            LATBbits.LATB12 = !LATBbits.LATB12;
            break;
        case 'F':
            LATBbits.LATB13 = !LATBbits.LATB13;
            break;
        case 'G':
            LATBbits.LATB14 = !LATBbits.LATB14;
            break;
        default:
            break;
        }
    }
    return 0;
}
