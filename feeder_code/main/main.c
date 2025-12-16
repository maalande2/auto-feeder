#include <stdio.h>
#include <stdint.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_log.h"

// header files
#include "config.h"
#include "pins.h"
#include "servo.h"

// link gpio input to btn, link btn to isr

TaskHandle_t xFeedTaskHandle;

void IRAM_ATTR button_isr_handler(void *arg)
{
    BaseType_t xTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(xFeedTaskHandle, &xTaskWoken);
    portYIELD_FROM_ISR(xTaskWoken);
}

void app_main(void)
{
    servo_init();
    // close servos before running loop
    set_servo_angle(SERVO_CLOSE);

    // set next feedtime
    while (1)
    {
        // task notify for next-feed-time ticks
        open_close_servo(OPEN_TICKS);
        vTaskDelay(NEXT_FEED);
    }
}
