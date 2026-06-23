#include "keymap.h"

/* ============================================================
 *  KEYMAP — 60% ANSI CHUẨN, 61 phím, ma trận 5×14
 *
 *  KC_NO = ô không có switch vật lý (firmware bỏ qua)
 *
 *  Layout Layer 0 (Default):
 *
 *  Row 0: ` 1 2 3 4 5 6 7 8 9 0 - = [BSpc]
 *  Row 1: [Tab] Q W E R T Y U I O P [ ] \
 *  Row 2: [Caps] A S D F G H J K L ; ' [Enter]
 *  Row 3: [LShift] Z X C V B N M , . / [RShift]
 *  Row 4: [LCtrl] [LGui] [LAlt] [    Space 6.25u    ] [RAlt] [RGui] [Fn] [RCtrl]
 *
 *  Layout Layer 1 (Fn held):
 *
 *  Row 0: ESC F1 F2 F3 F4 F5 F6 F7 F8 F9 F10 F11 F12 [Del]
 *  Row 1: [ ] Home Up End PgUp [ ] [ ] [ ] [ ] [ ] ScLk Pause PrtSc
 *  Row 2: [ ] Left Down Right PgDn [ ] [ ] [ ] [ ] [ ] [ ] [ ]
 *  Row 3: [ ] Vol- Vol+ Mute [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ]
 *  Row 4: tất cả giữ nguyên, Fn vẫn là Fn
 * ============================================================ */

#define ___ KC_NO

const uint8_t keymap[NUM_LAYERS][ROWS][COLS] = {

    /* =====================================================
     *  LAYER 0 — DEFAULT
     *  col index:  0          1          2           3          4     5     6     7     8     9           10         11       12       13
     * ===================================================== */
    {
        /* r0 */  { KC_GRV,   KC_1,      KC_2,       KC_3,      KC_4, KC_5, KC_6, KC_7, KC_8, KC_9,       KC_0,      KC_MINUS,KC_EQUAL,KC_BSPC  },
        /* r1 */  { KC_TAB,   KC_Q,      KC_W,       KC_E,      KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O,       KC_P,      KC_LBRC, KC_RBRC, KC_BSLS  },
        /* r2 */  { KC_CAPS,  KC_A,      KC_S,       KC_D,      KC_F, KC_G, KC_H, KC_J, KC_K, KC_L,       KC_SCLN,   KC_QUOT, KC_ENTER,___       },
        /* r3 */  { KC_LSFT,  KC_Z,      KC_X,       KC_C,      KC_V, KC_B, KC_N, KC_M, KC_COMM,KC_DOT,   KC_SLSH,   KC_RSFT, ___,     ___       },
        /* r4 */  { KC_LCTL,  KC_LGUI_K, KC_LALT_K,  KC_SPACE,  ___,  ___,  ___,  ___,  ___,  KC_RALT_K,  KC_RGUI_K, KC_FN,   KC_RCTL, ___       },
    },

    /* =====================================================
     *  LAYER 1 — FN
     *  col index:  0          1          2           3          4      5     6     7     8     9      10     11       12       13
     * ===================================================== */
    {
        /* r0 */  { KC_ESC,   KC_F1,     KC_F2,      KC_F3,     KC_F4, KC_F5,KC_F6,KC_F7,KC_F8,KC_F9,  KC_F10,KC_F11,  KC_F12,  KC_DEL  },
        /* r1 */  { ___,      KC_HOME,   KC_UP,      KC_END,    KC_PGUP,___,  ___,  ___,  ___,  ___,    ___,   KC_SLCK, KC_PAUS, KC_PSCR },
        /* r2 */  { ___,      KC_LEFT,   KC_DOWN,    KC_RIGHT,  KC_PGDN,___,  ___,  ___,  ___,  ___,    ___,   ___,     ___,     ___     },
        /* r3 */  { ___,      0xE0,      0xE1,       0xE2,      ___,  ___,  ___,  ___,  ___,  ___,    ___,   ___,     ___,     ___     },
        /* r4 */  { ___,      ___,       ___,        ___,       ___,  ___,  ___,  ___,  ___,  ___,    ___,   KC_FN,   ___,     ___     },
    },
};

uint8_t keymap_get(uint8_t layer, uint8_t row, uint8_t col) {
    if (layer >= NUM_LAYERS || row >= ROWS || col >= COLS) return KC_NO;
    return keymap[layer][row][col];
}
