#pragma once
#include <stdint.h>
#include <stdlib.h>
#include "led_strip.h"
#include "animations.hpp"


#define HEADLIGHT_NUM_LEDS 10
#define HID_NUM_LEDS 10

enum class LedRgbType{
    front,
    rear,
    hid,
};

void led_rgb_init();
void led_rgb_update_animations();
void led_rgb_set_animation(LedRgbType, Animation *);



extern NullAnimation null_animation_front_rear;
extern NullAnimation null_animation_hid;
extern BlinkAnimation blink_animation;
extern LarsonAnimation larson_animation;