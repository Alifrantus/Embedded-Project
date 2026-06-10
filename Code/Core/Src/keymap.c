#include "keymap.h"

/* Bản đồ phím 2 hàng x 2 cột */
const uint8_t keymap[NUM_LAYERS][ROWS][COLS] = {
    {
        /* COL0 (PB1)     COL1 (PB0) */
        {  KC_A,         KC_B  },    /* ROW0 (PB11): Nút 1 và Nút 2 */
        {  KC_C,         KC_D  }     /* ROW1 (PB10): Nút 3 và Nút 4 */
    }
};

uint8_t keymap_get(uint8_t layer, uint8_t row, uint8_t col) {
    if (layer >= NUM_LAYERS || row >= ROWS || col >= COLS) return KC_NO;
    return keymap[layer][row][col];
}