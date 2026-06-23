#include "debounce.h"
#include "matrix.h"
#include <string.h>

/* ============================================================
 *  DEBOUNCE ALGORITHM — "Integrator / Counter" method
 *
 *  Nguyên lý:
 *    - Theo dõi raw signal mới nhất (raw_prev)
 *    - Nếu raw signal thay đổi → reset counter về 0
 *    - Nếu raw signal không đổi → tăng counter
 *    - Khi counter >= DEBOUNCE_TIME → chấp nhận vào debounced[][]
 *
 *  Điều này đảm bảo: tín hiệu phải ổn định liên tục DEBOUNCE_TIME
 *  lần scan thì mới được công nhận — loại bỏ bounce hiệu quả.
 * ============================================================ */

uint8_t debounced[ROWS][COLS];

static uint8_t raw_prev[ROWS][COLS];
static uint8_t counter[ROWS][COLS];

void debounce_init(void) {
    memset(debounced,  0, sizeof(debounced));
    memset(raw_prev,   0, sizeof(raw_prev));
    memset(counter,    0, sizeof(counter));
}

void debounce_process(void) {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {

            uint8_t raw = matrix_raw[r][c];

            if (raw != raw_prev[r][c]) {
                /* Tín hiệu vừa thay đổi → reset, chưa tin tưởng */
                counter[r][c]  = 0;
                raw_prev[r][c] = raw;
            } else {
                /* Tín hiệu ổn định → đếm thêm */
                if (counter[r][c] < DEBOUNCE_TIME) {
                    counter[r][c]++;
                }

                /* Khi đã đủ số lần ổn định → cập nhật debounced */
                if (counter[r][c] >= DEBOUNCE_TIME) {
                    debounced[r][c] = raw;
                }
            }
        }
    }
}
