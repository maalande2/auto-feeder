#include "mode.h"

Mode curr_mode = MODE_TIMING;
static const char *TAG = "CURRENT_MODE";

void change_mode(void)
{
    curr_mode = (Mode)((curr_mode + 1) % COUNT);
    ESP_LOGI(TAG, "%d", curr_mode);
}

Mode get_mode(void)
{
    /*
    0 - timing
    1 - servings a day
    2 - serving size (open ticks)
    3 - feed now
    */
    return curr_mode;
}
