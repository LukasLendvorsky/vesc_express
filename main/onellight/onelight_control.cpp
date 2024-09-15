
#include "onelight_control.hpp"
#include "animations.hpp"
#include "led_rgb.hpp"
#include "led_white.hpp"
#include "config.hpp"
#include "esp_log.h"
extern "C" {
    #include "comm_can.h"
    #include "main.h"
}
#include "config.hpp"

enum class Status{
  wait_for_device,  
  operational,
};

Status status = Status::wait_for_device;
int vesc_can_id = 0;


void handle_status_wait_for_device(){
    const auto local_id = backup.config.controller_id;
    can_status_msg * msg = comm_can_get_status_msg_index(0);

    // msg not correct, try another
    if (!msg || msg->id < 0 || msg->id == local_id)
        msg = comm_can_get_status_msg_index(1);

    if (msg && msg->id >= 0 && msg->id != local_id){
        vesc_can_id = msg->id;
        status = Status::operational;        
        ESP_LOGI("control", "Detected VESC at ID=%d", vesc_can_id);
    }
}


enum class Direction{Front, Back, Stoppped};


Direction detect_direction(can_status_msg * msg){
    static constexpr uint32_t DIRECTION_SMOOTHING_TIME = pdMS_TO_TICKS(250);

    Direction apparent_direction =  Direction::Stoppped;
    if (msg->duty == 0.0 || msg->current == 0.0) apparent_direction = Direction::Stoppped;
    else if (msg->rpm  > 1) apparent_direction = Direction::Front;
    else if (msg->rpm  < -1) apparent_direction = Direction::Back;
    
    static Direction direction = Direction::Stoppped;
    static Direction new_direction = Direction::Stoppped;
    static uint32_t new_direction_change_time = 0;


    // detected direction is different that last detected change
    if (apparent_direction != new_direction)
    {
        new_direction = apparent_direction;
        new_direction_change_time = xTaskGetTickCount();
    }

    if (new_direction != direction && (xTaskGetTickCount() - new_direction_change_time) >= DIRECTION_SMOOTHING_TIME){
        direction = new_direction;
        //new_direction_change_time = xTaskGetTickCount();
    }

    return direction;
}






void handle_status_operational(){
    can_status_msg * msg = comm_can_get_status_msg_id(vesc_can_id);

    // no message -> go back to wait for device state
    if (!msg){
        status = Status::wait_for_device;
        return;
    }

    auto direction = detect_direction(msg);
    

    float new_front_intensity = 0;
    float new_rear_intensity = 0;

    if (direction == Direction::Front) {
        ESP_LOGI("control", "front");
        #ifndef USING_FIXED_LED_COLORS
            new_front_intensity = WHITE_MAX_INTENSITY;
            new_rear_intensity = 0;
            
            led_rgb_set_animation(LedRgbType::front, &null_animation_front_rear);
            led_rgb_set_animation(LedRgbType::rear, &larson_animation);
        #else
            new_front_intensity = WHITE_MAX_INTENSITY;
            new_rear_intensity = WHITE_MAX_INTENSITY;
        #endif
    } else if (direction == Direction::Back) {
        ESP_LOGI("control", "rear");
        #ifndef USING_FIXED_LED_COLORS
            new_front_intensity = 0;
            new_rear_intensity = WHITE_MAX_INTENSITY;
            led_white_set(LEDC_FRONT_CHANNEL, 0);
            led_white_set(LEDC_REAR_CHANNEL, WHITE_MAX_INTENSITY);
            led_rgb_set_animation(LedRgbType::front, &larson_animation);
            led_rgb_set_animation(LedRgbType::rear, &null_animation_front_rear);
        #else
            new_front_intensity = WHITE_MAX_INTENSITY;
            new_rear_intensity = WHITE_MAX_INTENSITY;
        #endif


    } else if (direction == Direction::Stoppped) {
        ESP_LOGI("control", "stoped");
        new_front_intensity = 0;
        new_rear_intensity = 0;
        led_rgb_set_animation(LedRgbType::front, &null_animation_front_rear);
        led_rgb_set_animation(LedRgbType::rear, &null_animation_front_rear);
    }

    static float front_intensity_smoothed = 0;
    static float rear_intensity_smoothed = 0;

    const float smoothing_factor = 0.1;

    front_intensity_smoothed = new_front_intensity * smoothing_factor + front_intensity_smoothed * (1.0f-smoothing_factor); 
    rear_intensity_smoothed = new_rear_intensity * smoothing_factor + rear_intensity_smoothed * (1.0f-smoothing_factor); 

    led_white_set(LEDC_FRONT_CHANNEL, front_intensity_smoothed);
    led_white_set(LEDC_REAR_CHANNEL, rear_intensity_smoothed);

}


void onelight_control_update(){
    switch(status) {
        case Status::wait_for_device:
            handle_status_wait_for_device();
            break;
        case Status::operational:
            handle_status_operational();
            break;
    };
    
    
}
