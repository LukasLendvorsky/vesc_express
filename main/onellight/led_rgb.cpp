extern "C"{
#include "led_rgb.h"
}
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "led_strip_spi.h"
#include "led_strip_types.h"
#include "animations.hpp"

led_strip_spi_config_t spi_config = {
    .clk_src = SPI_CLK_SRC_DEFAULT, // different clock source can lead to different power consumption
    .spi_bus = SPI2_HOST,   // SPI bus ID
    .flags = { .with_dma = true}, // Using DMA can improve performance and help drive more LEDs
};

led_strip_config_t front_strip_cfg = {
    .strip_gpio_num = 7, // The GPIO that connected to the LED strip's data line
    .max_leds = HEADLIGHT_NUM_LEDS, // The number of LEDs in the strip,
    .led_pixel_format = LED_PIXEL_FORMAT_GRB, // Pixel format of your LED strip
    .led_model = LED_MODEL_WS2812, // LED strip model
    .flags = {.invert_out = false}, // whether to invert the output signal (useful when your hardware has a level inverter)
};

Animation * front_animation;

led_strip_config_t rear_strip_cfg = {
    .strip_gpio_num = 8, // The GPIO that connected to the LED strip's data line
    .max_leds = HEADLIGHT_NUM_LEDS, // The number of LEDs in the strip,
    .led_pixel_format = LED_PIXEL_FORMAT_GRB, // Pixel format of your LED strip
    .led_model = LED_MODEL_WS2812, // LED strip model
    .flags = {.invert_out = false}, // whether to invert the output signal (useful when your hardware has a level inverter)
};
Animation * rear_animation;

led_strip_config_t hid_strip_cfg = {
    .strip_gpio_num = 6, // The GPIO that connected to the LED strip's data line
    .max_leds = HID_NUM_LEDS, // The number of LEDs in the strip,
    .led_pixel_format = LED_PIXEL_FORMAT_GRB, // Pixel format of your LED strip
    .led_model = LED_MODEL_WS2812, // LED strip model
    .flags = {.invert_out = false}, // whether to invert the output signal (useful when your hardware has a level inverter)
};
Animation * hid_animation;


NullAnimation null_animation_front_rear = NullAnimation(HEADLIGHT_NUM_LEDS);
NullAnimation null_animation_hid = NullAnimation(HID_NUM_LEDS);

void update_strip(led_strip_config_t & strip_cfg, Animation & animation){
    led_strip_handle_t handle;
    ESP_ERROR_CHECK(led_strip_new_spi_device(&strip_cfg, &spi_config, &handle));
    animation.update(handle);
    ESP_ERROR_CHECK(led_strip_del(handle));
}

void led_rgb_init(){
    front_animation = new RainbowAnimation(HEADLIGHT_NUM_LEDS);
    rear_animation = new BlinkAnimation(HEADLIGHT_NUM_LEDS, 0, 500, 2000);
}

void led_update_animations(){
    update_strip(front_strip_cfg,front_animation ? *front_animation : null_animation_front_rear);
    update_strip(rear_strip_cfg, rear_animation ? *rear_animation : null_animation_front_rear);
    update_strip(hid_strip_cfg, hid_animation ? *hid_animation : null_animation_hid);
}