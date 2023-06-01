#include <detpic32.h>

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

void send2display(unsigned char value){
    static const char display7Scodes[] = {0x3F, 0x06, 0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
    static char flag = 0;
    char dig_low = value & 0xF;
    char dig_high = value >> 4;
    char val;
    if(flag == 0){
        LATD = (LATD & 0xFF1F) | 0x0020; // 5 quado é o low
        val = display7Scodes[dig_low];
        LATB = ((LATB & 0x80FF) | (val<<8));
    }
    else{
        LATD = (LATD & 0xFF1F) | 0x0040; // 5 quado é o low
        val = display7Scodes[dig_high];
        LATB = ((LATB & 0x80FF) | (val<<8));
    }
    flag = !flag;
}

int main(void)
{
    TRISE = TRISE & 0xFFF0;
    TRISB = TRISB & 0x80FF;
    TRISD = TRISD & 0xFF9D;
    char val;
    char key;
    char i;
    int onOff = 0;

    while(1){
        do{
            key = inkey();
            if(onOff == 1){
                send2display(val);
            }else{
                LATB = (LATB & 0x80FF);
            }
            delay(10);
        }while(key == 0);
        if(key < '0' || key > '3'){
            LATE = (LATE | 0xF);
            val = 0xFF;
            do{
                send2display(val);
                delay(10);
            }while(i++ < 100);
            i = 0;
            LATE = (LATE & 0xFFF0);
            onOff = 0;
        }
        else{
            val = key & 0xF;
            key = 1 << val;
            LATE = (LATE & 0xFFF0) | key;
            onOff = 1;
        }
    }
    return 0;
}
