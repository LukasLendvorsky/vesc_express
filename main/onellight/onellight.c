
#include "led_white.h"
#include "led_rgb.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <math.h>
#include "esp_log.h"

#define UPDATE_PERIOD 30

void onellight_task(void *) {
	while (1) {
		led_update_animations();
		vTaskDelay(pdMS_TO_TICKS(UPDATE_PERIOD));
	}
}

void onellight_init() {
	//led_white_init();
	led_rgb_init();

	xTaskCreatePinnedToCore(
		onellight_task, "onellight_task", 3072, NULL, 8, NULL, tskNO_AFFINITY
	);
}