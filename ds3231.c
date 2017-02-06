#include "ds3231.h"
#include "i2c.h"

//
// Implementation
//
uint8_t ds3231_set_mode(uint8_t mode)
{
	uint8_t value = ((mode & 0x01) << 6);
	if(mode != 0 && mode != 1) return 1;
	return avr_i2c_write(DS3231_I2C_ADDR, 0x2, &value, 1);
}

uint8_t ds3231_set_time(struct ds3231_time * t)
{
	if(!t) return 1;
	return avr_i2c_write(DS3231_I2C_ADDR, 0, (uint8_t *)t, sizeof(struct ds3231_time));
}

uint8_t ds3231_get_time(struct ds3231_time * t)
{
	if(!t) return 1;
	return avr_i2c_read(DS3231_I2C_ADDR, 0, (uint8_t *)t, sizeof(struct ds3231_time)); 
}

