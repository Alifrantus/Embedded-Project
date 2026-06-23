#include "keyboard.h"
#include "debounce.h"
#include "usb_hid.h"
#include "usbd_hid.h"
#include <string.h>

extern USBD_HandleTypeDef hUsbDeviceFS;

/* ============================================================
 *  HID KEYCODE TABLE (USB HID Usage Table)
 * ============================================================ */
#define KC_NO    0x00
#define KC_A     0x04
#define KC_B     0x05
#define KC_C     0x06
#define KC_D     0x07
#define KC_E     0x08
#define KC_F     0x09
#define KC_G     0x0A
#define KC_H     0x0B
#define KC_I     0x0C
#define KC_J     0x0D
#define KC_K     0x0E
#define KC_L     0x0F
#define KC_M     0x10
#define KC_N     0x11
#define KC_O     0x12
#define KC_P     0x13
#define KC_Q     0x14
#define KC_R     0x15
#define KC_S     0x16
#define KC_T     0x17
#define KC_U     0x18
#define KC_V     0x19
#define KC_W     0x1A
#define KC_X     0x1B
#define KC_Y     0x1C
#define KC_Z     0x1D
#define KC_1     0x1E
#define KC_2     0x1F
#define KC_3     0x20
#define KC_4     0x21
#define KC_5     0x22
#define KC_6     0x23
#define KC_7     0x24
#define KC_8     0x25
#define KC_9     0x26
#define KC_0     0x27
#define KC_ENT   0x28
#define KC_ESC   0x29
#define KC_BSPC  0x2A
#define KC_TAB   0x2B
#define KC_SPC   0x2C
#define KC_MINS  0x2D  /* - */
#define KC_EQL   0x2E  /* = */
#define KC_LBRC  0x2F  /* [ */
#define KC_RBRC  0x30  /* ] */
#define KC_BSLS  0x31  /* \ */
#define KC_SCLN  0x33  /* ; */
#define KC_QUOT  0x34  /* ' */
#define KC_GRV   0x35  /* ` */
#define KC_COMM  0x36  /* , */
#define KC_DOT   0x37  /* . */
#define KC_SLSH  0x38  /* / */
#define KC_CAPS  0x39
#define KC_F1    0x3A
#define KC_F2    0x3B
#define KC_F3    0x3C
#define KC_F4    0x3D
#define KC_F5    0x3E
#define KC_F6    0x3F
#define KC_F7    0x40
#define KC_F8    0x41
#define KC_F9    0x42
#define KC_F10   0x43
#define KC_F11   0x44
#define KC_F12   0x45
#define KC_PSCR  0x46  /* Print Screen */
#define KC_SLCK  0x47  /* Scroll Lock  */
#define KC_PAUS  0x48  /* Pause        */
#define KC_INS   0x49
#define KC_HOME  0x4A
#define KC_PGUP  0x4B
#define KC_DEL   0x4C
#define KC_END   0x4D
#define KC_PGDN  0x4E
#define KC_RGHT  0x4F  /* Arrow Right  */
#define KC_LEFT  0x50  /* Arrow Left   */
#define KC_DOWN  0x51  /* Arrow Down   */
#define KC_UP    0x52  /* Arrow Up     */
#define KC_LCTL  0xE0
#define KC_LSFT  0xE1
#define KC_LALT  0xE2
#define KC_LGUI  0xE3  /* Win Left  */
#define KC_RCTL  0xE4
#define KC_RSFT  0xE5
#define KC_RALT  0xE6
#define KC_APP   0x65  /* Menu */

/* KC_FN — mã nội bộ, không gửi qua HID */
#define KC_FN    0xFF

/* ============================================================
 *  MODIFIER BIT FLAGS
 * ============================================================ */
#ifndef MOD_LCTL
#define MOD_LCTL  (1<<0)
#endif
#ifndef MOD_LSFT
#define MOD_LSFT  (1<<1)
#endif
#ifndef MOD_LALT
#define MOD_LALT  (1<<2)
#endif
#ifndef MOD_LGUI
#define MOD_LGUI  (1<<3)
#endif
#ifndef MOD_RCTL
#define MOD_RCTL  (1<<4)
#endif
#ifndef MOD_RSFT
#define MOD_RSFT  (1<<5)
#endif
#ifndef MOD_RALT
#define MOD_RALT  (1<<6)
#endif

/* ============================================================
 *  KEYMAP 5×14 — Layout 60% ANSI
 *
 *  Row 0: `   1   2   3   4   5   6   7   8   9   0   -   =  Bksp
 *  Row 1: Tab  Q   W   E   R   T   Y   U   I   O   P   [   ]   \
 *  Row 2: Caps  A   S   D   F   G   H   J   K   L   ;   '  Enter  NO
 *  Row 3: LShft  Z   X   C   V   B   N   M   ,   .   /  RShft  NO  NO
 *  Row 4: LCtrl LWin LAlt  NO  NO  Spc  NO  NO  RAlt  FN  Menu  RCtrl NO NO
 *
 *  col:    0    1    2    3    4    5    6    7    8    9   10   11   12   13
 * ============================================================ */
static const uint8_t keymap[ROWS][COLS] = {
    /* Row 0 — hàng số */
    { KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,
      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,
      KC_EQL,  KC_BSPC },

    /* Row 1 — hàng Tab/Q */
    { KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,
      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,
      KC_RBRC, KC_BSLS },

    /* Row 2 — hàng Caps/A */
    { KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,
      KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
      KC_ENT,  KC_NO },

    /* Row 3 — hàng Shift/Z */
    { KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,
      KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
      KC_NO,   KC_NO },

    /* Row 4 — hàng Ctrl/Space
     *   col 9 = FN (thay RWin) */
    { KC_LCTL, KC_LGUI, KC_LALT, KC_NO,   KC_NO,   KC_SPC,
      KC_NO,   KC_NO,   KC_RALT, KC_FN,   KC_APP,  KC_RCTL,
      KC_NO,   KC_NO },
};

/* ============================================================
 *  FN LAYER — ánh xạ khi giữ FN
 *
 *  FN + `   = ESC
 *  FN + 1–9 = F1–F9
 *  FN + 0   = F10
 *  FN + -   = F11
 *  FN + =   = F12
 *  FN + P   = Print Screen
 *  FN + [   = Scroll Lock
 *  FN + ]   = Pause
 *  FN + ;   = Home
 *  FN + '   = End
 *  FN + /   = Insert
 *  FN + Del (Bksp) = Delete
 *  FN + H   = Left
 *  FN + J   = Down
 *  FN + K   = Up
 *  FN + L   = Right
 *  FN + N   = Page Up
 *  FN + M   = Page Down
 * ============================================================ */
static const uint8_t fn_layer[ROWS][COLS] = {
    /* Row 0 */
    { KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,
      KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
      KC_F12,  KC_DEL },

    /* Row 1 */
    { KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
      KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_PSCR, KC_SLCK,
      KC_PAUS, KC_NO },

    /* Row 2 */
    { KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
      KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_HOME, KC_END,
      KC_NO,   KC_NO },

    /* Row 3 */
    { KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
      KC_PGUP, KC_PGDN, KC_NO,   KC_NO,   KC_INS,  KC_NO,
      KC_NO,   KC_NO },

    /* Row 4 */
    { KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
      KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
      KC_NO,   KC_NO },
};

/* ============================================================
 *  Xác định phím modifier
 * ============================================================ */
static uint8_t get_modifier_bit(uint8_t kc) {
    switch (kc) {
        case KC_LCTL: return MOD_LCTL;
        case KC_LSFT: return MOD_LSFT;
        case KC_LALT: return MOD_LALT;
        case KC_LGUI: return MOD_LGUI;
        case KC_RCTL: return MOD_RCTL;
        case KC_RSFT: return MOD_RSFT;
        case KC_RALT: return MOD_RALT;
        default:      return 0;
    }
}

/* ============================================================
 *  keyboard_init
 * ============================================================ */
void keyboard_init(void) {}

/* ============================================================
 *  keyboard_task — gọi mỗi 1ms từ main loop
 *
 *  Logic:
 *    1. Kiểm tra FN có đang giữ không (row4, col9)
 *    2. Nếu FN giữ → dùng fn_layer thay keymap
 *    3. Tạo HID report 8 byte (6KRO)
 * ============================================================ */
void keyboard_task(void) {
    uint8_t report[8] = {0};
    uint8_t key_count = 0;

    /* Kiểm tra FN (row 4, col 9) */
    uint8_t fn_held = debounced[4][9];

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            if (!debounced[r][c]) continue;

            /* Bỏ qua chính phím FN */
            if (r == 4 && c == 9) continue;

            /* Chọn layer */
            uint8_t kc = fn_held ? fn_layer[r][c] : keymap[r][c];
            if (kc == KC_NO) continue;

            uint8_t mod = get_modifier_bit(kc);
            if (mod) {
                report[0] |= mod;
            } else {
                if (key_count < 6) {
                    report[2 + key_count] = kc;
                    key_count++;
                }
            }
        }
    }

    USBD_HID_SendReport(&hUsbDeviceFS, report, 8);
}
