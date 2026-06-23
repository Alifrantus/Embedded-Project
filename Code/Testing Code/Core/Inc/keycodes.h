#ifndef KEYCODES_H
#define KEYCODES_H

/* ============================================================
 *  USB HID KEYCODES (HID Usage Table 1.12 — Keyboard Page 0x07)
 * ============================================================ */

/* Special */
#define KC_NONE     0x00
#define KC_NO       0xFF    /* ô ma trận không có switch vật lý — bỏ qua */
#define KC_ERR_OVF  0x01    // Keyboard Error Roll Over

/* Letters */
#define KC_A        0x04
#define KC_B        0x05
#define KC_C        0x06
#define KC_D        0x07
#define KC_E        0x08
#define KC_F        0x09
#define KC_G        0x0A
#define KC_H        0x0B
#define KC_I        0x0C
#define KC_J        0x0D
#define KC_K        0x0E
#define KC_L        0x0F
#define KC_M        0x10
#define KC_N        0x11
#define KC_O        0x12
#define KC_P        0x13
#define KC_Q        0x14
#define KC_R        0x15
#define KC_S        0x16
#define KC_T        0x17
#define KC_U        0x18
#define KC_V        0x19
#define KC_W        0x1A
#define KC_X        0x1B
#define KC_Y        0x1C
#define KC_Z        0x1D

/* Numbers row */
#define KC_1        0x1E
#define KC_2        0x1F
#define KC_3        0x20
#define KC_4        0x21
#define KC_5        0x22
#define KC_6        0x23
#define KC_7        0x24
#define KC_8        0x25
#define KC_9        0x26
#define KC_0        0x27

/* Special keys */
#define KC_ENTER    0x28
#define KC_ESC      0x29
#define KC_BSPC     0x2A
#define KC_TAB      0x2B
#define KC_SPACE    0x2C
#define KC_MINUS    0x2D    // -
#define KC_EQUAL    0x2E    // =
#define KC_LBRC     0x2F    // [
#define KC_RBRC     0x30    // ]
#define KC_BSLS     0x31    // backslash
#define KC_SCLN     0x33    // ;
#define KC_QUOT     0x34    // '
#define KC_GRV      0x35    // `
#define KC_COMM     0x36    // ,
#define KC_DOT      0x37    // .
#define KC_SLSH     0x38    // /
#define KC_CAPS     0x39

/* Function keys */
#define KC_F1       0x3A
#define KC_F2       0x3B
#define KC_F3       0x3C
#define KC_F4       0x3D
#define KC_F5       0x3E
#define KC_F6       0x3F
#define KC_F7       0x40
#define KC_F8       0x41
#define KC_F9       0x42
#define KC_F10      0x43
#define KC_F11      0x44
#define KC_F12      0x45

/* Navigation */
#define KC_PSCR     0x46    // Print Screen
#define KC_SLCK     0x47    // Scroll Lock
#define KC_PAUS     0x48    // Pause
#define KC_INS      0x49
#define KC_HOME     0x4A
#define KC_PGUP     0x4B
#define KC_DEL      0x4C
#define KC_END      0x4D
#define KC_PGDN     0x4E
#define KC_RIGHT    0x4F
#define KC_LEFT     0x50
#define KC_DOWN     0x51
#define KC_UP       0x52

/* Numpad */
#define KC_NLCK     0x53
#define KC_PSLS     0x54
#define KC_PAST     0x55
#define KC_PMNS     0x56
#define KC_PPLS     0x57
#define KC_PENT     0x58
#define KC_P1       0x59
#define KC_P2       0x5A
#define KC_P3       0x5B
#define KC_P4       0x5C
#define KC_P5       0x5D
#define KC_P6       0x5E
#define KC_P7       0x5F
#define KC_P8       0x60
#define KC_P9       0x61
#define KC_P0       0x62
#define KC_PDOT     0x63

/* ============================================================
 *  MODIFIER BITMASK (report[0])
 * ============================================================ */
#define MOD_NONE    0x00
#define MOD_LCTRL   (1 << 0)
#define MOD_LSHIFT  (1 << 1)
#define MOD_LALT    (1 << 2)
#define MOD_LGUI    (1 << 3)
#define MOD_RCTRL   (1 << 4)
#define MOD_RSHIFT  (1 << 5)
#define MOD_RALT    (1 << 6)
#define MOD_RGUI    (1 << 7)

/* ============================================================
 *  SPECIAL FIRMWARE KEYCODES (>= 0xF0, không gửi qua USB)
 * ============================================================ */
#define KC_FN       0xF0    // Fn layer toggle (hold)
#define KC_LCTL     0xF1    // Left Control (maps to modifier)
#define KC_LSFT     0xF2    // Left Shift
#define KC_LALT_K   0xF3    // Left Alt
#define KC_LGUI_K   0xF4    // Left GUI (Win/Cmd)
#define KC_RCTL     0xF5    // Right Control
#define KC_RSFT     0xF6    // Right Shift
#define KC_RALT_K   0xF7    // Right Alt
#define KC_RGUI_K   0xF8    // Right GUI

/* Helper macro */
#define IS_MODIFIER(kc)  ((kc) >= 0xF1 && (kc) <= 0xF8)
#define IS_FN(kc)        ((kc) == KC_FN)
#define IS_NORMAL(kc)    ((kc) > 0x00 && (kc) < 0xF0)
#define IS_VALID(kc)     ((kc) != KC_NO && (kc) != KC_NONE)

#endif /* KEYCODES_H */
