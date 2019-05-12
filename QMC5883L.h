/*	QMC5883L I²C magnetometer
 *
 *	written in 2019 by Marcel Meyer-Garcia
 *  see LICENCE.txt
 */

#ifndef QMC5883L_H_
#define QMC5883L_H_

#include "stm32f1xx.h"
#include "init.h"
#include "i2c.h"
#include <stdlib.h>
#include <math.h> // for the atan2() function

// I²C slave & register addresses
uint8_t QMC5883L_ADRESS;
uint8_t X_DATA_LSB;
uint8_t X_DATA_MSB;
uint8_t Y_DATA_LSB;
uint8_t Y_DATA_MSB;
uint8_t Z_DATA_LSB;
uint8_t Z_DATA_MSB;
uint8_t TEMP_DATA_LSB;
uint8_t TEMP_DATA_MSB;
uint8_t CONTROL_REGISTER_1;
uint8_t STATUS_REGISTER_1;
uint8_t SET_RESET_PERIOD_REGISTER;
uint8_t buf[2];
// structure for one data sample with field-strength values and temperature
typedef struct{
	int16_t x;
	int16_t y;
	int16_t z;
	int16_t temperature;
}magnetometer_data;

uint8_t init_QMC5883L(void);
uint8_t data_ready_QMC5883L(void);
magnetometer_data get_raw_QMC5883L(void);
uint16_t get_angle_QMC5883L(void);



#endif /* QMC5883L_H_ */
