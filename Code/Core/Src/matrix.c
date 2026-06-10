#include "matrix.h"
#include "stm32f1xx_hal.h"  /* Thư viện HAL của STM32F1 dùng trong CubeIDE */

uint8_t matrix_raw[ROWS][COLS];

/* Định nghĩa lại chân Hàng: ROW0 = PB11, ROW1 = PB10 */
static GPIO_TypeDef* const row_ports[ROWS] = {
    GPIOB, GPIOB
};
static const uint16_t row_pins[ROWS] = {
    GPIO_PIN_11, GPIO_PIN_10
};

/* Định nghĩa lại chân Cột: COL0 = PB1, COL1 = PB0 */
static GPIO_TypeDef* const col_ports[COLS] = {
    GPIOB, GPIOB
};
static const uint16_t col_pins[COLS] = {
    GPIO_PIN_1,  GPIO_PIN_0
};

/* Khởi tạo: Cho tất cả các hàng lên mức HIGH */
void matrix_init(void) {
    for (int r = 0; r < ROWS; r++) {
        HAL_GPIO_WritePin(row_ports[r], row_pins[r], GPIO_PIN_SET);
    }
}

/* Quét ma trận phím 2x2 */
void matrix_scan(void) {
    for (int r = 0; r < ROWS; r++) {
        // 1. Kéo hàng hiện tại xuống LOW
        HAL_GPIO_WritePin(row_ports[r], row_pins[r], GPIO_PIN_RESET);

        // 2. Chờ điện áp ổn định trên breadboard (settle time)
        // Vòng lặp cũ "for d < 72" chỉ tốn ~1µs, quá ngắn cho điện dung ký sinh
        // trên breadboard. HAL_Delay(1) cho phép 1ms để điện áp COL xả xuống LOW ổn định.
        HAL_Delay(1);

        // 3. Đọc trạng thái của 2 cột
        for (int c = 0; c < COLS; c++) {
            // Vì mạch KHÔNG DIODE và kéo Pull-up, khi bấm nút chân COL sẽ bị kéo xuống LOW (RESET)
            if (HAL_GPIO_ReadPin(col_ports[c], col_pins[c]) == GPIO_PIN_RESET) {
                matrix_raw[r][c] = 1; // Phím được nhấn
            } else {
                matrix_raw[r][c] = 0; // Phím thả
            }
        }

        // 4. Khôi phục hàng lên mức HIGH trở lại
        HAL_GPIO_WritePin(row_ports[r], row_pins[r], GPIO_PIN_SET);

        // 5. Chờ hàng vừa giải phóng kịp dâng lên HIGH trước khi kéo hàng tiếp theo xuống
        // Tránh điện áp tràn sang hàng kế, gây đọc nhầm phím ghost.
        HAL_Delay(1);
    }
}
