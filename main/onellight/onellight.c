
#include "led_strip.h"
#include "led_white.h"
#include "led_rgb.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <math.h>
#include "esp_log.h"


#define EXAMPLE_CHASE_SPEED_MS 10

void onellight_task(void *){
   uint16_t start_rgb = 0;

   while (1) {
       led_update_animations();


        vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
        start_rgb += 1;
   }

}

void onellight_init() {
	//led_white_init();
	led_rgb_init();

    xTaskCreatePinnedToCore(onellight_task, "onellight_task", 3072, NULL, 8, NULL, tskNO_AFFINITY);
}