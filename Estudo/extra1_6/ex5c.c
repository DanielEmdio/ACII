#include <detpic32.h>

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

void send2displays(unsigned char value){
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
        val = display7Scodes[dig_hig];
        LATB = ((LATB & 0x80FF) | (val<<8));
    }
    flag = !flag;
}


int main(void)
{
    int counter = 0;
    char key;
    int T = 100;
    int freq;
    int display;
    int i = 0;
    While(1){
        do{
            putchar('\r');
            printInt(counter, 10 | 2 << 16);
            T /= 20;
            do{
                send2displays(display);
                delay(20);
            }(while i++ < T);
            i = 0;
            key = inkey();
            if(key > 0x29 & key < 0x35){
                key = key & 0xF;
                freq = 2 * (1 + key);
                T = (1/freq)*1000;
            }
            else if (key == 0x0D){
                putchar('\n');
                printInt(counter, 10 | 2 << 16);
                println(', ' + freq + ' Hz');
                putchar('\n');
                display = counter;
            }
        }while(counter++ < 101);
        counter = 0;
    }
    return 0;
}