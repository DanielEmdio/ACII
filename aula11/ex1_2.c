#include <detpic32.h>

void putc(char byte){
    while(U2STAbits.UTXBF != 0);
    U2TXREG = byte;
}

void _int_(32) isr_uart2(void)
{
    if(IFS1bits.U2RXIF == 1){
    // Read character from FIFO (U2RXREG)
    // Send the character using putc()
    putc(U2RXREG);
    // Clear UART2 Rx interrupt flag
    IFS1bits.U2RXIF = 0;
    }
}

int main(void)
{
    // Configure UART2: 115200, N, 8, 1
    int ovs_factor[2] = { 16,4 };
    int ovs = 0; // 0 (x16, standard speed) ou 1 (x4, high speed)
    U2MODEbits.ON = 0; // desativa a UART
    U2MODEbits.BRGH = ovs; // configura fator de sobre amostragem (0 ou 1)
    U2MODEbits.PDSEL = 0; // 0 (8N), 1 (8E), 2 (8O), 3 (9N) --- ver manual 
    U2MODEbits.STSEL = 0; // 0 (1 stop bits), 1 (2 stop bits) --- ver manual
    U2STAbits.UTXEN = 1; // ativa transmissão (ver nota abaixo)
    U2STAbits.URXEN = 1; // ativa receção (ver nota abaixo)
    

    // Configure UART2 interrupts, with RX interrupts enabled and TX interrupts disabled:
    
    //enable U2RXIE, disable U2TXIE (register IEC1)
    IEC1bits.U2TXIE = 0; 
    IEC1bits.U2RXIE = 1; 
    
    //set UART2 priority level (register IPC8)
    IPC8bits.U2IP = 1; // prioridade da interrupção (1 a 6)
    
    //clear Interrupt Flag bit U2RXIF (register IFS1)
    IFS1bits.U2RXIF = 0;
    
    //define RX interrupt mode (URXISEL bits)
    U2STAbits.URXISEL = 0;

    U2MODEbits.ON = 1; // ativa UART

    // Enable global Interrupts
    EnableInterrupts();
    while(1);
    return 0;
}
