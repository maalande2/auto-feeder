#pragma once
#include "freertos/FreeRTOS.h"

#define SERVO_CLOSE 700                // ~0 degrees
#define SERVO_OPEN 1500                // ~90 degrees
#define NEXT_FEED pdMS_TO_TICKS(5000)  // delay between cycles = 24 hours / servings per day
#define OPEN_TICKS pdMS_TO_TICKS(3000) // how long to be open
