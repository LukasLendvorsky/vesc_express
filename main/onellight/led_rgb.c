#include "led_rgb.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "led_strip_spi.h"
#include "led_strip_types.h"

led_strip_handle_t front_strip;
led_strip_handle_t rear_strip;
led_strip_handle_t hid_strip;


void led_init_strip(led_strip_handle_t *strip, uint32_t num_leds, uint32_t gpio_num) {
    led_strip_config_t strip_config = {
        .strip_gpio_num = gpio_num, // The GPIO that connected to the LED strip's data line
        .max_leds = num_leds, // The number of LEDs in the strip,
        .led_pixel_format = LED_PIXEL_FORMAT_GRB, // Pixel format of your LED strip
        .led_model = LED_MODEL_WS2812, // LED strip model
        .flags.invert_out = false, // whether to invert the output signal (useful when your hardware has a level inverter)
    };

    led_strip_spi_config_t spi_config = {
        .clk_src = SPI_CLK_SRC_DEFAULT, // different clock source can lead to different power consumption
        .flags.with_dma = true, // Using DMA can improve performance and help drive more LEDs
        .spi_bus = SPI2_HOST,   // SPI bus ID
    };
    ESP_ERROR_CHECK(led_strip_new_spi_device(&strip_config, &spi_config, strip));
    ESP_ERROR_CHECK(led_strip_clear(*strip));
    ESP_LOGI("rgb", "Led strip init");
}

void led_rgb_init(){
    led_init_strip(&rear_strip, HEADLIGHT_NUM_LEDS, 7);
}


void led_update_animations(){
 
    static uint16_t start_rgb = 0;
    for (int j = 0; j < HEADLIGHT_NUM_LEDS; j += 1) {
        // Build RGB pixels
        uint16_t hue = (360 + start_rgb + (int)(60.0f*((float)j/HEADLIGHT_NUM_LEDS))) % 360;
        ESP_ERROR_CHECK(led_strip_set_pixel_hsv (rear_strip, j, hue, 255, 255));
    }
    ESP_ERROR_CHECK(led_strip_refresh(rear_strip));
    start_rgb+=1;
}