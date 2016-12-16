#ifndef __LED_H
#define __LED_H

#include <stdint.h>

typedef struct led_strand led_strand_t;

led_strand_t * led_strand_init(uint8_t count);
void led_strand_free(led_strand_t * leds);
int led_strand_set(led_strand_t * leds, uint8_t index, uint8_t r, uint8_t g, uint8_t b);
void led_strand_clear(led_strand_t * leds);
int led_strand_draw(led_strand_t * leds);

#endif
