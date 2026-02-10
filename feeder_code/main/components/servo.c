#include "servo.h"
#include <stdio.h>
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_log.h"
#include "../config.h"

static const char *TAG = "SERVO";

void servo_init()
{
    // timer config
    ledc_timer_config_t timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_16_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 50,
        .clk_cfg = LEDC_AUTO_CLK};
    ledc_timer_config(&timer);

    // channel configs
    ledc_channel_config_t ch1 = {
        .gpio_num = SERVO1_PIN,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0};
    ledc_channel_config(&ch1);

    ledc_channel_config_t ch2 = ch1;
    ch2.gpio_num = SERVO2_PIN;
    ch2.channel = LEDC_CHANNEL_1;
    ledc_channel_config(&ch2);
}

// us to duty
static inline uint32_t us_to_duty(uint32_t us)
{
    return (us * ((1 << 16) - 1) / 20000); // 16 = res; 20k = period
}

// change servo angles
void set_servo_angle(uint32_t us)
{
    // speed mode, channel, duty
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, us_to_duty(us));
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, us_to_duty(us));
    ESP_LOGI(TAG, "OPEN");
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
    ESP_LOGI(TAG, "CLOSE");
}

// opening and closing
void open_close_servo(TickType_t open_ticks)
{
    set_servo_angle(SERVO_OPEN);
    vTaskDelay(open_ticks); // freertos
    set_servo_angle(SERVO_CLOSE);
}
