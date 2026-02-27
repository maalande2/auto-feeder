#pragma once

#include <stdio.h>
#include <stdint.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define BTN_TASK_PRIO 5
#define HIGH 0
#define LOW 1
#define BTN_DEBOUNCE 30
#define POLL_INTRV 10

void btn_init(void);
void setup_btn_task(void);
void mode_btn_press(void);
int mode_logic_state(void);
void cvbbbb(void);
void dec_btn_press(void);
void confirm_btn_press(void);
void btn_event_loop(void *pvParameters);