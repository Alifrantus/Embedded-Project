#ifndef CONFIG_H
#define CONFIG_H

/* ============================================================
 *  KEYBOARD MATRIX CONFIG
 *  Layout  : 60% ANSI chuẩn — 61 phím
 *  PCB     : GH60 / DZ60 compatible
 *  Ma trận : 5 rows × 14 cols = 70 ô vật lý
 *            nhưng chỉ 61 ô có switch thật
 *            (các phím wide như Backspace, Shift, Enter
 *             chiếm 1 switch, còn lại là KC_NO)
 * ============================================================ */

#define ROWS                5
#define COLS                14
#define KEY_COUNT           61      /* số switch thực tế */

/* Debounce: số lần scan liên tiếp ổn định mới chấp nhận     */
/* @ 1ms/scan → DEBOUNCE_TIME=10 tương đương ~10ms           */
#define DEBOUNCE_TIME       10

/* USB HID Boot Protocol report                               */
#define HID_REPORT_SIZE     8
#define MAX_KEYS_IN_REPORT  6       /* 6KRO */

/* Scan rate                                                   */
#define SCAN_INTERVAL_MS    1

/* Layer                                                       */
#define NUM_LAYERS          2       /* 0 = default, 1 = Fn    */

#endif /* CONFIG_H */
