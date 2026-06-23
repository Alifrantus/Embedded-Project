#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include <stdint.h>
#include "config.h"

/* ============================================================
 *  DEBOUNCE — kết quả sau khi lọc bounce
 *  debounced[r][c] = 1 nếu phím ổn định ở trạng thái nhấn
 *                  = 0 nếu nhả (hoặc chưa đủ DEBOUNCE_TIME lần ổn định)
 * ============================================================ */
extern uint8_t debounced[ROWS][COLS];

/**
 * @brief  Khởi tạo bộ lọc debounce: xóa toàn bộ bộ đếm và trạng thái.
 */
void debounce_init(void);

/**
 * @brief  Xử lý debounce từ matrix_raw[][].
 *         Phải gọi SAU matrix_scan() mỗi chu kỳ.
 *         Cập nhật debounced[][] khi tín hiệu ổn định >= DEBOUNCE_TIME lần.
 */
void debounce_process(void);

#endif /* DEBOUNCE_H */
