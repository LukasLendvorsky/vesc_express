#include "led_rgb.h"
#include <string.h>
#include "esp_log.h"
#include "driver/rmt_tx.h"
#include "led_strip_encoder.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define RMT_LED_STRIP_RESOLUTION_HZ  10000000 // 10MHz resolution, 1 tick = 0.1us (led strip needs a high
			 // resolution)

static const char *TAG = "led_rgb";

rmt_encoder_handle_t led_encoder = NULL;
rmt_channel_handle_t channel1;


rmt_channel_handle_t init_strip(const int gpio_num) {
    rmt_channel_handle_t led_chan;
	ESP_LOGI(TAG, "Create RMT TX channel");
	rmt_tx_channel_config_t tx_chan_config = {
		.clk_src  = RMT_CLK_SRC_DEFAULT, // select source clock
		.gpio_num = gpio_num,
		.mem_block_symbols =
			64, // increase the block size can make the LED less flickering
		.resolution_hz     = RMT_LED_STRIP_RESOLUTION_HZ,
		.trans_queue_depth = 4, // set the number of transactions that can be
								// pending in the background
	};
	ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &led_chan));

	ESP_LOGI(TAG, "Install led strip encoder");
	
	led_strip_encoder_config_t encoder_config = {
		.resolution = RMT_LED_STRIP_RESOLUTION_HZ,
	};
	ESP_ERROR_CHECK(rmt_new_led_strip_encoder(&encoder_config, &led_encoder));

	ESP_LOGI(TAG, "Enable RMT TX channel");
	ESP_ERROR_CHECK(rmt_enable(led_chan));

    return led_chan;
}


void led_rgb_init(){
    channel1 = init_strip(7);
}


void led_strip_transmit(uint8_t *led_strip_pixels, size_t bytes){
    //todo
    rmt_channel_handle_t led_chan = channel1;
    rmt_transmit_config_t tx_config = {
        .loop_count = 0, // no transfer loop
    };
    // Flush RGB values to LEDs
    ESP_ERROR_CHECK(rmt_transmit(led_chan, led_encoder, led_strip_pixels, bytes, &tx_config));
    ESP_ERROR_CHECK(rmt_tx_wait_all_done(led_chan, portMAX_DELAY));

}
/*
{
    vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
    memset(led_strip_pixels, 0, bytes);
    ESP_ERROR_CHECK(rmt_transmit(led_chan, led_encoder, led_strip_pixels, bytes, &tx_config));
    ESP_ERROR_CHECK(rmt_tx_wait_all_done(led_chan, portMAX_DELAY));
    vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
}
*/

/**
 * @brief Simple helper function, converting HSV color space to RGB color space
 *
 * Wiki: https://en.wikipedia.org/wiki/HSL_and_HSV
 *
 */
void led_strip_hsv2rgb(uint32_t h, uint32_t s, uint32_t v, uint32_t *r, uint32_t *g, uint32_t *b)
{
    h %= 360; // h -> [0,360]
    uint32_t rgb_max = v * 2.55f;
    uint32_t rgb_min = rgb_max * (100 - s) / 100.0f;

    uint32_t i = h / 60;
    uint32_t diff = h % 60;

    // RGB adjustment amount by hue
    uint32_t rgb_adj = (rgb_max - rgb_min) * diff / 60;

    switch (i) {
    case 0:
        *r = rgb_max;
        *g = rgb_min + rgb_adj;
        *b = rgb_min;
        break;
    case 1:
        *r = rgb_max - rgb_adj;
        *g = rgb_max;
        *b = rgb_min;
        break;
    case 2:
        *r = rgb_min;
        *g = rgb_max;
        *b = rgb_min + rgb_adj;
        break;
    case 3:
        *r = rgb_min;
        *g = rgb_max - rgb_adj;
        *b = rgb_max;
        break;
    case 4:
        *r = rgb_min + rgb_adj;
        *g = rgb_min;
        *b = rgb_max;
        break;
    default:
        *r = rgb_max;
        *g = rgb_min;
        *b = rgb_max - rgb_adj;
        break;
    }
}