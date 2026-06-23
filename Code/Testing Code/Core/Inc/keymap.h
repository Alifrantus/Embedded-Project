#ifndef KEYMAP_H
#define KEYMAP_H

#include <stdint.h>
#include "config.h"
#include "keycodes.h"

/* Bảng keymap [layer][row][col] */
extern const uint8_t keymap[NUM_LAYERS][ROWS][COLS];

uint8_t keymap_get(uint8_t layer, uint8_t row, uint8_t col);

#endif /* KEYMAP_H */
