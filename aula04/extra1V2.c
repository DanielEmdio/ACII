#include <detpic32.h>

int main(void)
{
    // Configure port RE as output
    TRISB = (TRISB & 0x8000) | 0x00FF;
    TRISD = (TRISD & 0xFF80) | 0x001F;
    LATD = (LATD & 0xFF1F) | 0x0020;
    unsigned char input;
    unsigned int res;
    while(1)
    {
        input = 0;
        do{
            input = getChar();
        }while((input < 'A' || input >'G') && (input < 'a' || input > 'g'));

        input -= 1;
        res = 0x100;
        input = input & 0x7; //posso comentar esta linha
        res <<= input;
        LATB = (LATB ^ res);
    }
    return 0;
}
