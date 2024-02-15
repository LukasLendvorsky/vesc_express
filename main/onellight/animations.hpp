#pragma once
#include "led_strip.h"
#include "led_strip_spi.h"
#include <stdint.h>
#include <algorithm>
extern "C"{
	#include "freertos/task.h"
}

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
		for (int j = 0; j < num_leds; j += 1) {
			ESP_ERROR_CHECK(led_strip_set_pixel_hsv(handle, j, hue, 255, value));
		}
	}

  private:
    uint16_t hue;

    uint32_t on_ms;
    uint32_t off_ms;

    bool on = false;
    uint32_t last_change_time = 0;
};



class LarsonAnimation : public Animation {
public:
	LarsonAnimation(uint32_t num_leds, uint16_t hue) : Animation(num_leds), hue(hue){}


	virtual void update(led_strip_handle_t handle) override {
		// step (0-1)
		float step = (pdTICKS_TO_MS( xTaskGetTickCount() ) % period) / (float) period;

		// make step go from 0 to 1, then back to 0
		step *= 2;
		if (step > 1) step = 2-step;
		// center of larson
		float center = side_offset + (num_leds - 2*side_offset)*step;

		// just update leds
		for (int j = 0; j < num_leds; j += 1) {
			float dist = std::abs((float)j - center);
			uint8_t value = 0;
			if (dist <= wideness_full_brightness/2) {
				value = 255;
			} else if (dist <= wideness_total/2) {
				value = 255 * ((wideness_total/2) - dist) / (wideness_total/2);
			}
			ESP_ERROR_CHECK(led_strip_set_pixel_hsv(handle, j, hue, 255, value));
		}


	}

private:
    uint16_t hue;

	uint32_t period = 1000;
	float side_offset = 3;
	float wideness_full_brightness = 4;
	float wideness_total = 10;

};