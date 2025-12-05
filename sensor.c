/*
 * sensor.c
 *
 *  Created on: Dec 3, 2025
 *      Author: lmshraybman
 */

#include <msp430.h>
#include "sensor.h"


unsigned int in_temp;
float tempC[MOVING_AVG_LENGTH];
volatile float temperatureDegC, temperatureDegF, degC_per_bit;

void setupADC(void){
    degC_per_bit = ((float)(85.0 – 30.0))/ ((float)(CALADC12_15V_85C-CALADC12_15V_30C));
    // Reset REFMSTR to hand over control of internal reference
    // voltages to ADC12_A control registers
    REFCTL0 &= ~REFMSTR;
    // Internal ref is on and set to 1.5V
    ADC12CTL0 = ADC12SHT0_9 | ADC12REFON | ADC12ON;
    ADC12CTL1 = ADC12SHP; // Enable sample timer
    ADC12MCTL0 = ADC12SREF_1 + ADC12INCH_10;
    __delay_cycles(100); // delay to allow Ref to settle
    ADC12CTL0 |= ADC12ENC; // Enable conversion
}

float get_temp_C(void){
    ADC12CTL0 &= ~ADC12SC; // clear the start bit
    ADC12CTL0 |= ADC12SC; // Sampling and conversion start
    // Single conversion (single channel)
    // Poll busy bit waiting for conversion to complete
    while (ADC12CTL1 & ADC12BUSY){
        __no_operation();
    }

    in_temp = ADC12MEM0; // Read results from conversion
    temperatureDegC=(float)(((long)in_temp-CALADC12_15V_30C) *degC_per_bit + 30.0;
    // Temperature in Fahrenheit = (9/5)*Tc + 32
    //temperatureDegF = temperatureDegC * 9.0/5.0 + 32.0;
    __no_operation(); // SET BREAKPOINT HERE
    return temperatureDegC;
}






