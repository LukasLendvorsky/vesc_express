#pragma once
#include <stdint.h>
#include <stdlib.h>
#include "led_strip.h"

extern led_strip_handle_t my_strip;

void led_rgb_init();


void led_strip_transmit(uint8_t *led_strip_pixels, size_t bytes);

/**
 * @brief Simple helper function, converting HSV color space to RGB color space
 *
 * Wiki: https://en.wikipedia.org/wiki/HSL_and_HSV
 *
 */
void led_strip_hsv2rgb(uint32_t h, uint32_t s, uint32_t v, uint32_t *r, uint32_t *g, uint32_t *b);