#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <esp_log.h>
#include "driver/i2c_master.h"
#include "esp_err.h"

#include "components/servo.h"
#include "components/buttons.h"
#include "components/mode.h"
#include "components/i2c_bus.h"
#include "components/display.h"

const char *MAIN_TAG = "MAIN";

// config i2c device

void app_main(void)
{
    i2c_master_bus_handle_t bus_handle = NULL;
    i2c_init(&bus_handle);
    display_dev_init(&bus_handle);
    /*
        runs a task checking if any btn (inc/dec/mode/confirm)
        has been pressed. poll from here to see if we need to
        change modes from a mode press. other button presses
        can be handled in buttons.c
    */
    setup_btn_task();

    /*
        servo task uses values from buttons.c to determine
        when to next open
    */
    setup_servo_task();

    while (1)
    {
        // change mode on mode btn press
        if (mode_logic_state() == 0)
        {
            change_mode();
        }
        vTaskDelay(pdMS_TO_TICKS(250));
    }
}
