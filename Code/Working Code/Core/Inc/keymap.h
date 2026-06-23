#ifndef KEYMAP_H
#define KEYMAP_H

#include <stdint.h>
#include "config.h"
#include "keycodes.h"

/* ============================================================
 *  KEYMAP — bảng ánh xạ (layer, row, col) → keycode
 *  Kích thước: NUM_LAYERS × ROWS × COLS
 * ============================================================ */
extern const uint8_t keymap[NUM_LAYERS][ROWS][COLS];

/**
 * @brief  Lấy keycode tại vị trí (layer, row, col).
 *         Trả về KC_NO nếu tham số ngoài phạm vi.
 *
 * @param  layer  Layer index (0 = default, 1 = Fn)
 * @param  row    Chỉ số hàng (0 .. ROWS-1)
 * @param  col    Chỉ số cột  (0 .. COLS-1)
 * @return uint8_t  HID keycode hoặc firmware keycode đặc biệt
 */
uint8_t keymap_get(uint8_t layer, uint8_t row, uint8_t col);

#endif /* KEYMAP_H */
