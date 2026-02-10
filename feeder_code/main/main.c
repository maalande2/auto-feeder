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
    // init & close servo
    servo_init();
    set_servo_angle(SERVO_CLOSE);
    // btn task set up
    setup_btn_task();

    while (1)
    {
        /* --- servos open-close each feed time --- */
        open_close_servo(OPEN_TICKS); // can be changed by PORTION SIZE later
        vTaskDelay(NEXT_FEED);        // can be changed by SET TIME later

        /* --- change mode on mode btn press --- */
        if (mode_logic_state() == 0)
        {
            change_mode();
        }
        vTaskDelay(pdMS_TO_TICKS(250));
    }
}
