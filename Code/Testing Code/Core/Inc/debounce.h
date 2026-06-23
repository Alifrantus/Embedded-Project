#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include <stdint.h>
#include "config.h"

/* Ma trận trạng thái sau debounce — 1 = nhấn ổn định, 0 = thả */
extern uint8_t debounced[ROWS][COLS];

void debounce_init(void);
void debounce_process(void);

#endif /* DEBOUNCE_H */
