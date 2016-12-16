#include "led.h"
#include <stdlib.h>

// Structure represents one WS2812B RGB LED
// A WS2812B reads values in the order G-R-B, so they are orginized in
// memory as such
__attribute__((packed)) struct rgb_led { 
	uint8_t g_val;
	uint8_t r_val;
	uint8_t b_val;
};

// Strucutre represents a chain of WS2812B RGB LEDs connected together in series
struct led_strand {
	struct rgb_led * led_buffer;
	uint16_t led_count;
};

led_strand_t * led_strand_init(uint8_t count)
{
	struct led_strand * leds = malloc(sizeof(struct led_strand));
	if(!leds) return NULL;

	leds->led_count = count;
	leds->led_buffer = malloc(sizeof(struct rgb_led) * count);
	led_strand_clear(leds);

	return leds;
}

void led_strand_free(led_strand_t * leds)
{
	if(!leds) return;
	if(leds->led_buffer) free(leds->led_buffer);
	free(leds);
}

int led_strand_set(led_strand_t * leds, uint8_t index, uint8_t r, uint8_t g, uint8_t b)
{
	struct rgb_led * led = NULL;

	if(!leds) return -1;
	if(index > leds->led_count) return -1;
	
	led = leds->led_buffer + index;
	led->r_val = r;
	led->g_val = g;
	led->b_val = b;	
	return 0;
}

void led_strand_clear(led_strand_t * leds)
{
	if(!leds || !leds->led_buffer) return;
	memset(leds->led_buffer, 0, leds->led_count * sizeof(struct rgb_led));
}

extern void ws2812b_strand_draw(uint8_t * buffer, uint16_t size);
int led_strand_draw(led_strand_t * leds)
{
	if(!leds || !leds->led_buffer) return -1;
	ws2812b_strand_draw((uint8_t *)(leds->led_buffer),
			leds->led_count * sizeof(struct rgb_led));
	return 0;
}

