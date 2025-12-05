/*
 * sensor.h
 *
 *  Created on: Dec 3, 2025
 *      Author: lmshraybman
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#define CALADC12_15V_30C *((unsigned int *)0x1A1A)
#define CALADC12_15V_85C *((unsigned int *)0x1A1C)
#define MOVING_AVG_LENGTH 36

void setupADC(void);
float get_Temp_C(void);
float get_Temp_AVG_C(void);
float get_Temp_AVG_F(void);
float get_Pot_Scaled(int min, int max);


#endif /* SENSOR_H_ */
