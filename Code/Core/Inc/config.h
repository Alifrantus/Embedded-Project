#ifndef CONFIG_H
#define CONFIG_H

/* ============================================================
 * KEYBOARD MATRIX CONFIG - Cụm Test 2x2 Không Diode
 * ============================================================ */

#define ROWS                2       /* Cấu hình 2 hàng */
#define COLS                2       /* Cấu hình 2 cột */
#define KEY_COUNT           4       /* Tổng số 4 switch thực tế */

/* Chống dội phím (Debounce): Đợi N chu kỳ quét ổn định liên tục
 *
 * Tính toán thời gian thực tế:
 *   matrix_scan() = 2 hàng x (HAL_Delay(1) settle + HAL_Delay(1) inter-row)
 *                 ~ 4 ms / chu ky
 *
 *   DEBOUNCE_TIME = 10  ->  10 x 4ms = 40ms  <- QUA CHAT cho breadboard
 *   DEBOUNCE_TIME =  3  ->   3 x 4ms = 12ms  <- Du loc bounce Cherry MX (~5ms)
 *
 * Cherry MX bounce time thuc do: 1-5ms.
 * HAL_Delay(1) trong matrix_scan da hap thu phan lon nhieu dien dung breadboard.
 * Bo dem debounce chi can loc not phan bounce co hoc con sot lai. */
#define DEBOUNCE_TIME        3

/* USB HID Cấu hình */
#define HID_REPORT_SIZE     8
#define MAX_KEYS_IN_REPORT  6       

#define SCAN_INTERVAL_MS    1
#define NUM_LAYERS          1       /* Chỉ cần 1 layer để test */

#endif /* CONFIG_H */
