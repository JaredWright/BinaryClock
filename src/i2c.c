#include "i2c.h"
#include <avr/io.h> 
#include <util/twi.h>
#include <util/delay.h>

//
// Private declarations
//
#define AVR_I2C_READ		1
#define AVR_I2C_WRITE		0

// Transmit a start condition, the 7 bit slave address, and R/W bit
uint8_t avr_i2c_begin(uint8_t addr, uint8_t rw);

// Send transmition stop contition
void avr_i2c_end(void);

// Read one byte from the active slave on the I2C bus into buffer at address val
// If argument more > 0, an ACK will be transmitted, resulting in the slave
// sending more data if available. If more = 0, no ack will be generated and the
// active slave will stop transmitting 
uint8_t avr_i2c_read_byte(uint8_t * val, uint8_t more);

// Writes one byte to the active slave on the I2C bus 
uint8_t avr_i2c_write_byte(uint8_t val);


//
// Implementation
//
void avr_i2c_init(uint8_t bitrate)
{
	TWBR = bitrate;
}

uint8_t avr_i2c_begin(uint8_t addr, uint8_t rw)
{
	if(rw != AVR_I2C_WRITE && rw != AVR_I2C_READ) goto error;
	if((addr & 0x80) > 0) goto error; // Check that address is 7 bits

	// Use the two-wire control register to generate a start condition
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1<<TWEN);	
	// Wait for the TWI to ACK that the start condition has occurred
	while (!(TWCR & (1 << TWINT)));
	// Check the value of TWISTATUS to see if the start condition was successful
	if ((TWSR & 0xF8) != TW_START) goto error;
	// Load 7 bit slave address + R/W bit into data register
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
	if(result) goto error;
	result = avr_i2c_write_byte(reg);
	if(result) goto error;
	avr_i2c_end();

	_delay_us(10);

	result = avr_i2c_begin(addr, AVR_I2C_READ);
	if(result) goto error;
	for(i = 0; i < len; i++){
		if(i < len - 1){
			result = avr_i2c_read_byte(buffer + i, 1);
			if(result) goto error;
		}else{
			result = avr_i2c_read_byte(buffer + i, 0);
		}
	}
error:
	avr_i2c_end();
	return result;
}

