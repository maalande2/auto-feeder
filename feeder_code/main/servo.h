#pragma once
#include <stdint.h>
#include "freertos/FreeRTOS.h"

void servo_init(void);
void set_servo_angle(uint32_t us);
void open_close_servo(TickType_t open_ticks);
