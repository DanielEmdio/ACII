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
            LATB = (LATB & 0x0000) | 0x7700;
            break;
        case 'B':
            LATB = (LATB & 0x0000) | 0x7C00;
            break;
        case 'C':
            LATB = (LATB & 0x0000) | 0x3900;
            break;
        case 'D':
            LATB = (LATB & 0x0000) | 0x5E00;
            break;
        case 'E':
            LATB = (LATB & 0x0000) | 0x7900;
            break;
        case 'F':
            LATB = (LATB & 0x0000) | 0x7100;
            break;
        case 'G':
            LATB = (LATB & 0x0000) | 0x7D00;
            break;
        default:
            break;
        }
    }
    return 0;
}
