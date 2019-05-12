/*	PWM AUDIO PLAYER
 * 	plays audio files from SPI flash memory chip
 * 	output is on pin PA0
 *
 *	written in 2019 by Marcel Meyer-Garcia
 *  see LICENCE.txt
 */

#ifndef AUDIOPLAYER_H_
#define AUDIOPLAYER_H_

#include "W25Q64JV.h" //SPI flash memory

volatile uint8_t audioplayer_busy;
volatile uint32_t sample_counter;
volatile uint32_t audio_length;

void TIM1_UP_IRQHandler(void);
void audioplayer_setup(uint32_t samplerate);
void playback(uint32_t address, uint32_t length);

#endif /* AUDIOPLAYER_H_ */
