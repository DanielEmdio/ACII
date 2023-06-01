#include <detpic32.h>
#define SAMPLE_COUNT 4

volatile int voltage = 0; // Global variable

void putc(char byte){
    while(U2STAbits.UTXBF != 0);
    U2TXREG = byte;
}

void send2Display(int value) {
    static const char disp7codes[] = {0x3F ,0X06 ,0x5B ,0x4F ,0x66 ,0x6D ,0x7D ,0x07 ,0x7F ,0x6F ,0x77 ,0x7C ,0x39 ,0x5E ,0x79 ,0x71 ,0x3D};
    static char displayFlag = 0;

    char digit_low = value & 0x0F;
    char digit_high = value >> 4;

    if (!displayFlag)
    {
        // select display low
        LATD = (LATD & 0xFF9F) | 0x0020; // ... 1001 1111 | ... 0010 0000

        // set display low
        LATB = (LATB & 0x80FF) | (disp7codes[(unsigned)digit_low] << 8); // 1000 0000 1111 1111
    } else {
        // select display high
        LATD = (LATD & 0xFF9F) | 0x0040; // ... 1001 1111 | ... 0100 0000

        // set display high
        LATB = (LATB & 0x80FF) | (disp7codes[(unsigned)digit_high] << 8); // 1000 0000 1111 1111
    }

    displayFlag = !displayFlag;
}

unsigned char toBcd(unsigned char value) {
    return ((value / 10) << 4) + (value % 10);
}

void _int_(4) isr_T1(void)
{
    AD1CON1bits.ASAM = 1; // start convertion
    IFS0bits.T1IF = 0;      // Reset T1IF flag
}

void _int_(12) isr_T3(void)
{
    send2Display(toBcd(voltage));// Send the value of the global variable "voltage" to the displays using BCD (decimal) format
    IFS0bits.T3IF = 0;      // Reset T3IF flag
}

void _int_(27) isr_adc(void)
{
    while( IFS1bits.AD1IF == 0 );
    int average = 0;
    int j;
    int *p = (int *) (&ADC1BUF0);
        for (j = 0; j < SAMPLE_COUNT; j++) {
            average += p[j*4];
        }
    average /= SAMPLE_COUNT; // Calculate buffer average (8 samples)
    voltage = (average * 33 + 511) / 1023; // Calculate voltage amplitude and copy it to "voltage"
    
    if(voltMax < average){
        voltMax = average;
    }else if(voltMin > average){
        voltMin = average;
    }

    IFS1bits.AD1IF = 0; // Reset AD1IF flag
}

void _int_(32) isr_uart2(void)
{
    char c = U2RXREG// Read character from FIFO
    if(c == 'M'){
        // Send "voltMax" to the serial port UART2
        
    }
    
    else if(c == 'm'){
        // Send "voltMin" to the serial port UART2
    }
    
    // Clear UART2 rx interrupt flag
    IFS1bits.U2RXIF = 0;
}


int voltMin;
int voltMax;

int main(void) {
    voltMin = 9999;
    voltMax = 0;
    // config 7segDisplays

    // set RB8- RB14 as outputs
    TRISB = TRISB & 0x80FF; // 1000 0000 1111 1111

    // set RD5 - RD6 as outputs
    TRISD = TRISD & 0xFF9F; // ... 1001 1111

    // config A/D
    TRISBbits.TRISB4   = 1;  // RB4 digital output disconnected
    AD1PCFGbits.PCFG4  = 0;  // RB4 configured  as analog input

    AD1CON1bits.SSRC    = 7; // convertion trigger selection (this mode uses a counter to end sampling and start convertion)
    AD1CON1bits.CLRASAM = 1; // stop convertion when 1st A/D converter interrupt is generated

    AD1CON3bits.SAMC   = 16; // sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI   = SAMPLE_COUNT - 1; // 4 consecutive sample

    AD1CHSbits.CH0SA   = 4;  // set analog input channel 4 (0 to 15)
    AD1CON1bits.ON     = 1;  // enable A/D converter
    IPC6bits.AD1IP     = 1; // configure priority of A/D interrupts
    IEC1bits.AD1IE     = 1;

    
    // config T1
    T1CONbits.TCKPS = 2; 
    PR1 = 62499; 
    IPC1bits.T1IP = 2;
    IEC0bits.T1IE = 1;
    

    // config T3
    T3CONbits.TCKPS = 2; 
    PR1 = 49999; 
    IPC3bits.T3IP = 3;
    IEC0bits.T3IE = 1;
    
    TMR1 = 0; // Reset timer T1 count register
    T1CONbits.TON = 1; // Enable timer T1 (must be the last command of the timer configuration sequence)
    
    TMR3 = 0; // Reset timer T3 count register
    T3CONbits.TON = 1; // Enable timer T3 (must be the last command of the timer configuration sequence)
    
    IFS0bits.T1IF = 0;      // Reset T1IF flag
    IFS0bits.T3IF = 0;      // Reset T3IF flag
    IFS1bits.AD1IF = 0;

    // Configure UART2: 115200, N, 8, 1
    int ovs_factor[2] = { 16,4 };
    int ovs = 0; // 0 (x16, standard speed) ou 1 (x4, high speed)
    U2MODEbits.ON = 0; // desativa a UART
    U2MODEbits.BRGH = ovs; // configura fator de sobre amostragem (0 ou 1)
    U2MODEbits.PDSEL = 0; // 0 (8N), 1 (8E), 2 (8O), 3 (9N) --- ver manual 
    U2MODEbits.STSEL = 0; // 0 (1 stop bits), 1 (2 stop bits) --- ver manual
    U2STAbits.UTXEN = 1; // ativa transmissão (ver nota abaixo)
    U2STAbits.URXEN = 1; // ativa receção (ver nota abaixo)
    
    // Configure UART2 interrupts, with RX and TX interrupts DISABLED disable U2RXIE, disable U2TXIE (register IEC1)
    IEC1bits.U2TXIE = 0; 
    IEC1bits.U2RXIE = 0; 

    //set UART2 priority level (register IPC8)
    IPC8bits.U2IP = 1; // prioridade da interrupção (1 a 6)

    //define TX interrupt mode (UTXISEL bits)
    U2STAbits.UTXISEL = 0;

    EnableInterrupts(); // Global Interrupt Enable

    while(1);
    return 0;
}