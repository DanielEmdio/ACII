#include <detpic32.h>

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

int main(void)
{
    // Configure port RE as output
    TRISE = (TRISE & 0xFF80) | 0x0007;
    int count;
    while(1)
    {
        LATE = (LATE & 0x0000) | count<<3;
        // 4 Hz
        delay(250);
        count++;
        // Toggle RC14 port value
        if(count == 10){
            count = 0;
        }
    }
    return 0;
}

