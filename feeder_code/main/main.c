#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <esp_log.h>

#include "components/servo.h"
#include "components/buttons.h"
#include "components/mode.h"

const char *MAIN_TAG = "main loop";

void app_main(void)
{
    setup_btn_task();   // btn task
    setup_servo_task(); // servo task

    while (1)
    {
        /* --- change mode on mode btn press --- */
        if (mode_logic_state() == 0)
        {
            change_mode();
        }
        vTaskDelay(pdMS_TO_TICKS(250));
    }
}
