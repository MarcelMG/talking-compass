/*	SPI FLASH FILE TABLE
 *	written in 2019 by Marcel Meyer-Garcia
 *  see LICENCE.txt
 */

#ifndef FILES_H_
#define FILES_H_

// addresses and lengths of the audio samples stored in the flash memory
// sampling freq. is 16kHz
#define NORTH_ADDR 0x000000
#define NORTH_LEN 6887
#define EAST_ADDR 0x001B00
#define EAST_LEN 6198
#define SOUTH_ADDR 0x003400
#define SOUTH_LEN 6152
#define WEST_ADDR 0x004D00
#define WEST_LEN 7199
#define WELCOME_ADDR 0x006A00
#define WELCOME_LEN 42906

#endif /* FILES_H_ */
