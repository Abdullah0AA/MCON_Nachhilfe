#include <xc.h>
#include <stdint.h>
#include <pic18f25k22.h>
#include "../headers/config.h"
#include "../headers/lib.h"

char resultBits;

void main(void){

    __init();
    ADC_Init();
    CCP5_Config();
    Timer1Config();
    interrupts();

    while (1)
    {
        // Do Nothing
    }
    

}


void __interrupt ( high_priority ) high_isr( void ){

    if (ADIF && ADIE){

        ADIF = 0;
        resultBits = ADRESH; // read out result of ADC
        LATB = resultBits;
    }

}