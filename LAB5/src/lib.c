#include <xc.h>
#include<pic18f24k22.h>
#define _XTAL_FREQ 4000000
#include <stdint.h>

void __init(){

    // Fosc = 4MHz
    OSCCON = 0x50;
    // pin config
    TRISA0 = 1;
    TRISB0 = 0;
    LATB0 = 0;
    // set RA0 to Analog
    ANSELA = 1;
}

void ADC_Init(){
    // Select AN0
    ADCON0bits.CHS = 0b00000;
    // Select Vref+ to be VDD
    ADCON1bits.PVCFG = 0b00;
    // Select Vref- to be Vss
    ADCON1bits.NVCFG = 0b00;
    // 4 * TAD
    ADCON2bits.ACQT = 0b010;
    // TAD = 2us <=> Fosc/8
    ADCON2bits.ADCS = 0b001;
    // Result: Left justified
    ADCON2bits.ADFM = 1;

    ADCON0bits.ADON = 1;

    // Wait Aq Time
    __delay_us(8);

    // Start Conversion
    ADCON0bits.GO_DONE = 1;
    
}

uint16_t ADC_Read(){
    // Wait for conversion
    while (ADCON0bits.GO_DONE);

    // Start another conversion
    ADCON0bits.GO_DONE = 1;

    return ADRES;
}

// 10 - 1024
// 5V <=> 1024
// 0V <=> 0
// 2.5V <=> 512
// 5/2.5 = 1024/x
// x = 2.5*1024/5 = 512 