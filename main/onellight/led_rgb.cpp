
#include "led_rgb.hpp"
#include "animations.hpp"
#include "driver/gpio.h"
#include "hal/gpio_types.h"
extern "C"{
    #include "esp_log.h"
    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"
    #include "led_strip.h"
    #include "led_strip_spi.h"
    #include "led_strip_types.h"
    #include "soc/spi_periph.h"
    #include "esp_rom_gpio.h"
    #include "soc/io_mux_reg.h"
}


led_strip_spi_config_t spi_config = {
    .clk_src = SPI_CLK_SRC_DEFAULT, // different clock source can lead to different power consumption
    .spi_bus = SPI2_HOST,   // SPI bus ID
    .flags = { .with_dma = true}, // Using DMA can improve performance and help drive more LEDs
};

led_strip_config_t strip_cfg = {
    .strip_gpio_num = 7, // The GPIO that connected to the LED strip's data line
    .max_leds = HEADLIGHT_NUM_LEDS > HID_NUM_LEDS ? HEADLIGHT_NUM_LEDS : HID_NUM_LEDS, // The number of LEDs in the strip,
    .led_pixel_format = LED_PIXEL_FORMAT_GRB, // Pixel format of your LED strip
    .led_model = LED_MODEL_WS2812, // LED strip model
    .flags = {.invert_out = false}, // whether to invert the output signal (useful when your hardware has a level inverter)
};

static constexpr auto front_led_strip_gpio = 7;
static constexpr auto rear_led_strip_gpio = 8;
static constexpr auto hid_led_strip_gpio = 6;


Animation * front_animation;
Animation * rear_animation;
Animation * hid_animation;

NullAnimation null_animation_front_rear = NullAnimation(HEADLIGHT_NUM_LEDS);
NullAnimation null_animation_hid = NullAnimation(HID_NUM_LEDS);
BlinkAnimation blink_animation = BlinkAnimation(10, 0, 100, 100);

static led_strip_handle_t handle;

void update_strip(uint8_t strip_gpio, Animation & animation){
    // connect GPIO of current led strip to 
    gpio_iomux_out(strip_gpio, SPI2_FUNC_NUM, false);
    animation.update(handle);
    gpio_iomux_out(strip_gpio, PIN_FUNC_GPIO, false);
}

void led_rgb_init(){
    front_animation = &null_animation_front_rear;
    rear_animation = &null_animation_front_rear;
    hid_animation = &null_animation_hid;


    ESP_ERROR_CHECK(led_strip_new_spi_device(&strip_cfg, &spi_config, &handle));

    gpio_set_direction((gpio_num_t)front_led_strip_gpio, gpio_mode_t::GPIO_MODE_OUTPUT);
    gpio_set_level((gpio_num_t)front_led_strip_gpio, 0);
    gpio_set_direction((gpio_num_t)rear_led_strip_gpio, gpio_mode_t::GPIO_MODE_OUTPUT);
    gpio_set_level((gpio_num_t)rear_led_strip_gpio, 0);
    gpio_set_direction((gpio_num_t)hid_led_strip_gpio, gpio_mode_t::GPIO_MODE_OUTPUT);
    gpio_set_level((gpio_num_t)hid_led_strip_gpio, 0);
    
}


void led_rgb_update_animations(){
    update_strip(front_led_strip_gpio,front_animation ? *front_animation : null_animation_front_rear);
    update_strip(rear_led_strip_gpio, rear_animation ? *rear_animation : null_animation_front_rear);
    update_strip(hid_led_strip_gpio, hid_animation ? *hid_animation : null_animation_hid);
}

void led_rgb_set_animation(LedRgbType type, Animation *anim)
{
    switch(type){
        case LedRgbType::front:
            front_animation = anim;
            break;
        case LedRgbType::rear:
            rear_animation = anim;
            break;
        case LedRgbType::hid:
            hid_animation = anim;
            break;
    }
}