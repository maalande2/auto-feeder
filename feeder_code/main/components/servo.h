#pragma once
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "../config.h"

#define SERVO_CLOSE 700                // ~0 degrees
#define SERVO_OPEN 1500                // ~90 degrees
#define OPEN_TICKS pdMS_TO_TICKS(3000) // how long to be open
#define SERVO_TASK_PRIO 3

void servo_init(void);
void set_servo_angle(uint32_t us);
void open_close_servo(TickType_t open_ticks);
void setup_servo_task(void);
void servo_event_loop(void *pvParamaters);