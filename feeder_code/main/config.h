#pragma once

#include "driver/gpio.h"

#define SERVO1_PIN GPIO_NUM_18
#define SERVO2_PIN GPIO_NUM_19
#define MODE_BTN_PIN GPIO_NUM_23
#define INC_BTN_PIN GPIO_NUM_25
#define DEC_BTN_PIN GPIO_NUM_26
#define CONFIRM_BTN_PIN GPIO_NUM_13
#define DISPLAY_SCL GPIO_NUM_22
#define DISPLAY_SDA GPIO_NUM_21

#define DISPLAY_I2C_SPEED_HZ 10000
#define LCD_ADDR_1 0x3E
#define LCD_ADDR_2 0x62

#define STACK_SIZE 2048

extern volatile int next_feed_time;
extern volatile int servings_day;