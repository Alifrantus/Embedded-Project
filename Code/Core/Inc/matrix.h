#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>
#include "config.h"

/* Ma trận trạng thái thô (raw) — 1 = nhấn, 0 = thả */
extern uint8_t matrix_raw[ROWS][COLS];

void matrix_init(void);
void matrix_scan(void);

#endif /* MATRIX_H */
