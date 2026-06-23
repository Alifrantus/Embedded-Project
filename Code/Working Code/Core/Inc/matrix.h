#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>
#include "config.h"

/* ============================================================
 *  MATRIX — raw scan result
 *  matrix_raw[r][c] = 1 nếu phím (row r, col c) đang được nhấn
 *                   = 0 nếu nhả
 * ============================================================ */
extern uint8_t matrix_raw[ROWS][COLS];

/**
 * @brief  Khởi tạo ma trận: đặt tất cả row về HIGH (idle).
 *         GPIO mode phải được cấu hình sẵn bởi MX_GPIO_Init().
 */
void matrix_init(void);

/**
 * @brief  Quét toàn bộ ma trận 5×14 một lần.
 *         Kết quả ghi vào matrix_raw[][].
 *         Gọi mỗi SCAN_INTERVAL_MS ms trong main loop.
 */
void matrix_scan(void);

#endif /* MATRIX_H */
