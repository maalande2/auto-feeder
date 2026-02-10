#pragma once

#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <esp_log.h>

typedef enum
{
    MODE_TIMING,
    MODE_PORTION,
    MODE_FEED,
    COUNT
} Mode;

// get current mode for settings
Mode get_mode(void);

// change mode on btn press
void change_mode(void);
