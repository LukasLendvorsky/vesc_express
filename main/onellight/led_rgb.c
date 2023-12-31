#include "led_rgb.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "led_strip_rmt.h"

static const char *TAG = "led_rgb";
led_strip_handle_t my_strip;


void led_rgb_init(){
    /* LED strip initialization with the GPIO and pixels number*/
    led_strip_config_t strip_config = {
        .strip_gpio_num = 7, // The GPIO that connected to the LED strip's data line
        .max_leds = 2, // The number of LEDs in the strip,
        .led_pixel_format = LED_PIXEL_FORMAT_GRB, // Pixel format of your LED strip
        .led_model = LED_MODEL_WS2812, // LED strip model
        .flags.invert_out = true, // whether to invert the output signal (useful when your hardware has a level inverter)
    };

    led_strip_rmt_config_t rmt_config = {
    #if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
        .rmt_channel = 0,
    #else
        .clk_src = RMT_CLK_SRC_DEFAULT, // different clock source can lead to different power consumption
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
        .flags.with_dma = false, // whether to enable the DMA feature
    #endif
    };
     	
    //ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &my_strip));
    //ESP_LOGI("rgb", "led_strip_new_rmt_device");

    led_strip_spi_config_t spi_config = {
        .clk_src = SPI_CLK_SRC_DEFAULT, // different clock source can lead to different power consumption
        .flags.with_dma = true, // Using DMA can improve performance and help drive more LEDs
        .spi_bus = SPI2_HOST,   // SPI bus ID
    };
    ESP_ERROR_CHECK(led_strip_new_spi_device(&strip_config, &spi_config, &my_strip));

    
    ESP_ERROR_CHECK(led_strip_clear(my_strip));
    ESP_LOGI("rgb", "led_strip_clear");
    ESP_ERROR_CHECK(led_strip_set_pixel(my_strip, 0, 255, 0, 0));
    ESP_LOGI("rgb", "led_strip_set_pixel");
    ESP_ERROR_CHECK(led_strip_refresh(my_strip));
    ESP_LOGI("rgb", "led_strip_set_pixel");
}
