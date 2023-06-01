
#include <detpic32.h>

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

int main(void)
{
    int counter = 0;
    char key;
    int T = 100;
    int freq = 10;
    while(1){
        do{
            putchar('\r');
            printInt(counter, 10 | 2 << 16);
            delay(T);
            key = inkey();
            if(key > 0x29 & key < 0x35){
                key = key & 0xF;
                freq = 2 * (1 + key);
                T = (1000/freq);
            }
            else if (key == '\n'){
                putchar('\n');
                printInt(counter, 10 | 2 << 16);
                printStr(", ");
                printInt10(freq);
                printStr(" Hz");
                putchar('\n');
            }
        }while(counter++ < 99);
        counter = 0;
    }
    return 0;
}
