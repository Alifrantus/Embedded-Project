#include "usb_hid.h"
#include "usbd_hid.h"
#include "usb_device.h"
#include <string.h>

extern USBD_HandleTypeDef hUsbDeviceFS;

/* ============================================================
 *  Cache report trước đó để tránh gửi trùng lặp
 *  → giảm USB bandwidth và tải CPU host
 * ============================================================ */
static hid_report_t last_report;
static uint8_t initialized = 0;

void usb_hid_init(void) {
    memset(&last_report, 0, sizeof(last_report));
    initialized = 1;
}

/* ============================================================
 *  usb_hid_send
 *  Chỉ gửi nếu report thay đổi so với lần trước.
 *  USBD_HID_SendReport trả về USBD_OK (0) nếu thành công.
 * ============================================================ */
void usb_hid_send(const hid_report_t *report) {
    if (!initialized) return;

    /* Chỉ gửi khi có thay đổi */
    if (memcmp(report, &last_report, sizeof(hid_report_t)) == 0) return;

    USBD_HID_SendReport(
        &hUsbDeviceFS,
        (uint8_t *)report,
        sizeof(hid_report_t)
    );

    memcpy(&last_report, report, sizeof(hid_report_t));
}

void usb_hid_send_empty(void) {
    hid_report_t empty = {0};
    usb_hid_send(&empty);
}
