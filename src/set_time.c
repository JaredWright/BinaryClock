#include "ds3231.h"
#include "i2c.h"


int main (void)
{
	struct ds3231_time t; 

	t.year = 0;
	t.month = 9;
	t.date = 15;
	t.day = 6;
	t.hour = 12;
	t.minute = 0;
	t.second = 0;

	avr_i2c_init(TWI_BIT_RATE);
	ds3231_set_mode(0);
	ds3231_set_time(&t);
}
