#ifndef USB_HID_H
#define USB_HID_H

#include <stdint.h>
#include "config.h"

/* Cấu trúc HID report chuẩn Boot Keyboard (8 bytes) */
typedef struct {
    uint8_t modifier;               /* Byte 0: modifier keys (Ctrl, Shift, Alt, GUI) */
    uint8_t reserved;               /* Byte 1: reserved, luôn = 0 */
    uint8_t keys[MAX_KEYS_IN_REPORT]; /* Byte 2-7: tối đa 6 keycodes */
} __attribute__((packed)) hid_report_t;

/* Modifier bitmask */
#define MOD_LCTRL   (1 << 0)
#define MOD_LSHIFT  (1 << 1)
#define MOD_LALT    (1 << 2)
#define MOD_LGUI    (1 << 3)
#define MOD_RCTRL   (1 << 4)
#define MOD_RSHIFT  (1 << 5)
#define MOD_RALT    (1 << 6)
#define MOD_RGUI    (1 << 7)

void usb_hid_init(void);
void usb_hid_send(const hid_report_t *report);
void usb_hid_send_empty(void);

#endif /* USB_HID_H */
