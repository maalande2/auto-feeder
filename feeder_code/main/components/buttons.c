#include "buttons.h"

// task setup related
static uint8_t task_params;
static TaskHandle_t btnHandle = NULL;

// hold mode btn logic
int curr_mode_logic = LOW;
int prev_mode_logic = LOW;
int mode_logic_flag = LOW;

// debug tag
const char *BTN_TAG = "Button Task";

// create gpio button
void mode_btn_config(void)
{
    gpio_config_t io = {
        .pin_bit_mask = 1ULL << MODE_BTN_PIN,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE};

    gpio_config(&io);
}

// set up mode btn task
void setup_btn_task(void)
{
    xTaskCreate(btn_event_loop, "btn task", STACK_SIZE, &task_params, TASK_PRIO, &btnHandle);
}

// main loop
void btn_event_loop(void *pvParameters)
{
    (void)pvParameters;
    mode_btn_config();
    while (1)
    {
        curr_mode_logic = gpio_get_level(MODE_BTN_PIN);
        if (prev_mode_logic == LOW && curr_mode_logic == HIGH)
        {
            vTaskDelay(pdMS_TO_TICKS(BTN_DEBOUNCE));
            curr_mode_logic = gpio_get_level(MODE_BTN_PIN);
            if (curr_mode_logic == HIGH)
            {
                ESP_LOGI(BTN_TAG, "btn pressed");
                mode_logic_flag = HIGH;
                while (gpio_get_level(MODE_BTN_PIN) == HIGH)
                {
                    vTaskDelay(pdMS_TO_TICKS(POLL_INTRV));
                }
            }
        }
        prev_mode_logic = curr_mode_logic;
        vTaskDelay(pdMS_TO_TICKS(POLL_INTRV));
    }
}

// return mode btn's logic state
int mode_logic_state(void)
{
    if (mode_logic_flag == HIGH)
    {
        mode_logic_flag = LOW;
        return HIGH;
    }
    return LOW;
}
