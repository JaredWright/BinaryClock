//
//	Helper functions for drawing colors on the face of the clock
//

#ifndef __CLOCK_H
#define __CLOCK_H

#include <stdint.h>

#define CLOCK_STRAND_LEN 59	// Total number of LEDs per strand on the clock's face
#define BITS_PER_STRAND 6	// Number of "bits" per strand on each clock face
#define LED_BIT_LEN 5		// Number of LEDs per "bit" on the clock's face
#define LED_SPACER_LEN 5	// Number of LEDs between "bits" on the clock's face

// RGB colors for the clock face 
#define HOUR_R 255
#define HOUR_G 0
#define HOUR_B 0

#define MINUTE_R 0
#define MINUTE_G 255
#define MINUTE_B 0

#define SECOND_R 255
#define SECOND_G 255
#define SECOND_B 255

#define BACKGROUND_R 0
#define BACKGROUND_G 255
#define BACKGROUND_B 255

// Initilize the clock face
void clock_init();
void clock_show_time(uint8_t hour, uint8_t minute, uint8_t second);

#endif
