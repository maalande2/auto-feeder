#include "mode.h"

mode_t current_mode = MODE_SET_TIME;

void next_mode(void)
{
    current_mode = (mode_t)((current_mode + 1) % MODE_COUNT);
}
