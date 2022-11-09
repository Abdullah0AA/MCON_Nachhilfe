/*
 * File:   L02_main_badStyle1HzBlinker.c
 * Author: blero
 *
 * Created on 21. August 2019, 16:49
 * modified 28.03.2022
 */

/*  1. Run example as it is. There are four errors built in (syntax and logical). React to the error message!
 *  2. Run example with two breakpoints on lines
 *     line 51  flagBlinking = flagBlinking^1; // toggle bit 0 of flagBlinking  and
 *     line 57  Nop(); // Do nothing
 */

#include <xc.h> // XC8 Compiler Header, provides processor specific information e.g. register names
#include <pic18f25k22.h>
// configuration of the device

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

//--- P R I V A T E   P R O T O T Y P E S --------------------------------------
void __init(void); // only one function is given in this small project

//##############################################################################
// Function:        void main(void)
//					called from the XC8 startup code
//##############################################################################

void main()
{
    volatile unsigned int time;
    unsigned char flagBlinking = 0;

    // call function for initialization
    __init();

    /* main loop, never left */

    while (1)
    {
        /**
         * Answer to question L2.1:
         * The first if-statement checks whether the encoder button is pressed by reading the input to RB1 from PORTBbits.RB1.
         * The logic is inverted, i.e. when the button is pressed, the input of RB1 is 0, otherwise 1.
         * The while in the line after it makes sure that nothing happens until the pressed encoder button is released.
         * After pressing and releasing the button, the value of flagBlinking changes from 0 to 1 and vice versa.
         *
         * The second if statement checks flagBlinking. If it is 1 (true),
         * the value written to LATBbits.LATB4 is toggled, from 0 to 1 and vice versa. (which causes LED 4 to blink).
         * The for loop is used to create a delay of about 500ms.
         * As long as the encoder button is not pressed again, LATBbits.LATB4 is toggled every 500ms and LED 4 flashes.
         * When the encoder button is pressed, LED 4 assumes the last state before the button was pressed.
         */

        if (PORTBbits.RB1 == 0)
        { // encoder button pressed
            while (PORTBbits.RB1 == 0)  ;                            // encoder button still pressed
            flagBlinking = flagBlinking ^ 1; // toggle bit 0 of flagBlinking
        }

        if (flagBlinking)
        {
            LATBbits.LATB4 = !LATBbits.LATB4; // toggle LED 2
                                              /*The delay is calculated using the following equation:
                                               * In the for-loop t < x.
                                               * Instruction clock = Fosc/4 --> 500kHz/4 = 125khz <-> Tinsruct = 1/125khz = 8us
                                               * A loop pass corresponds to 11 instruction cycles
                                               * so for a loop the create a delay of 500ms we need x iterations
                                               * x = delay in us/Tinsruct*11
                                               * x = 500000us/8us*11 = 5682
                                               */
            for (time = 0; time < 5682; time++)
            {          // waste some time approx. 500 ms
                Nop(); // Do nothing
            }
        }
    }
}

void __init()
{

    /* Configuration of oscillator */
    OSCCONbits.SCS = 0b00; // primary clock by internal oscillator (CONFIG1H FOSC = ...)
    OSCCONbits.IRCF = 2;   // 500 kHz internal clock, see datasheet 2.2 Oscillator Control

    /* Configuration of input/output pins */
    LATB = 0;           // first define output values, then change direction!
    TRISB = 0b11101111; // see datasheet 10.0 I/O PORTS
}

// last line has to be empty