// Read/write operations for the DS3231 real-time clock
// Time is encoded in binary coded decimal format

// Toggle the clock between 12 and 24 hour mode
// 		mode = 0 for 24 hour mode
// 		mode = 1 for 12 hour mode
int ds3231_set_mode(uint8_t mode)
{
	if(mode != 0 || mode != 1) return -1;
	return avr_i2c_write_byte(0x00 /* ?? */, 0x02, (mode << 6));
}

// Set the internal clock of the DS3231 to the values of the given ds3231_time
void ds3231_set_time(struct ds3231_time * t)
{
	if(!t) return -1;

	avr_i2c_write_byte(0x00 /* ?? */, 0, t->second);
	avr_i2c_write_byte(0x00 /* ?? */, 1, t->minute);
	avr_i2c_write_byte(0x00 /* ?? */, 2, t->hour);
	avr_i2c_write_byte(0x00 /* ?? */, 3, t->day);
	avr_i2c_write_byte(0x00 /* ?? */, 4, t->date);
	avr_i2c_write_byte(0x00 /* ?? */, 5, t->month);
	avr_i2c_write_byte(0x00 /* ?? */, 6, t->year);
	return 0;
}

// Read the current time into the given ds3231_time structure
// Return 0 for success, 1 for failure
int ds3231_get_time(struct ds3231_time * t)
{
	if(!t) return -1;
	struct ds3231_time temp = { 0 };

	avr_i2c_read_byte(0x00 /* ?? */, 0, &(t->second));
	avr_i2c_read_byte(0x00 /* ?? */, 1, &(t->minute));
	avr_i2c_read_byte(0x00 /* ?? */, 2, &(t->hour));
	avr_i2c_read_byte(0x00 /* ?? */, 3, &(t->day));
	avr_i2c_read_byte(0x00 /* ?? */, 4, &(t->date));
	avr_i2c_read_byte(0x00 /* ?? */, 5, &(t->month));
	avr_i2c_read_byte(0x00 /* ?? */, 6, &(t->year));
	return 0;
}

