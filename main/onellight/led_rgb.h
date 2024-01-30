#pragma once
#include <stdint.h>
#include <stdlib.h>
#include "led_strip.h"

#define HEADLIGHT_NUM_LEDS 10
#define HID_NUM_LEDS 10

extern led_strip_handle_t front_strip;

void led_rgb_init();
void led_update_animations();