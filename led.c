#include "led.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <avr/io.h>

//
// Private declarations
//
struct led_strand {
	struct rgb * led_buffer;
	uint16_t led_count;
	volatile uint8_t * out_port;
	uint8_t out_pin;
};

// Low-level LED driver (ws2812b.S)
extern void ws2812b_strand_draw(uint8_t * buffer, uint16_t size,
		volatile uint8_t * out_port, uint8_t out_pin);

//
// Implementation
//
led_strand_t * led_strand_init(uint8_t count, volatile uint8_t * port, uint8_t pin)
{
	struct led_strand * leds = malloc(sizeof(struct led_strand));
	if(!leds) goto error;
	leds->led_buffer = malloc(sizeof(struct rgb) * count);
	if(!leds->led_buffer) goto error;

	leds->led_count = count;
	leds->out_port = port;
	leds->out_pin = 1 << pin;

	led_strand_clear(leds);

	// Set the specified port/pin as an output 
	if(pin > 7) goto error;
	if(port == &PORTA){
		DDRA |= (1 << pin);
	}else if(port == &PORTB){
		DDRB |= 1 << pin;
	}else if (port == &PORTC){
		DDRC |= 1 << pin;
	}else if(port == &PORTD){
		DDRD |= 1 << pin;
	}else{
		goto error;
	}

	return leds;
error:
	led_strand_free(leds);
	return NULL;	
}

void led_strand_free(led_strand_t * leds)
{
	if(!leds) return;
	if(leds->led_buffer) free(leds->led_buffer);
	free(leds);
}

int led_strand_set(led_strand_t * leds, uint8_t index, struct rgb * color)
{
	struct rgb * led = NULL;

	if(!leds || !color) return -1;
	if(index > leds->led_count) return -1;
	
	led = leds->led_buffer + index;
	led->r = color->r;
	led->g = color->g;
	led->b = color->b;	
	return 0;
}

void led_strand_clear(led_strand_t * leds)
{
	if(!leds || !leds->led_buffer) return;
	memset(leds->led_buffer, 0, leds->led_count * sizeof(struct rgb));
}

int led_strand_draw(led_strand_t * leds)
{
	if(!leds || !leds->led_buffer) return -1;
	ws2812b_strand_draw((uint8_t *)(leds->led_buffer),
			leds->led_count * sizeof(struct rgb), leds->out_port, leds->out_pin);
	return 0;
}

uint8_t led_strand_get_count(led_strand_t * leds)
{
	if(!leds) return 0;
	return leds->led_count;
}

