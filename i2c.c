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

#include "i2c.h"
#include <avr/io.h> 
#include <util/twi.h>
#include <util/delay.h>

// Initialize bit rate for the TWI interface
void avr_i2c_init(uint8_t bitrate)
{
	TWBR = bitrate;
}

// Transmit start condition, slave address, and R/W request
uint8_t avr_i2c_begin(uint8_t addr, uint8_t rw)
{
	if(rw != AVR_I2C_WRITE && rw != AVR_I2C_READ) goto error;

	// Use the two-wire control register to generate a start condition
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1<<TWEN);	
	// Wait for the TWI to ACK that the start condition has occurred
	while (!(TWCR & (1 << TWINT)));
	// Check the value of TWISTATUS to see if the start condition was successful
	if ((TWSR & 0xF8) != TW_START) goto error;
	// Load slave address + R/W bit into data register
	TWDR = (addr | rw);
	// Clear (set to 1) the TWINT bit in TWCR to transmit address frame 
	TWCR = (1 << TWINT) | (1 << TWEN);
	// Wait on TWINT to indicate that the address has been transmitted
	while (!(TWCR & (1 << TWINT)));
	// Check the TWI status register to see if the slave ACKed the request
	if (rw == AVR_I2C_WRITE && ((TWSR & 0xF8) != TW_MT_SLA_ACK)) goto error;
	if (rw == AVR_I2C_READ && ((TWSR & 0xF8) != TW_MR_SLA_ACK)) goto error;
	return 0;

error:
	return 1;
}

// Send transmition stop contition
void avr_i2c_end(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);	
}

uint8_t avr_i2c_write_byte(uint8_t val)
{
	// Value to be written
	TWDR = val;
	// Transmit the value
	TWCR = (1 << TWINT) | (1 << TWEN);
	// Wait for the transmission to complete
	while (!(TWCR & (1 << TWINT)));
	// Check that the slave ACKed the transaction
	if ((TWSR & 0xF8) != TW_MT_DATA_ACK) goto error;	
	return 0;

error:
	return 1;
}

uint8_t avr_i2c_write(uint8_t addr, uint8_t reg, uint8_t * buffer, uint8_t len)
{
	uint8_t i, result;
	if(!buffer) return 1;

	result = avr_i2c_begin(addr, AVR_I2C_WRITE);
	if(result) goto error;
	for(i = 0; i < len; i++){
		result = avr_i2c_write_byte(buffer[i]);
		if(result) goto error;
	}

error:
	avr_i2c_end();
	return result;
}

uint8_t avr_i2c_read_byte(uint8_t * val, uint8_t more)
{
	uint8_t ack_bit = 0;
	if(!val) goto error;
	if(more) ack_bit = 1;

	TWCR = (1 << TWINT) | (1 << TWEN) | (ack_bit << TWEA);	
	while (!(TWCR & (1 << TWINT)));
	*val = TWDR;
	return 0;

error:
	return 1;
}

uint8_t avr_i2c_read(uint8_t addr, uint8_t reg, uint8_t * buffer, uint8_t len)
{
	uint8_t i, result;
	if(!buffer) return 1;

	result = avr_i2c_begin(addr, AVR_I2C_WRITE);
	if(result) goto end;
	result = avr_i2c_write_byte(reg);
	if(result) goto end;
	avr_i2c_end();

	_delay_us(10);

	result = avr_i2c_begin(addr, AVR_I2C_READ);
	if(result) goto end;
	for(i = 0; i < len; i++){
		if(i < len - 1){
			result = avr_i2c_read_byte(buffer + i, 1);
			if(result) goto end;
		}else{
			result = avr_i2c_read_byte(buffer + i, 0);
		}
	}
end:
	avr_i2c_end();
	return result;
}

