#include "usb_hid.h"
#include "usbd_hid.h"
#include "usb_device.h"
#include <string.h>

/* Cache report lần gửi trước — chỉ gửi khi có thay đổi */
static hid_report_t last_report;

extern USBD_HandleTypeDef hUsbDeviceFS;

void usb_hid_init(void) {
    memset(&last_report, 0, sizeof(last_report));
}

void usb_hid_send(const hid_report_t *report) {
    /* Chỉ gửi nếu report thay đổi */
    if (memcmp(report, &last_report, sizeof(hid_report_t)) == 0) {
        return;
    }

    /* Gửi 8 bytes qua USB HID */
    uint8_t buf[8];
    buf[0] = report->modifier;
    buf[1] = report->reserved;
    memcpy(&buf[2], report->keys, MAX_KEYS_IN_REPORT);

    USBD_HID_SendReport(&hUsbDeviceFS, buf, 8);

    /* Lưu lại cache */
    memcpy(&last_report, report, sizeof(hid_report_t));
}
