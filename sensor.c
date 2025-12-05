/*
 * sensor.c
 *
 *  Created on: Dec 3, 2025
 *      Author: lmshraybman
 */

#include <msp430.h>
#include "sensor.h"

unsigned int in_current,in_temp;
volatile float milliamps, tempC;
float tempC_array[MOVING_AVG_LENGTH];


void setupADC(void){
    REFCTL0 &= ~REFMSTR;

    ADC12CTL0=ADC12SHT0_9|ADC12REFON|ADC12REF2_5V|ADC12ON|ADC12MSC;

    ADC12CTL1 = ADC12SHP+ADC12CONSEQ_1;

    ADC12MCTL0 = ADC12SREF_1 + ADC12INCH_0;
    // ADC12MCTL1: EOS = 1, SREF =001 = voltage refs = GND to Vref+
    // INCHx = 1010
    ADC12MCTL1 = ADC12SREF_1 + ADC12INCH_10 + ADC12EOS;
    // Set Port 6 Pins 0 to FUNCTION mode (=1) for ADC12
    // (Current sensor input is on analog input A0)
    P6SEL = P6SEL | BIT0;
}
float get_Temp_C(void){
    ADC12CTL0 |= ADC12SC + ADC12ENC;
    while (ADC12CTL1 & ADC12BUSY) // poll busy bit
        __no_operation();
    in_temp = ADC12MEM1 & 0x0FFF; // keep only low 12 bits
    tempC = (float)(((long)in_temp-CALADC12_25V_30C)*(85 - 30))/
            (CALADC12_25V_85C - CALADC12_25V_30C) + 30.0;
    return tempC;
}

void updateTempAverage(unsigned long time){
    //run every second
    float new_temp = get_Temp_C();
    int average_time_idx = time % MOVING_AVG_LENGTH;

    tempC_array[average_time_idx] = new_temp;
}

float get_Temp_AVG_C(void){
    float sum = 0.0;
    int non_zero_temps = 0;
    for (int i = 0; i < MOVING_AVG_LENGTH; i++){
        if (tempC_array[i] != 0){
            sum += tempC_array[i];
            non_zero_temps++;
        }
    }

    return sum / non_zero_temps;
}
float get_Temp_AVG_F(void){
    float temp_C = get_Temp_AVG_C();
    return (temp_C * 9/5) + 32;
}
int get_Pot_Scaled(int min, int max){
    ADC12CTL0 |= ADC12SC + ADC12ENC;
    while (ADC12CTL1 & ADC12BUSY) // poll busy bit
        __no_operation();

    in_current = ADC12MEM0 & 0x0FFF;
    //Scale in_current to the range min-max
    int scaled = min + ((in_current * (max - min)) / 4095);

    return scaled;
}







