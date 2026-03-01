#include "servo.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_log.h"
#include "../config.h"

static const char *TAG = "SERVO";

static uint8_t servo_params;
static TaskHandle_t servoHandle = NULL;

int next_feed_time_default = 10;
volatile int next_feed_time = 10;
volatile int servings_day = 2;

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

    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
}

// opening and closing
void open_close_servo(TickType_t open_ticks)
{
    set_servo_angle(SERVO_OPEN);
    ESP_LOGI(TAG, "OPEN");
    vTaskDelay(open_ticks); // freertos
    set_servo_angle(SERVO_CLOSE);
    ESP_LOGI(TAG, "CLOSE");
}

// set up mode btn task
void setup_servo_task(void)
{
    xTaskCreate(servo_event_loop, "servo task", STACK_SIZE, &servo_params, SERVO_TASK_PRIO, &servoHandle);
}

// main servo event loop
void servo_event_loop(void *pvParameters)
{
    (void)pvParameters;
    servo_init();
    set_servo_angle(SERVO_CLOSE);
    TickType_t start_tick = xTaskGetTickCount();
    while (1)
    {
        // ESP_LOGI(TAG, "Delay for next open-close: %d seconds", next_feed_time);
        TickType_t curr_tick = xTaskGetTickCount();
        TickType_t elapsed_time = curr_tick - start_tick;
        if (elapsed_time >= pdMS_TO_TICKS(next_feed_time * 1000))
        {
            open_close_servo(OPEN_TICKS);
            next_feed_time = 60 / servings_day; // servings a minute for now
            start_tick = xTaskGetTickCount();
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
