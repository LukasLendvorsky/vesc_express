
#include "led_white.h"
#include "led_rgb.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <math.h>
#include "esp_log.h"

#define EXAMPLE_LED_NUMBERS 2
#define EXAMPLE_CHASE_SPEED_MS 10

void onellight_task(void *){
    uint16_t hue = 0;
    uint16_t start_rgb = 0;


   while (1) {
        for (int j = 0; j < EXAMPLE_LED_NUMBERS; j += 1) {
            // Build RGB pixels
            hue = j * 360 / EXAMPLE_LED_NUMBERS + start_rgb;

            led_strip_set_pixel_hsv (my_strip, j, hue, 100, 100);
        }
        led_strip_refresh(my_strip);

        vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
        start_rgb += 60;
        static float val = 0.0f;
        val += .01;
        val = fmod(val, 1.f);
        //led_white_set(0, val);
        //led_white_set(1, 1.0f-val);
   }

}

void onellight_init() {
	led_white_init();
	led_rgb_init();


    xTaskCreatePinnedToCore(onellight_task, "usb_rx", 3072, NULL, 8, NULL, tskNO_AFFINITY);
}