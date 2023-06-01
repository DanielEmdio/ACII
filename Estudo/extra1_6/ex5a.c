#include <detpic32.h>

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

int main()
{
    int counter = 0;
    while(1){
        do{
            printInt(counter, 10 | 2 << 16);
            delay(100);
            counter++;
            putchar('\r');
        }while(counter < 100);
        counter = 0;
    }
    return 0;
}
