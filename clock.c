#include <avr/io.h>
#include <util/delay.h>
#include "clock.h"
#include "led.h"
#include "ds3231.h"
#include "i2c.h"
 
//
// Private data
//
static led_strand_t * hour_strand;
static led_strand_t * minute_strand;
static led_strand_t * second_strand;
static struct rgb hour_color = {HOUR_G, HOUR_R, HOUR_B};
static struct rgb minute_color = {MINUTE_G, MINUTE_R, MINUTE_B};
static struct rgb second_color = {SECOND_G, SECOND_R, SECOND_B};
static struct rgb background_color = {BACKGROUND_G, BACKGROUND_R, BACKGROUND_B};
static struct rgb off_color;

//
// Private declarations
//
void draw_clock_face(led_strand_t * s, uint8_t value, struct rgb * color);
void set_bit_color(led_strand_t * s, uint8_t bit_number, struct rgb * color);
void draw_background(led_strand_t * s);

//
// Implementation
//
void clock_init()
{
	hour_strand = led_strand_init(CLOCK_STRAND_LEN, &PORTB, PINB0);
	minute_strand = led_strand_init(CLOCK_STRAND_LEN, &PORTB, PINB1);
	second_strand = led_strand_init(CLOCK_STRAND_LEN, &PORTB, PINB2);

	draw_background(hour_strand);
	draw_background(minute_strand);
	draw_background(second_strand);
}

void clock_show_time(uint8_t hour, uint8_t minute, uint8_t second)
{
	draw_clock_face(hour_strand, hour, &hour_color);
	draw_clock_face(minute_strand, minute, &minute_color);
	draw_clock_face(second_strand, second, &second_color);
}

void draw_clock_face(led_strand_t * s, uint8_t value, struct rgb * color)
{
	uint8_t i;
	for(i = 0; i < BITS_PER_STRAND; i++){
		if((value >> i) & 1){
			set_bit_color(s, i, color);
		}else{
			set_bit_color(s, i, &off_color);
		}
	}

	led_strand_draw(s);	
}

void set_bit_color(led_strand_t * s, uint8_t bit_number, struct rgb * color)
{
	uint8_t led_count = led_strand_get_count(s);
	uint8_t bit_position = (led_count - (bit_number * (LED_BIT_LEN + LED_SPACER_LEN))) - LED_BIT_LEN;
	uint8_t i;

	for(i = 0; i < LED_BIT_LEN; i++){
		led_strand_set(s, bit_position + i, color);
	}
}

void draw_background(led_strand_t * s)
{
	uint8_t i;
	if(!s) return;

	for(i = 0; i < CLOCK_STRAND_LEN; i++){
		led_strand_set(s, i, &background_color);
	}
	led_strand_draw(s);
}

