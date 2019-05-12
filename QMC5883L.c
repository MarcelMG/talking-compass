/*	QMC5883L I²C magnetometer
 *
 *	written in 2019 by Marcel Meyer-Garcia
 *  see LICENCE.txt
 */

#include "QMC5883L.h"

// I²C slave & register addresses
uint8_t QMC5883L_ADRESS = 0x0D;
uint8_t X_DATA_LSB = 0x00;
uint8_t X_DATA_MSB = 0x01;
uint8_t Y_DATA_LSB = 0x02;
uint8_t Y_DATA_MSB = 0x03;
uint8_t Z_DATA_LSB = 0x04;
uint8_t Z_DATA_MSB = 0x05;
uint8_t TEMP_DATA_LSB = 0x07;
uint8_t TEMP_DATA_MSB = 0x08;
uint8_t CONTROL_REGISTER_1 = 0x09;
uint8_t STATUS_REGISTER_1 = 0x06;
uint8_t SET_RESET_PERIOD_REGISTER = 0x0B;

// initialize the I2C1 peripheral and configure the magnetometer
uint8_t init_QMC5883L(){
	// init the I2C1 peripheral and the SDA/SCL GPIO pins
	init_i2c1();
	uint8_t buf[2];
	// init QMC5883L
	buf[0] = SET_RESET_PERIOD_REGISTER;
	buf[1] = 0x01;
	uint8_t i2c_err = i2c_write(QMC5883L_ADRESS, buf, 2);
	if(i2c_err) return i2c_err;
	buf[0] = CONTROL_REGISTER_1;
	// continuous mode, 200Hz data rate, 8G scale, 512 oversampling ratio
	buf[1] = 0b00011101;
	i2c_err = i2c_write(QMC5883L_ADRESS, buf, 2);
	if(i2c_err) return i2c_err;
	return 0;
}

// returns 1 if a new data sample is available, 0 if not
uint8_t data_ready_QMC5883L(){
	uint8_t ready = 0;
	i2c_write(QMC5883L_ADRESS, &STATUS_REGISTER_1, 1);
	uint8_t buf;
	i2c_read(QMC5883L_ADRESS, &buf, 1);
	ready = buf & (1<<0);
	return ready;
}

// retrieve a new data sample
magnetometer_data get_raw_QMC5883L(){
	magnetometer_data data = {0, 0, 0, 0};
	uint8_t buf;
	// read x-data
	i2c_write(QMC5883L_ADRESS, &X_DATA_LSB, 1);
	i2c_read(QMC5883L_ADRESS, &buf, 1);
	data.x = buf;
	i2c_write(QMC5883L_ADRESS, &X_DATA_MSB, 1);
	i2c_read(QMC5883L_ADRESS, &buf, 1);
	data.x |= (buf << 8);
	// read y-data
	i2c_write(QMC5883L_ADRESS, &Y_DATA_LSB, 1);
	i2c_read(QMC5883L_ADRESS, &buf, 1);
	data.y = buf;
	i2c_write(QMC5883L_ADRESS, &Y_DATA_MSB, 1);
	i2c_read(QMC5883L_ADRESS, &buf, 1);
	data.y |= (buf << 8);
	// read z-data (NOTE: if we don't read the z-data MSB, the QMC5883L won't
	//update the values, so we have to read it even if we don't need it
	i2c_write(QMC5883L_ADRESS, &Z_DATA_LSB, 1);
	i2c_read(QMC5883L_ADRESS, &buf, 1);
	data.z = buf;
	i2c_write(QMC5883L_ADRESS, &Z_DATA_MSB, 1);
	i2c_read(QMC5883L_ADRESS, &buf, 1);
	data.z |= (buf << 8);
	// read temperature data; coefficient is 100 LSB/°C
	i2c_write(QMC5883L_ADRESS, &TEMP_DATA_LSB, 1);
	i2c_read(QMC5883L_ADRESS, &buf, 1);
	data.temperature = buf;
	i2c_write(QMC5883L_ADRESS, &TEMP_DATA_MSB, 1);
	i2c_read(QMC5883L_ADRESS, &buf, 1);
	data.temperature |= (buf << 8);
	return data;
}

// retrieve a new data sample and calculate the orientation in the (x,y)-plane,
// returns value in degrees 0..360
uint16_t get_angle_QMC5883L(){
	// wait until new data is ready
	while(!data_ready_QMC5883L());
	magnetometer_data data = {0, 0, 0, 0};
	data = get_raw_QMC5883L();
	double orientation;
	// check if x or y are 0, this would cause an error in atan2f()
	if(data.x==0.0 || data.y==0.0){
		orientation = 0;
	}else{
		// calculate orientation in degrees
		orientation = atan2f((double)data.y, (double)data.x) * 180/M_PI;
	}
	// constrain the angles between 0° and 360°
	if(orientation > 360.0) orientation -= 360.0;
	else if(orientation < 0.0) orientation += 360.0;
	return (uint16_t) orientation;
}





