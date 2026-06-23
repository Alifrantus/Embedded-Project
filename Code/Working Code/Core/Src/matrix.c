#include "matrix.h"
#include "stm32f1xx_hal.h"

/* ============================================================
 *  PIN MAPPING — thực tế theo dây hàn trên PCB
 *
 *  ROW PINS : GPIOA → OUTPUT Push-Pull, Active LOW
 *  COL PINS : GPIOB → INPUT Pull-Up,    LOW = nhấn
 *
 *  Row mapping (đếm từ dưới lên):
 *    Hàng 1 (Ctrl/Space) = PA0 → keymap row 4
 *    Hàng 2 (Shift/Z)    = PA1 → keymap row 3
 *    Hàng 3 (Caps/A)     = PA2 → keymap row 2
 *    Hàng 4 (Tab/Q)      = PA3 → keymap row 1
 *    Hàng 5 (số)         = PA4 → keymap row 0
 *
 *  Col mapping (thứ tự GPIO theo dây hàn thực tế):
 *    col[ 0] → PB0   col[ 7] → PB15
 *    col[ 1] → PB1   col[ 8] → PB3
 *    col[ 2] → PB10  col[ 9] → PB4
 *    col[ 3] → PB11  col[10] → PB5
 *    col[ 4] → PB12  col[11] → PB6
 *    col[ 5] → PB13  col[12] → PB7
 *    col[ 6] → PB14  col[13] → PB8
 *
 *  Sơ đồ nối:
 *    ROW(OUT) ── 1N4148(Cathode→ROW) ── Switch ── COL(IN PULLUP)
 * ============================================================ */

uint8_t matrix_raw[ROWS][COLS];

/* Row pins — thứ tự theo keymap row 0–4, PA pin đảo ngược */
static GPIO_TypeDef* const row_ports[ROWS] = {
    GPIOA, GPIOA, GPIOA, GPIOA, GPIOA
};
static const uint16_t row_pins[ROWS] = {
    GPIO_PIN_4,  /* row 0 — hàng số   (hàng 5 vật lý) → PA4 */
    GPIO_PIN_3,  /* row 1 — hàng Tab  (hàng 4 vật lý) → PA3 */
    GPIO_PIN_2,  /* row 2 — hàng Caps (hàng 3 vật lý) → PA2 */
    GPIO_PIN_1,  /* row 3 — hàng Shft (hàng 2 vật lý) → PA1 */
    GPIO_PIN_0,  /* row 4 — hàng Ctrl (hàng 1 vật lý) → PA0 */
};

/* Col pins — thứ tự theo col logic 0–13, GPIO pin theo dây hàn */
static GPIO_TypeDef* const col_ports[COLS] = {
    GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB,
    GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB
};
static const uint16_t col_pins[COLS] = {
    GPIO_PIN_0,   /* col 0  → PB0  */
    GPIO_PIN_1,   /* col 1  → PB1  */
    GPIO_PIN_10,  /* col 2  → PB10 */
    GPIO_PIN_11,  /* col 3  → PB11 */
    GPIO_PIN_12,  /* col 4  → PB12 */
    GPIO_PIN_13,  /* col 5  → PB13 */
    GPIO_PIN_14,  /* col 6  → PB14 */
    GPIO_PIN_15,  /* col 7  → PB15 */
    GPIO_PIN_3,   /* col 8  → PB3  */
    GPIO_PIN_4,   /* col 9  → PB4  */
    GPIO_PIN_5,   /* col 10 → PB5  */
    GPIO_PIN_6,   /* col 11 → PB6  */
    GPIO_PIN_7,   /* col 12 → PB7  */
    GPIO_PIN_8,   /* col 13 → PB8  */
};

/* ============================================================
 *  matrix_init — set tất cả row HIGH (inactive)
 * ============================================================ */
void matrix_init(void) {
    for (int r = 0; r < ROWS; r++) {
        HAL_GPIO_WritePin(row_ports[r], row_pins[r], GPIO_PIN_SET);
    }
}

/* ============================================================
 *  matrix_scan — quét toàn bộ 5×14
 *
 *  Mỗi row:
 *    1. Kéo row xuống LOW
 *    2. Chờ ~1µs settle
 *    3. Đọc 14 col: LOW = nhấn → raw = 1
 *    4. Khôi phục row lên HIGH
 * ============================================================ */
void matrix_scan(void) {
    for (int r = 0; r < ROWS; r++) {
        HAL_GPIO_WritePin(row_ports[r], row_pins[r], GPIO_PIN_RESET);

        /* ~1µs @ 72MHz */
        for (volatile int d = 0; d < 72; d++);

        for (int c = 0; c < COLS; c++) {
            matrix_raw[r][c] =
                (HAL_GPIO_ReadPin(col_ports[c], col_pins[c]) == GPIO_PIN_RESET)
                ? 1 : 0;
        }

        HAL_GPIO_WritePin(row_ports[r], row_pins[r], GPIO_PIN_SET);
    }
}
