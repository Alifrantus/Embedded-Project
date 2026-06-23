#ifndef USB_HID_H
#define USB_HID_H

#include <stdint.h>

/* ============================================================
 *  HID Report Structure — Boot Keyboard Protocol
 *  8 bytes: [modifier][reserved][key0][key1][key2][key3][key4][key5]
 * ============================================================ */

#define MAX_KEYS_IN_REPORT  6

typedef struct {
    uint8_t modifier;           /* Byte 0: modifier bitmask */
    uint8_t reserved;           /* Byte 1: always 0 */
    uint8_t keys[MAX_KEYS_IN_REPORT]; /* Bytes 2-7: keycodes (6KRO) */
} hid_report_t;

/* ============================================================
 *  Modifier bitmask (byte 0 của HID report)
 * ============================================================ */
#define MOD_LCTRL   (1 << 0)
#define MOD_LSHIFT  (1 << 1)
#define MOD_LALT    (1 << 2)
#define MOD_LGUI    (1 << 3)
#define MOD_RCTRL   (1 << 4)
#define MOD_RSHIFT  (1 << 5)
#define MOD_RALT    (1 << 6)
#define MOD_RGUI    (1 << 7)

/* ============================================================
 *  API
 * ============================================================ */

/**
 * @brief Khởi tạo: xóa cache report về 0
 */
void usb_hid_init(void);

/**
 * @brief Gửi HID report nếu có thay đổi so với lần trước
 *        (tránh flood USB bus khi không có phím nào đổi)
 * @param report  con trỏ đến hid_report_t cần gửi
 */
void usb_hid_send(const hid_report_t *report);

#endif /* USB_HID_H */
