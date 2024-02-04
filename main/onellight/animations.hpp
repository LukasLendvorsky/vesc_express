#pragma once
#include "led_strip.h"
#include "led_strip_spi.h"
#include <stdint.h>
#include "freertos/FreeRTOS.h"

class Animation {
  public:
	Animation(uint32_t num_leds) : num_leds(num_leds) {
	}
	virtual void update(led_strip_handle_t);

  protected:
	uint32_t num_leds;
};

class NullAnimation : public Animation {
  public:
	NullAnimation(uint32_t num_leds) : Animation(num_leds) {
	}
	virtual void update(led_strip_handle_t handle) override {
		led_strip_clear(handle);
	}
};

class RainbowAnimation : public Animation {
  public:
	RainbowAnimation(uint32_t num_leds) : Animation(num_leds) {
	}

	virtual void update(led_strip_handle_t handle) override {
		for (int j = 0; j < num_leds; j += 1) {
			// Build RGB pixels
			uint16_t hue =
				(start_rgb + (int)(360.0f * ((float)j / num_leds))) % 360;
			ESP_ERROR_CHECK(led_strip_set_pixel_hsv(handle, j, hue, 255, 255));
		}
		ESP_ERROR_CHECK(led_strip_refresh(handle));
		start_rgb += 10;
	}

  private:
	uint16_t start_rgb;
};

class BlinkAnimation : public Animation {
  public:
	BlinkAnimation(uint32_t num_leds, uint16_t hue, uint32_t on_ms, uint32_t off_ms) : Animation(num_leds), hue(hue), on_ms(on_ms), off_ms(off_ms) { }

	virtual void update(led_strip_handle_t handle) override {
        uint32_t currentTime = pdTICKS_TO_MS( xTaskGetTickCount() );
        if (on && currentTime - last_change_time > on_ms){
            last_change_time = currentTime;
            on = false;
        } else if (!on && currentTime - last_change_time > off_ms) {
            last_change_time = currentTime;
            on = true;
        }

        uint32_t value = on ? 255 : 0;

        ESP_LOGI("blink", "current: %lu, last_change_time=%lu, value=%lu",currentTime, last_change_time, value);

		for (int j = 0; j < num_leds; j += 1) {
			ESP_ERROR_CHECK(led_strip_set_pixel_hsv(handle, j, hue, 255, value));
		}
		ESP_ERROR_CHECK(led_strip_refresh(handle));
	}

  private:

    uint16_t start_rgb;
    uint16_t hue;

    uint32_t on_ms;
    uint32_t off_ms;

    bool on = false;
    uint32_t last_change_time = 0;
};