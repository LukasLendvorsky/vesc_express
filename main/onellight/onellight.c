
#include "led_white.h"
#include "led_rgb.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <math.h>
#include "esp_log.h"

#define EXAMPLE_LED_NUMBERS 12
#define EXAMPLE_CHASE_SPEED_MS 10

static uint8_t led_strip_pixels[EXAMPLE_LED_NUMBERS * 3];

void onellight_task(void *){
    uint32_t red = 0;
    uint32_t green = 0;
    uint32_t blue = 0;
    uint16_t hue = 0;
    uint16_t start_rgb = 0;


   while (1) {
        for (int i = 0; i < 3; i++) {
            for (int j = i; j < EXAMPLE_LED_NUMBERS; j += 3) {
                // Build RGB pixels
                hue = j * 360 / EXAMPLE_LED_NUMBERS + start_rgb;
                led_strip_hsv2rgb(hue, 100, 100, &red, &green, &blue);
                led_strip_pixels[j * 3 + 0] = green;
                led_strip_pixels[j * 3 + 1] = blue;
                led_strip_pixels[j * 3 + 2] = red;
            }

            //led_strip_transmit(&led_strip_pixels[0], EXAMPLE_LED_NUMBERS * 3);
            vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
            
        }
        start_rgb += 60;
        static float val = 0.0f;
        val += .01;
        val = fmod(val, 1.f);
        led_white_set(0, val);
        led_white_set(1, 1.0f-val);
   }

}

void onellight_init() {
	led_white_init();
	//led_rgb_init();


    xTaskCreatePinnedToCore(onellight_task, "usb_rx", 3072, NULL, 8, NULL, tskNO_AFFINITY);
}