#pragma once
#include <stdint.h>
#include <stdlib.h>
#include "led_strip.h"

#define HEADLIGHT_NUM_LEDS 10
#define HID_NUM_LEDS 10

void led_rgb_init();
void led_update_animations();
