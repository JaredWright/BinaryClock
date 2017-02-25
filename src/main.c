#include "clock.h"
#include "led.h"
#include "ds3231.h"
#include "i2c.h"
#include <util/delay.h>

static struct ds3231_time t; 

int main (void)
{
	avr_i2c_init(TWI_BIT_RATE);
	clock_init();

	while(1){
		ds3231_get_time(&t);
		// if(t.month
		// TODO: Adjust daylight savings
		clock_show_time(t.hour, t.minute, t.second);
		_delay_ms(100);
	}
}
