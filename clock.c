#include <avr/io.h>

#include <util/delay.h>
#include "led.h"
#include "ds3231.h"
 
#define BLINK_DELAY_MS 1000
#define ONBOARD_LED_ON() (PORTB |= (1 << DDB5))
#define ONBOARD_LED_OFF() (PORTB &= ~(1 << DDB5))

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define F_SCL 100000UL
#define TWI_PRESCALAR 1
// #define TWI_BIT_RATE ((((F_CPU / F_SCL) / TWI_PRESCALAR) - 16 ) / 2)
#define TWI_BIT_RATE 10		// Smallest value (fastest rate)

void setup()
{
	// Setup PORTB, pin 0 as output for led
	DDRB = (1 << DDB0) /* | (1 << DDB4) | (1 << DDB5) */ ;
	avr_i2c_init(TWI_BIT_RATE);
}

void swirl(led_strand_t * leds)
{
	uint8_t i = 0;
	if(!leds) return;
	for(i = 0; i < 255; i++){
		led_strand_set(leds, 0, i, 255 - i, 0);
		led_strand_draw(leds);
		_delay_ms(20);
	}
	for(i = 0; i < 255; i++){
		led_strand_set(leds, 0, 255 - i, 0, i);
		led_strand_draw(leds);
		_delay_ms(20);
	}
	for(i = 0; i < 255; i++){
		led_strand_set(leds, 0, 0, i, 255 - i);
		led_strand_draw(leds);
		_delay_ms(20);
	}
}

void do_clock()
{
	led_strand_t * leds = led_strand_init(1);
	struct ds3231_time t; 
	while(1){
		// i2c_test();
		ds3231_get_time(&t);
		// swirl(leds);
	}	
}

int main (void)
{
	setup();
	do_clock();
}
