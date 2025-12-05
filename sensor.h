/*
 * sensor.h
 *
 *  Created on: Dec 3, 2025
 *      Author: lmshraybman
 */

#ifndef SENSOR_H_
#define SENSOR_H_

// Temperature Sensor Calibration readings for 2.5V from TLV
#define CALADC12_25V_30C *((unsigned int *)0x1A22)
#define CALADC12_25V_85C *((unsigned int *)0x1A24)
#define MOVING_AVG_LENGTH 36

void setupADC(void);
float get_Temp_C(void);
void updateTempAverage(unsigned long time);
float get_Temp_AVG_C(void);
float get_Temp_AVG_F(void);

int get_Pot_Scaled(int min, int max);


#endif /* SENSOR_H_ */
