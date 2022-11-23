/*
 * File:   L03_main_exploringTimer.c
 * Author: Abdullah
 *
 * Created on 13. November 2022, 15:19
 */

// CONFIG1H
#pragma config FOSC = INTIO67 // Oscillator Selection bits ( Internal oscillator block )
// CONFIG2H
#pragma config WDTEN = OFF // Watchdog Timer Enable bits ( Watch dog timer disabled )
// Watchdog will be explained later
// CONFIG3H
#pragma config PBADEN = OFF // PORTB A/D Enable bit (PORTB <5:0 > pins are configured as digital )
// replaces ANSELB = 0; within source code
// CONFIG4L
#pragma config LVP = OFF // Single - Supply ICSP Enable bit (Single - Supply ICSP disabled )

void __init();

#include <xc.h>
#include <pic18f24k22.h>

void main(void)
{

    void __init();

    /* main loop , never left */
    while (1)
    {

        // LED 1
        LATBbits.LATB2 = (TMR0 >> 2) & 0b00000001; // LED 1 blinks , when TMR0 is running

        // LED 2
        if (INTCONbits.TMR0IF)
        {                        // overflow Timer 0
            LATBbits.LATB3 ^= 1; // toggle LED 2 , TMR0 overflown
            TMR0IF = 0;
            // timeT0++; // This is for debugging purposes , to see how often the LED blinked
        }

        // LED 3
        if (PIR1bits.TMR1IF)
        {
            LATBbits.LATB4 ^= 1; // toggle LED 3 , TMR1 overflown
             TMR1IF = 0;
            // timeT1++; // This is for debugging purposes , to see how often the LED blinked
        }

        // LED 4
        if (PIR1bits.CCP1IF)
        {
            PIR1bits.CCP1IF = 0; // reset overflow flag
            LATBbits.LATB5 ^= 1; // toggle LED 4 , CCP1 match occured
            // timeT1 ++; // This is for debugging purposes , to see how often the LED blinked
        }
    }
}

void __init()
{
    /* Configuration of oscillator */
    OSCTUNEbits.INTSRC = 1; // 31.25 kHz device clock derived from the MFINTOSC or HFINTOSC source (this needs a little more power but is more accurate )

    OSCCONbits.IRCF = 0; // 31.25 khz internal clock , very slow but low power !!

    /* Configuration of input / output pins */
    TRISB = 0b00000000; // all LEDs output
    LATB = 255;         // all LEDs off

    // set up timer 0T0CONbits . T0CS = 0; // clock source is instruction clock Fosc /4 = 31.5 kHz / 4
    T0CONbits.PSA = 0;      // pres - scaler assigned
    T0CONbits.T0PS = 0b111; // prescaler 1:256 (256 times slower )
    T0CONbits.T08BIT = 1;   // 8 bit , counts till 255
    TMR0H = 0;              // always write Highbyte first !
    TMR0L = 0;              // start value for timer set to zero
    T0CONbits.TMR0ON = 1;   // Timer 0 on
    // T out = 500ms
    /**T overflow = 8,33s
     * T ohne prescaler = 524ms 
     * 
     * 
     */

    // setup timer 1
    T1CONbits.TMR1CS = 0;  // Timer1 clock source is instruction clock ( FOSC /4)
    T1CONbits.T1CKPS = 1;  // 2^1 -> 1:2 Prescale value
    TMR1H = 0;             // always write Highbyte first !
    TMR1L = 0;             // start value for timer set to zero
    T1CONbits.TMR1ON = 1;  // Timer on 65535
    T1GCONbits.TMR1GE = 0; // Timer1 / counts regardless of gate function


    // setup capture and compare module CCP1
     CCP1CONbits . CCP1M = 0b1011 ; // Compare Mode with Special Event Trigger , CCP1IF is set
     CCPTMRS0bits . C1TSEL = 0; // timer <-> ccp module ( CCP1 / TMR1 )
     CCPR1 = 1953; // compare value for setting CCP1IF

     TMR1 = 0;
     // Zeit für ein Tick = 0,256ms
     // CCP1IF = 1 at 0,256ms * CCPR1 = 505ms
     // CCPR1 = 1000ms/0,256 = 3906 
     // 
     

    // clear overflow flag bits , called IF
    INTCONbits.TMR0IF = 0; // clear overflow bit of timer 0
    PIR1bits.TMR1IF = 0;   // clear overflow bit of timer 1
    PIR1bits.CCP1IF = 0;   // clear match of compare module CCP1
}

// 0000 0101 >> 2 & 0000 0001;
// 0000 0001 & 0000 0001 = 0000 0001
// 1 & 0 = 0
// 0 & 0 = 0
// 1 & 1 = 1
