/*
 * sensor.c
 *
 *  Created on: Dec 3, 2025
 *      Author: lmshraybman
 */

#include <msp430.h>
#include "sensor.h"

float tempC_array[MOVING_AVG_LENGTH];

volatile unsigned int temp_reading;
volatile unsigned int pot_reading;

void setupADC(void)
{
    // Disable master reference voltage
    REFCTL0 &= ~REFMSTR;

    // Set Sample/Hold time to 384 cycles, enable adc internal reference, set adv internal reference to 2.5v, turn on adc, enable continuous sampling.
    ADC12CTL0 = ADC12SHT0_9 | ADC12REFON | ADC12REF2_5V | ADC12ON | ADC12MSC;

    // Set Sample/Hold to use sampling timer, enable sequence of channels sampling
    ADC12CTL1 = ADC12SHP + ADC12CONSEQ_3;

    // Set register 0 reference to 0-3.3v, input channel 0 (pin A0, P6.0)
    ADC12MCTL0 = ADC12SREF_0 + ADC12INCH_0;
    // ADC12MCTL1: EOS = 1, SREF =001 = voltage refs = GND to Vref+
    // INCHx = 1010

    // Set register 1 reference to 0-2.5v, input channel 10 (temp diode), last conversion in sequence
    ADC12MCTL1 = ADC12SREF_1 + ADC12INCH_10 + ADC12EOS;

    // Set Port 6 Pins 0 to FUNCTION mode (=1) for ADC12 register 0
    // (Current sensor input is on analog input A0)
    P6SEL = P6SEL | BIT0;

    // Enable interrupts for register 1
    ADC12IE = ADC12IE1;

    //Start and enable conversion
    ADC12CTL0 |= ADC12SC + ADC12ENC;
}
float get_Temp_C(void)
{
    return (((long) temp_reading - CALADC12_25V_30C) * (85 - 30))
            / (CALADC12_25V_85C - CALADC12_25V_30C) + 30.0;
}

void updateTempAverage(unsigned long time)
{
    //run every second
    float new_temp = get_Temp_C();
    int average_time_idx = time % MOVING_AVG_LENGTH;

    tempC_array[average_time_idx] = new_temp;
}

float get_Temp_AVG_C(void)
{
    float sum = 0.0;
    int non_zero_temps = 0;
    for (int i = 0; i < MOVING_AVG_LENGTH; i++)
    {
        if (tempC_array[i] != 0)
        {
            sum += tempC_array[i];
            non_zero_temps++;
        }
    }
    return sum / non_zero_temps;
}
float get_Temp_AVG_F(void)
{
    return (get_Temp_AVG_C() * 9 / 5) + 32;
}

int get_pot_scaled(int min, int max)
{
    return min + ((pot_reading * (max - min)) / 4095);
}

#pragma vector=ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
    if (ADC12IFG & ADC12IFG0)
    {
        pot_reading = ADC12MEM0 & 0x0FFF;
    }
    if (ADC12IFG & ADC12IFG1)
    {
        temp_reading = ADC12MEM1 & 0x0FFF;
    }
}

