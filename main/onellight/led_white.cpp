
#include "led_white.hpp"
#include <stdio.h>
#include <math.h>
#include "esp_log.h"
#include "esp_err.h"


#define LEDC_TIMER     LEDC_TIMER_0
#define LEDC_MODE      LEDC_LOW_SPEED_MODE
#define LEDC_DUTY_RES  LEDC_TIMER_10_BIT // Set duty resolution to 10 bits
#define LEDC_FREQUENCY (500) // Frequency in Hertz. Set frequency at 500Hz


#define LEDC_FRONT_OUTPUT_IO (4)

#define LEDC_REAR_OUTPUT_IO  (5)

void led_white_init() {
	// Prepare and then apply the LEDC PWM timer configuration
	ledc_timer_config_t ledc_timer = {
		.speed_mode      = LEDC_MODE,
		.duty_resolution = LEDC_DUTY_RES,
		.timer_num       = LEDC_TIMER,
		.freq_hz         = LEDC_FREQUENCY, // Set output frequency at 4 kHz
		.clk_cfg         = LEDC_AUTO_CLK
	};
	ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

	// Configure front led channel
	ledc_channel_config_t front_config = {
		.gpio_num   = LEDC_FRONT_OUTPUT_IO,
		.speed_mode = LEDC_MODE,
		.channel    = LEDC_FRONT_CHANNEL,
		.intr_type  = LEDC_INTR_DISABLE,
		.timer_sel  = LEDC_TIMER,
		.duty       = 0, // Set duty to 0%
		.hpoint     = 0,
		.flags = {0}
	};
	ESP_ERROR_CHECK(ledc_channel_config(&front_config));
	
 	ESP_LOGI("white", "ledc_channel_config 1");

	// Configure front led channel
	ledc_channel_config_t rear_config = {
		.gpio_num   = LEDC_REAR_OUTPUT_IO,
		.speed_mode = LEDC_MODE,
		.channel    = LEDC_REAR_CHANNEL,
		.intr_type  = LEDC_INTR_DISABLE,
		.timer_sel  = LEDC_TIMER,
		.duty       = 0, // Set duty to 0%
		.hpoint     = 0,
		.flags = {0}
	};
	ESP_ERROR_CHECK(ledc_channel_config(&rear_config));

 	ESP_LOGI("white", "ledc_channel_config 2");
}

void led_white_set(ledc_channel_t channel, float value) {
	uint32_t value_int = roundf(value * 1024);
	ESP_ERROR_CHECK(
		ledc_set_duty(LEDC_MODE, channel, value_int)
	);
	ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, channel));

       
 	ESP_LOGI("white", "set channel %u val:%lu",channel,  value_int);
}