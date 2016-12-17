#ifndef __I2C_H
#define __I2C_H

#include <stdint.h>

#define AVR_I2C_READ 1
#define AVR_I2C_WRITE 0

uint8_t avr_i2c_read(uint8_t addr, uint8_t reg, uint8_t * buffer, uint8_t len);
uint8_t avr_i2c_write(uint8_t addr, uint8_t reg, uint8_t * buffer, uint8_t len);

#endif

