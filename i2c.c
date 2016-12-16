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

uint8_t avr_i2c_read_byte(uint16_t address, uint16_t offset, uint8_t * value)
{
	if(!value) return -1;
	// Use the two-wire control register to generate a start condition
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1<<TWEN);	
	// Wait for the TWI to ack that the start condition has occurred
	while (!(TWCR & (1 << TWINT)));
	// Check the value of TWISTATUS to see if the start condition was successful
	if ((TWSR & 0xF8) != START) goto error;	
	// Transmit address frame with R/W bit set to read
	// To do this, first load the two-wire data register with "SLA_R"
	TWDR = 0x00;	// SLA_R??
	// Next, clear (set to 1) the TWINT bit in TWCR to start the address transmission
	TWCR = (1 << TWINT) | (1 << TWEN);
	// Wait on TWINT to be set to indicate that the address has been transmitted
	while (!(TWCR & (1 << TWINT)));
	// Check the two-wire status register to see if the slave ACKed the request
	if ((TWSR & 0xF8) != MT_SLA_ACK) goto error;
	// Wait for the slave to send data, then ACK the transaction
	// TODO
	// *value = whatever came back
error:
	return -1;
}

uint8_t avr_i2c_write_byte(uint16_t address, uint16_t offset, uint8_t value)
{
	if(!address) return -1;

	// Use the two-wire control register to generate a start condition
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1<<TWEN);	
	// Wait for the TWI to ack that the start condition has occurred
	while (!(TWCR & (1 << TWINT)));
	// Check the value of TWISTATUS to see if the start condition was successful
	if ((TWSR & 0xF8) != START) goto error;	
	// Transmit address frame with R/W bit set to read
	// To do this, first load the two-wire data register with "SLA_W"
	TWDR = (offset);	// TODO: How to add R/W bit?? Manual says "SLA_W"
	// Next, clear (set to 1) the TWINT bit in TWCR to start the address transmit
	TWCR = (1 << TWINT) | (1 << TWEN);
	// Wait on TWINT to be set to indicate that the address has been transmitted
	while (!(TWCR & (1 << TWINT)));
	// Check the two-wire status register to see if the slave ACKed the request
	if ((TWSR & 0xF8) != MT_SLA_ACK) goto error;
	// Transmit one byte to the slave
	// Load value into the two-wire data register, then set the TWINT flag
	TWDR = value;
	TWCR = (1 << TWINT) | (1 << TWEN);	
	// Wait for TWINT to be set, which indicates the data transmit happened
	while (!(TWCR & (1 << TWINT)));
	// Check status register for success, and if ACK/NAK was received
	if ((TWSR & 0xF8) != MT_SLA_ACK) goto error;
	// Send transmition stop contition
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);	
	return 0;
error:
	return -1;
}
