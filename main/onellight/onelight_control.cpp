
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




void handle_status_operational(){
    can_status_msg * msg = comm_can_get_status_msg_id(vesc_can_id);

    // no message -> go back to wait for device state
    if (!msg){
        status = Status::wait_for_device;
        return;
    }

    if (msg->rpm > 1) {
        led_white_set(LEDC_FRONT_CHANNEL, WHITE_MAX_INTENSITY);
        led_white_set(LEDC_REAR_CHANNEL, 0);
        led_rgb_set_animation(LedRgbType::front, &null_animation_front_rear);
        led_rgb_set_animation(LedRgbType::rear, &blink_animation);
    } else if (msg->rpm < 1) {
        led_white_set(LEDC_FRONT_CHANNEL, 0);
        led_white_set(LEDC_REAR_CHANNEL, WHITE_MAX_INTENSITY);
        led_rgb_set_animation(LedRgbType::front, &blink_animation);
        led_rgb_set_animation(LedRgbType::rear, &null_animation_front_rear);
    }

    
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