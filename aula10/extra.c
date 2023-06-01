#include <detpic32.h>

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

void putc(char byte){
    // wait while UART2 UTXBF == 1
    while(U2STAbits.UTXBF != 0);// espera até que o buffer de transmissão deixe de estar cheio

    // Copy "byte" to the U2TXREG register
    U2TXREG = byte;
}

void configUart2(unsigned int baud, char parity, unsigned int stopbits)
{
    // Configure UART2:
    int ovs_factor[2] = { 16,4 };
    int ovs = 0; // 0 (x16, standard speed) ou 1 (x4, high speed)
    U2MODEbits.ON = 0; // desativa a UART
    U2MODEbits.BRGH = ovs; // configura fator de sobre amostragem (0 ou 1)

    // Configure BaudRate Generator
    U2BRG = ((20000000+8*baud)/(16*baud))-1;

    // Configure number of data bits (8), parity and number of stop bits
    switch(parity) {
    case 'N':
        parity = 0;
        break;
    case 'E':
        parity = 1;
        break;
    default:
        parity = 2;
    }
    U2MODEbits.PDSEL = parity; // 0 (N), 1 (E), 2 (O) 


    U2MODEbits.STSEL = stopbits-1; // 0 (1 stop bits), 1 (2 stop bits)

    // Enable the trasmitter and receiver modules
    U2STAbits.UTXEN = 1; // ativa transmissão (ver nota abaixo)
    U2STAbits.URXEN = 1; // ativa receção (ver nota abaixo)
   
    // Enable UART2
    U2MODEbits.ON = 1; // ativa UART
}


int main(void){
    printStr("Baud?");
    unsigned int baud = readInt10();
    if(baud < 600 || baud >115200){
        baud = 115200;
    }

    printStr("\nParity?");
    char parity = getChar();
    if(parity == 'N' || parity == 'E' || parity == 'O'){
    }else{
        parity = 'N';
    }

    printStr("\nStop bits?");
    unsigned int stopbits = readInt10();
    if(stopbits < 1 || stopbits > 2){
        stopbits = 1;
    }

    configUart2(baud,parity,stopbits);

    while(1){
        putc('+');
        delay(1000);// wait 1s
    }
    
    return 0;
}
