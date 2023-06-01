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


int main(void){
    // Configure UART2:
    int ovs_factor[2] = { 16,4 };
    int ovs = 0; // 0 (x16, standard speed) ou 1 (x4, high speed)
    U2MODEbits.ON = 0; // desativa a UART
    U2MODEbits.BRGH = ovs; // configura fator de sobre amostragem (0 ou 1)

    // 1 - Configure BaudRate Generator
    U2BRG = 10;
    
    // 2 – Configure number of data bits, parity and number of stop bits
    U2MODEbits.PDSEL = 0; // 0 (8N), 1 (8E), 2 (8O), 3 (9N) --- ver manual 
    U2MODEbits.STSEL = 0; // 0 (1 stop bits), 1 (2 stop bits) --- ver manual
    
    // 3 – Enable the trasmitter and receiver modules (see register U2STA)
    U2STAbits.UTXEN = 1; // ativa transmissão (ver nota abaixo)
    U2STAbits.URXEN = 1; // ativa receção (ver nota abaixo)
    
    // 4 – Enable UART2 (see register U2MODE)
    U2MODEbits.ON = 1; // ativa UART

    while(1){
        putc('+');
        delay(1000);// wait 1s
    }
    
    return 0;
}

