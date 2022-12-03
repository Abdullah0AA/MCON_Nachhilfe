#include <xc.h>
#include<pic18f24k22.h>
#define _XTAL_FREQ 4000000
#include <stdint.h>

void __init(){
    OSCCON = 0x50; // 4 MHz internal clock

    /***************************************************/
    // configure pins
    /***************************************************/
    TRISAbits.TRISA0 = 1; // Poti to input
    ANSELA = 1; // Port A0 to analog, all others digital
        
    ANSELB = ANSELC = 0x00; // all pins of port B and C digital I/O 
    TRISB &= 0b11000011; // all LEDs output, other directions unchanged
}

void ADC_Init(){
    
    ADCON0bits.CHS = 0b00000; // select poti PORTA0
    ADCON1bits.PVCFG = 0b00; // upper limit 5V (Vdd)
    ADCON1bits.NVCFG = 0b00; // lower limit 0V (Vss)
    ADCON2bits.ACQT = 0b010; // 4 Tad = 8 us
    ADCON2bits.ADCS = 0b001; // Tad = 1/(Fosc/8) = 1/(4 MHz /8) = 2 us
    ADCON2bits.ADFM = 0; // left just => use only 8 bit
    ADCON0bits.ADON = 1;    // AD Module on!! Don't forget
    
}

void CCP5_Config(){

    // AD Conversion with 10Hz With CCP5
    CCP5CONbits.CCP5M = 0b1011; // CCP5 On match Starts AD Conversion (AD...GO)
    // Select Timer 1
    CCPTMRS1bits.C5TSEL = 0b00;
    // When should a Match happen? Every 100 ms
    CCPR5 = 50000; // 100 ms

}
void Timer1Config(){

    // CLK source
    T1CONbits.TMR1CS = 0b00;
    // Prescaler
    T1CKPS1 = 0;
    T1CKPS0 = 1;
    // Timer 1 On
    TMR1ON = 1;

}
void interrupts(){

    PIE1bits.ADIE = 1; // Enable AC-Interrrupt upon Completion
    PIR1bits.ADIF = 0; // will be set upon conversion completion
    GIE = 1;
    PEIE = 1;
    
}




uint16_t ADC_Read(){
    // Wait for conversion
    GO = 1;
    while (ADCON0bits.GO_DONE);
    // Start another conversion
    ADCON0bits.GO_DONE = 1;

    return ADRES;
}

/** PS = 2
 * Fosc = 4 / PS MHz <-> 2 / 4 MHz <-> 0.5 MHz 
 * 1/0.5*10^6 = 2*10^-6s <=> 2us
 * Tinstruction = 2 us
 * Ticks * 2us = 100ms
 * 
 * Ticks = 100*1000us/2us = 50000 ticks
 * 
 * TMR1 Max = 65535 ticks
 * 
 */