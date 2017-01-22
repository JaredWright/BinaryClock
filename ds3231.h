#ifndef __DS3231_H
#define __DS3231_H

#include <stdint.h>

#define DS3231_I2C_ADDR (0x68 << 1)

__attribute__((packed)) struct ds3231_time {
	uint8_t second;	// 0 - 59
	uint8_t minute;	// 0 - 59
	uint8_t hour;	// 1 - 12 + AM/PM (12hr mode) || 0 - 23 (24hr mode)
	uint8_t day;	// Weekday (1-7)
	uint8_t date;	// 1 - 31
	uint8_t month;	// 1 - 12
	uint8_t year;	// 0 - 99
};

uint8_t ds3231_set_mode(uint8_t mode);
uint8_t ds3231_set_time(struct ds3231_time * t);
uint8_t ds3231_get_time(struct ds3231_time * t);

#endif

