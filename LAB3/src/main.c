#include <xc.h>
#include <pic18f25k22.h>

// CONFIG1H
#pragma config FOSC = INTIO67 // Oscillator Selection bits ( Internal oscillator block )
// CONFIG2H
#pragma config WDTEN = OFF // Watchdog Timer Enable bits ( Watch dog timer disabled )
// Watchdog will be explainded later
// CONFIG3H
#pragma config PBADEN = OFF // PORTB A/D Enable bit (PORTB <5:0 > pins are configured as digital )
// replaces ANSELB = 0; within source code
// CONFIG4L
#pragma config LVP = OFF // Single - Supply ICSP Enable bit (Single - Supply ICSP disabled )


void main(void){
    /* Configuration of oscillator */
    OSCCONbits.SCS = 0b00; // primary clock by internal oscillator (CONFIG1H FOSC = ...)
    OSCCONbits.IRCF = 2;   // 500 kHz internal clock, see datasheet 2.2 Oscillator Control

    
    // Timer 0 init
    TMR0ON = 1;
    T08BIT = 0;
    T0CS = 0;
    PSA = 1;
    TMR0IE = 1;
    TMR0IF = 0;

    TRISA2 = 0;
    LATA2 = 0;

    char c = 0;
    while (1)
    {
        if (TMR0IF){
            c++;
            if (c == 2){

            LATA2 = !LATA2;
            c = 0;
        }
            TMR0IF = 0;
        }
    }
}