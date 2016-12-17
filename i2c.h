#ifndef __I2C_H
#define __I2C_H

#include <stdint.h>

uint8_t avr_i2c_read_byte(uint16_t address, uint16_t offset, uint8_t * value);
uint8_t avr_i2c_write_byte(uint16_t address, uint16_t offset, uint8_t value);

#endif

