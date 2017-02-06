//
// Helper functions that use the ws2812b driver to draw strands of LEDs
//

#ifndef __LED_H
#define __LED_H

#include <stdint.h>

// Structure represents one WS2812B RGB LED
// A WS2812B reads values in the order G-R-B, so they are orginized in
// memory as such
__attribute__((packed)) struct rgb { 
	uint8_t g;
	uint8_t r;
	uint8_t b;
};

typedef struct led_strand led_strand_t;

led_strand_t * led_strand_init(uint8_t count, volatile uint8_t * port, uint8_t pin);
void led_strand_free(led_strand_t * leds);
int led_strand_set(led_strand_t * leds, uint8_t index, struct rgb * color);
void led_strand_clear(led_strand_t * leds);
int led_strand_draw(led_strand_t * leds);
uint8_t led_strand_get_count(led_strand_t * leds);

#endif
