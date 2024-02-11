
extern "C"{
	#include "onellight.h"
	#include "freertos/FreeRTOS.h"
	#include "freertos/task.h"
}
#include "led_white.hpp"
#include "led_rgb.hpp"
#include <math.h>
#include "esp_log.h"
#include "onelight_control.hpp"

#define UPDATE_PERIOD 30

void onellight_task(void *) {
	while (1) {
		onelight_control_update();
		led_rgb_update_animations();
		vTaskDelay(pdMS_TO_TICKS(UPDATE_PERIOD));
	}
}

void onellight_init() {
	led_white_init();
	led_rgb_init();

	xTaskCreatePinnedToCore(
		onellight_task, "onellight_task", 3072, NULL, 8, NULL, tskNO_AFFINITY
	);
}