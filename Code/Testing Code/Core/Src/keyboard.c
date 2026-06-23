#include "keyboard.h"
#include "debounce.h"
#include "keymap.h"
#include "usb_hid.h"
#include "keycodes.h"
#include <string.h>

/* ============================================================
 *  KEYBOARD PROCESSOR
 *
 *  Luồng xử lý mỗi cycle:
 *    1. Duyệt toàn bộ debounced[][]
 *    2. Phát hiện Fn key → chọn layer
 *    3. Phân loại modifier key vs normal key
 *    4. Build HID report (modifier byte + tối đa 6 keycodes)
 *    5. Gửi qua USB nếu có thay đổi
 *
 *  Features:
 *    - 6KRO (6-Key Rollover): tối đa 6 phím thường đồng thời
 *    - Tất cả modifier không giới hạn (bitmask)
 *    - Fn layer hold: nhấn giữ Fn để chuyển layer 1
 *    - Key held: giữ phím = liên tục trong report (OS xử lý repeat)
 * ============================================================ */

static uint8_t current_layer = 0;

/* Map firmware modifier keycode → bit trong modifier byte */
static uint8_t modifier_bit(uint8_t kc) {
    switch (kc) {
        case KC_LCTL:   return MOD_LCTRL;
        case KC_LSFT:   return MOD_LSHIFT;
        case KC_LALT_K: return MOD_LALT;
        case KC_LGUI_K: return MOD_LGUI;
        case KC_RCTL:   return MOD_RCTRL;
        case KC_RSFT:   return MOD_RSHIFT;
        case KC_RALT_K: return MOD_RALT;
        case KC_RGUI_K: return MOD_RGUI;
        default:        return 0;
    }
}

void keyboard_init(void) {
    current_layer = 0;
}

uint8_t keyboard_get_layer(void) {
    return current_layer;
}

void keyboard_process(void) {
    hid_report_t report;
    memset(&report, 0, sizeof(report));

    uint8_t fn_held  = 0;
    int     key_idx  = 0;   /* index vào report.keys[0..5] */
    uint8_t overflow = 0;   /* nhiều hơn 6 phím → báo ERR_OVF */

    /* ---- Pass 1: kiểm tra Fn -------------------------------- */
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            if (!debounced[r][c]) continue;
            uint8_t kc = keymap_get(0, r, c);  /* Fn luôn check layer 0 */
            if (IS_FN(kc)) {
                fn_held = 1;
            }
        }
    }

    current_layer = fn_held ? 1 : 0;

    /* ---- Pass 2: build report ------------------------------- */
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            if (!debounced[r][c]) continue;

            uint8_t kc = keymap_get(current_layer, r, c);

            if (kc == KC_NONE || kc == KC_NO || IS_FN(kc)) {
                continue;
            }

            if (IS_MODIFIER(kc)) {
                /* Modifier → set bit tương ứng */
                report.modifier |= modifier_bit(kc);

            } else if (IS_NORMAL(kc)) {
                /* Normal key → thêm vào danh sách 6KRO */
                if (key_idx < MAX_KEYS_IN_REPORT) {
                    report.keys[key_idx++] = kc;
                } else {
                    overflow = 1;
                }
            }
            /* kc >= 0xE0: media key → TODO: Consumer HID report */
        }
    }

    /* Nếu quá 6 phím → gửi ERR_OVF theo chuẩn HID */
    if (overflow) {
        memset(report.keys, KC_ERR_OVF, MAX_KEYS_IN_REPORT);
    }

    usb_hid_send(&report);
}
