#pragma once
#include <stdint.h>
#include "driver/ledc.h"

void led_white_init();

static constexpr auto LEDC_FRONT_CHANNEL =   ledc_channel_t::LEDC_CHANNEL_0;
static constexpr auto LEDC_REAR_CHANNEL =    ledc_channel_t::LEDC_CHANNEL_1;


void led_white_set(ledc_channel_t channel, float value);