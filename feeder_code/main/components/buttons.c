#include "buttons.h"
#include "mode.h"
#include "servo.h"
#include "../config.h"

// task setup related
static uint8_t task_params;
static TaskHandle_t btnHandle = NULL;

// hold mode btn logic
int curr_mode_logic = LOW;
int prev_mode_logic = LOW;
int mode_logic_flag = LOW;

// inc btn logic
int curr_inc_logic = LOW;
int prev_inc_logic = LOW;

// dec btn logic
int curr_dec_logic = LOW;
int prev_dec_logic = LOW;

// confirm btn logic
int curr_confirm_logic = LOW;
int prev_confirm_logic = LOW;

// edit feed timing
int temp_counter = 0;

// debug tag
static const char *BTN_TAG = "Button Task";

// create gpio mode button
void btn_init(void)
{
    gpio_config_t mode = {
        .pin_bit_mask = 1ULL << MODE_BTN_PIN,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE};

    gpio_config(&mode);

    gpio_config_t inc = mode;
    inc.pin_bit_mask = 1ULL << INC_BTN_PIN;
    gpio_config(&inc);

    gpio_config_t dec = mode;
    dec.pin_bit_mask = 1ULL << DEC_BTN_PIN;
    gpio_config(&dec);

    gpio_config_t confirm = mode;
    confirm.pin_bit_mask = 1ULL << CONFIRM_BTN_PIN;
    gpio_config(&confirm);
}

// set up mode btn task
void setup_btn_task(void)
{
    xTaskCreate(btn_event_loop, "btn task", STACK_SIZE, &task_params, BTN_TASK_PRIO, &btnHandle);
}

// check mode press
void mode_btn_press(void)
{
    // TODO: add a tmp value that will be used by other btns. reset on mode change. will save on confirm
    curr_mode_logic = gpio_get_level(MODE_BTN_PIN);
    if (prev_mode_logic == LOW && curr_mode_logic == HIGH)
    {
        vTaskDelay(pdMS_TO_TICKS(BTN_DEBOUNCE));
        curr_mode_logic = gpio_get_level(MODE_BTN_PIN);
        if (curr_mode_logic == HIGH)
        {
            ESP_LOGI(BTN_TAG, "MODE btn pressed");
            mode_logic_flag = HIGH;
            temp_counter = 0;
            while (gpio_get_level(MODE_BTN_PIN) == HIGH)
            {
                vTaskDelay(pdMS_TO_TICKS(POLL_INTRV)); // prevent duplicate reads
            }
        }
    }
    prev_mode_logic = curr_mode_logic;
}

// inc button pressed
void inc_btn_press(void)
{
    /*
    TODO:
    - check if btn pressed w/ debounce logic to prevent dup reads
    - check if MODE_TIMING (we will implement portion later)
    - inc tmp var value on each press
    */
    curr_inc_logic = gpio_get_level(INC_BTN_PIN);
    if (prev_inc_logic == LOW && curr_inc_logic == HIGH)
    {
        vTaskDelay(pdMS_TO_TICKS(BTN_DEBOUNCE));
        curr_inc_logic = gpio_get_level(INC_BTN_PIN);
        if (curr_inc_logic == HIGH && (get_mode() == MODE_TIMING || get_mode() == MODE_SERVINGS_DAY))
        {
            temp_counter += 1;
            ESP_LOGI(BTN_TAG, "INC pressed: %d", temp_counter);
            while (gpio_get_level(INC_BTN_PIN) == HIGH)
            {
                vTaskDelay(pdMS_TO_TICKS(POLL_INTRV)); // prevent duplicate reads
            }
        }
    }
    prev_inc_logic = curr_inc_logic;
}

// dec button pressed
void dec_btn_press(void)
{
    /*
    TODO:
    - check if btn pressed w/ debounce logic to prevent dup reads
    - check if MODE_TIMING (we will implement portion later)
    - dec tmp var value on each press
    */
    curr_dec_logic = gpio_get_level(DEC_BTN_PIN);
    if (prev_dec_logic == LOW && curr_dec_logic == HIGH)
    {
        vTaskDelay(pdMS_TO_TICKS(BTN_DEBOUNCE));
        curr_dec_logic = gpio_get_level(DEC_BTN_PIN);
        if (curr_dec_logic == HIGH && (get_mode() == MODE_TIMING || get_mode() == MODE_SERVINGS_DAY))
        {
            temp_counter -= 1;
            ESP_LOGI(BTN_TAG, "DEC pressed: %d", temp_counter);
            while (gpio_get_level(DEC_BTN_PIN) == HIGH)
            {
                vTaskDelay(pdMS_TO_TICKS(POLL_INTRV)); // prevent duplicate reads
            }
        }
    }
    prev_dec_logic = curr_dec_logic;
}

// confirm button pressed
void confirm_btn_press(void)
{
    curr_confirm_logic = gpio_get_level(CONFIRM_BTN_PIN);
    if (prev_confirm_logic == LOW && curr_confirm_logic == HIGH)
    {
        vTaskDelay(pdMS_TO_TICKS(BTN_DEBOUNCE));
        curr_confirm_logic = gpio_get_level(CONFIRM_BTN_PIN);
        if (curr_confirm_logic == HIGH)
        {
            ESP_LOGI(BTN_TAG, "CONFIRM btn pressed");
            /*
            TODO:
            - check if MODE_TIMING: set NEXT_FEED to tmp var value
            - else check if MODE_FEED: set NEXT_FEED to 0
            */
            if (get_mode() == MODE_TIMING)
            {
                if (temp_counter > 0)
                {
                    next_feed_time = temp_counter;
                }
                else
                {
                    next_feed_time = 0;
                }
                temp_counter = 0;
            }
            else if (get_mode() == MODE_SERVINGS_DAY)
            {
                if (temp_counter > 0)
                {
                    servings_day = temp_counter;
                }
                else
                {
                    servings_day = 1;
                }
                temp_counter = 0;
            }
            else if (get_mode() == MODE_FEED)
            {
                next_feed_time = 0;
            }
            while (gpio_get_level(CONFIRM_BTN_PIN) == HIGH)
            {
                vTaskDelay(pdMS_TO_TICKS(POLL_INTRV)); // prevent duplicate reads
            }
        }
    }
    prev_confirm_logic = curr_confirm_logic;
}

// main loop
void btn_event_loop(void *pvParameters)
{
    (void)pvParameters;
    btn_init();
    while (1)
    {
        mode_btn_press();
        inc_btn_press();
        confirm_btn_press();
        dec_btn_press();
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
