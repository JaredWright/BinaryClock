#include <avr/io.h>
#include <util/delay.h>
#include "led.h"
 
#define BLINK_DELAY_MS 1000
#define ONBOARD_LED_ON() (PORTB |= (1 << DDB5))
#define ONBOARD_LED_OFF() (PORTB &= ~(1 << DDB5))

void setup()
{
	// Setup pins 0 and 5 of PORTB as outputs
	DDRB = (1 << DDB0) | (1 << DDB5);
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
	while(1){
		swirl(leds);
	}	
}

int main (void)
{
	setup();
	do_clock();
}
