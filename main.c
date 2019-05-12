/*	-SPEAKING COMPASS-
 *  audio playback with PWM and the Winbond W25Q64JV SPI flash memory
 *	using magnetometer QMC5883Lto sense orientation
 *  connections:
 *  	W25Q64JV:
 *			PB3 -> CLK
 *			PB4 -> MISO (DO/IO1)
 *			PB5 -> MOSI (DI/IO0)
 *			PA4 -> CS
 *			Vdd -> 3.3V
 *			Vref-> 3.3V
 *		PA0 -> PWM output
 *		QMC5883L:
 *			PB6 -> SCL
 *			PB7 -> SDA
 *			VCC -> 3.3V
 *
 *	written in 2019 by Marcel Meyer-Garcia
 *  see LICENCE.txt
 */

#include "stm32f1xx.h"
#include "init.h"
#include "QMC5883L.h"
#include "files.h"
#include "audioplayer.h"

int main(void)
{
	// Initialize system timer for 1ms ticks (else divide by 1e6 for µs ticks)
	SysTick_Config(SystemCoreClock / 1e3);
	// setup the PWM audioplayer with a sampling rate of 16kHz
	audioplayer_setup(16000);
	// init the magnetometer
	init_QMC5883L();

	playback(WELCOME_ADDR, WELCOME_LEN);
	while(audioplayer_busy);

	while(1){

		uint16_t orientation = get_angle_QMC5883L();
		if( (orientation>=337) || (orientation<=23) ){
			playback(NORTH_ADDR, NORTH_LEN);
			while(audioplayer_busy);
		}else if( (orientation>293) && (orientation<=337) ){
			playback(NORTH_ADDR, NORTH_LEN);
			while(audioplayer_busy);
			playback(WEST_ADDR, WEST_LEN);
			while(audioplayer_busy);
		}else if( (orientation>248) && (orientation<=293) ){
			playback(WEST_ADDR, WEST_LEN);
			while(audioplayer_busy);
		}else if( (orientation>203) && (orientation<=248) ){
			playback(SOUTH_ADDR, SOUTH_LEN);
			while(audioplayer_busy);
			playback(WEST_ADDR, WEST_LEN);
			while(audioplayer_busy);
		}else if( (orientation>158) && (orientation<=203) ){
			playback(SOUTH_ADDR, SOUTH_LEN);
			while(audioplayer_busy);
		}else if( (orientation>113) && (orientation<=158) ){
			playback(SOUTH_ADDR, SOUTH_LEN);
			while(audioplayer_busy);
			playback(EAST_ADDR, EAST_LEN);
			while(audioplayer_busy);
		}else if( (orientation>68) && (orientation<=113) ){
			playback(EAST_ADDR, EAST_LEN);
			while(audioplayer_busy);
		}else if( (orientation>0) && (orientation<=68) ){
			playback(NORTH_ADDR, NORTH_LEN);
			while(audioplayer_busy);
			playback(EAST_ADDR, EAST_LEN);
			while(audioplayer_busy);
		}
		delay(1000);
	}
	return 0;
}
