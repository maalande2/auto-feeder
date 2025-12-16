#ifndef MODE_H
#define MODE_H

typedef enum
{
    MODE_SET_TIME,           // next feed at HH:MM
    MODE_FEED_NOW,           // feeds then resets timer
    MODE_SET_PORTION,        // decide how long to keep servos open
    MODE_PORTIONS_REMAINING, // weigh food in hopper / portion size
    MODE_COUNT
} mode_t;

extern mode_t current_mode;

void next_mode(void);

#endif
