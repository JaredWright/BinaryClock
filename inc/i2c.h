// Driver for the TWI (two-wire interface) on an AVR processor.
// TWI is used to implement I2C communictions in all I2C modes
// 
// AVR TWI register overview:
// --------------------------------------------
// 	TWCR = Two-wire control register:
// 	
// 	Bit		  7       6      5       4      3       2     1     0
// 	Name  | TWINT | TWEA | TWSTA | TWSTO | TWWC | TWEN |  -  | TWIE  |
// 	R/W      R/W     R/W    R/W     R/W     R      R/W    R     R/W
//
// 		TWINT = Two-wire interupt bit
// 		TWEA = Two-wire enable acknowledge (i.e. transmit an ACK)
// 		TWSTA = Two-wire start condition bit
// 		TWSTO = Two-wire stop condition bit
// 		TWWC = Two-wire write collision (data transmitted/received as same time)
// 		TWEN = Two-wire interface enable bit
// 		TWIE = Two-wire interupt enable bit
// --------------------------------------------
// 	TWSR = Two-wire status register
// 		Bits 7:3 = Status code bits
// 		Bit 2 = reserved
// 		Bits 1:0 = bit rate prescalar value
// --------------------------------------------
// 	TWDR = Two-wire data register
// 		Used as the transmit and recieve buffer for read/write operations 
// --------------------------------------------
// 	TWBR = Two-wire bit rate register
// 		Determines the clock speed of TWI
// --------------------------------------------
// 	TWAR = Two-wire slave address register
// 		This register is used to assign a slave address that this TWI interface
// 		will repond to while acting in slave mode
// --------------------------------------------

#ifndef __I2C_H
#define __I2C_H

#include <stdint.h>

#define TWI_BIT_RATE 10		// Smallest value (fastest rate)

#define AVR_I2C_SUCCESS		0
#define AVR_I2C_ERROR		1
typedef uint8_t AVR_I2C_STATUS;

// Initialize bit rate for the TWI interface
void avr_i2c_init(uint8_t bitrate);

// Read len bytes from the device at address addr, offset reg into the supplied buffer
// Address must be 7 bits
// Returns 0 for success, 1 for failure
AVR_I2C_STATUS avr_i2c_read(uint8_t addr, uint8_t reg, uint8_t * buffer, uint8_t len);

// Write len bytes from buffer to the device at address addr, offset reg
// Address must be 7 bits
AVR_I2C_STATUS  avr_i2c_write(uint8_t addr, uint8_t reg, uint8_t * buffer, uint8_t len);

#endif

