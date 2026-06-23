/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c  (DEBUG VERSION — HID self-report)
  * @brief          : Debug qua USB HID, không cần USB-TTL
  *
  * Cách hoạt động:
  *   Sau khi flash, mở Notepad (hoặc bất kỳ text editor nào).
  *   Board sẽ TỰ GÕ ra màn hình các thông tin debug:
  *
  *   [BOOT OK]
  *   GPIO: C0=1 C1=1 R0=1 R1=1
  *   WAIT KEY...
  *
  *   Khi bấm phím:
  *   KEY[0,0]=A
  *
  *   Heartbeat mỗi 5 giây (nếu không bấm gì):
  *   .
  *
  ******************************************************************************
  */
/* USER CODE END Header */
#include "main.h"
#include "usb_device.h"

/* USER CODE BEGIN Includes */
#include "matrix.h"
#include "debounce.h"
#include "keyboard.h"
#include "usb_hid.h"
#include "usbd_hid.h"
#include <string.h>
#include <stdio.h>
/* USER CODE END Includes */

/* ============================================================
 *  HID TYPE KEYS — gõ ký tự ra màn hình qua USB HID
 * ============================================================ */
extern USBD_HandleTypeDef hUsbDeviceFS;

/* Map ASCII printable → HID keycode (chỉ cần A-Z, 0-9, dấu câu cơ bản) */
typedef struct { uint8_t mod; uint8_t kc; } hid_char_t;

static hid_char_t ascii_to_hid(char c) {
    if (c >= 'a' && c <= 'z') return (hid_char_t){0x00, 0x04 + (c - 'a')};
    if (c >= 'A' && c <= 'Z') return (hid_char_t){0x02, 0x04 + (c - 'A')}; /* shift */
    if (c >= '1' && c <= '9') return (hid_char_t){0x00, 0x1E + (c - '1')};
    if (c == '0') return (hid_char_t){0x00, 0x27};
    if (c == ' ') return (hid_char_t){0x00, 0x2C};
    if (c == '\n')return (hid_char_t){0x00, 0x28}; /* Enter */
    if (c == '=') return (hid_char_t){0x00, 0x2E};
    if (c == '[') return (hid_char_t){0x00, 0x2F};
    if (c == ']') return (hid_char_t){0x00, 0x30};
    if (c == '.') return (hid_char_t){0x00, 0x37};
    if (c == ',') return (hid_char_t){0x00, 0x36};
    if (c == '-') return (hid_char_t){0x00, 0x2D};
    if (c == '/') return (hid_char_t){0x00, 0x38};
    if (c == ':') return (hid_char_t){0x02, 0x33}; /* shift+; */
    if (c == '!') return (hid_char_t){0x02, 0x1E};
    if (c == '<') return (hid_char_t){0x02, 0x36};
    if (c == '>') return (hid_char_t){0x02, 0x37};
    return (hid_char_t){0x00, 0x00}; /* unknown → skip */
}

static void hid_type_char(char c) {
    uint8_t report[8] = {0};
    hid_char_t h = ascii_to_hid(c);
    if (h.kc == 0x00) return;

    /* Key down */
    report[0] = h.mod;
    report[2] = h.kc;
    USBD_HID_SendReport(&hUsbDeviceFS, report, 8);
    HAL_Delay(20);

    /* Key up */
    memset(report, 0, 8);
    USBD_HID_SendReport(&hUsbDeviceFS, report, 8);
    HAL_Delay(20);
}

static void hid_type_string(const char *s) {
    while (*s) hid_type_char(*s++);
}

static void hid_type_int(int n) {
    char buf[12];
    snprintf(buf, sizeof(buf), "%d", n);
    hid_type_string(buf);
}

/* ============================================================
 *  GPIO INIT (giống main gốc)
 * ============================================================ */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

static void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* [FIX] Phải gọi GPIO_Init TRƯỚC rồi mới WritePin
     * Thứ tự cũ: WritePin → Init → Init  (sai: WritePin không có tác dụng)
     * Thứ tự mới: Init COL → Init ROW → WritePin ROW HIGH (đúng) */

    /* ① COL: PB0, PB1 — Input Pull-Up */
    GPIO_InitStruct.Pin  = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* ② ROW: PB10, PB11 — Output Push-Pull */
    GPIO_InitStruct.Pin   = GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* ③ SAU KHI init xong: kéo ROW lên HIGH (idle state) */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_SET);
}

/* ============================================================
 *  MAIN
 * ============================================================ */
int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USB_DEVICE_Init();
    matrix_init();
    debounce_init();
    keyboard_init();
    usb_hid_init();

    /* Đợi USB enumerate xong */
    HAL_Delay(1500);

    /* ---- BƯỚC 1: In boot message ---- */
    hid_type_string("[BOOT OK]\n");
    HAL_Delay(200);

    /* ---- BƯỚC 2: Kiểm tra GPIO idle ---- */
    uint8_t c0 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)  == GPIO_PIN_SET ? 1 : 0;
    uint8_t c1 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)  == GPIO_PIN_SET ? 1 : 0;
    uint8_t r0 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == GPIO_PIN_SET ? 1 : 0;
    uint8_t r1 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == GPIO_PIN_SET ? 1 : 0;

    hid_type_string("GPIO C0=");
    hid_type_int(c0);
    hid_type_string(" C1=");
    hid_type_int(c1);
    hid_type_string(" R0=");
    hid_type_int(r0);
    hid_type_string(" R1=");
    hid_type_int(r1);
    hid_type_char('\n');
    HAL_Delay(200);

    /* Cảnh báo nếu GPIO sai */
    if (c0 == 0 || c1 == 0) {
        hid_type_string("WARN COL=LOW short or stuck\n");
    }
    if (r0 == 0 || r1 == 0) {
        hid_type_string("WARN ROW=LOW init fail\n");
    }
    if (c0 && c1 && r0 && r1) {
        hid_type_string("GPIO OK\n");
    }
    HAL_Delay(200);

    /* ---- BƯỚC 3: Test scan thủ công từng hàng ---- */
    hid_type_string("SCAN TEST\n");
    HAL_Delay(100);

    /* Scan ROW0 */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
    HAL_Delay(2);
    uint8_t r0c0 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET ? 1 : 0;
    uint8_t r0c1 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET ? 1 : 0;
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
    HAL_Delay(1);

    /* Scan ROW1 */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
    HAL_Delay(2);
    uint8_t r1c0 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET ? 1 : 0;
    uint8_t r1c1 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET ? 1 : 0;
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);

    hid_type_string("R0 C0=");
    hid_type_int(r0c0);
    hid_type_string(" C1=");
    hid_type_int(r0c1);
    hid_type_char('\n');

    hid_type_string("R1 C0=");
    hid_type_int(r1c0);
    hid_type_string(" C1=");
    hid_type_int(r1c1);
    hid_type_char('\n');
    HAL_Delay(200);

    /* Cảnh báo nếu scan thấy phím lúc boot */
    if (r0c0 || r0c1 || r1c0 || r1c1) {
        hid_type_string("WARN KEY STUCK AT BOOT\n");
    } else {
        hid_type_string("SCAN OK no key at boot\n");
    }

    hid_type_string("WAIT KEY...\n");
    HAL_Delay(300);

    /* ============================================================
     *  VÒNG LẶP CHÍNH
     * ============================================================ */
    uint32_t last_scan_tick  = 0;
    uint32_t last_heartbeat  = 0;
    uint8_t  prev_deb[2][2]  = {0};
    const char *key_names[2][2] = {{"A","B"},{"C","D"}};

    while (1) {
        uint32_t now = HAL_GetTick();

        if ((now - last_scan_tick) >= 1) {
            last_scan_tick = now;
            matrix_scan();

            /* ---- DEBUG RAW: Kiểm tra tín hiệu thô TRƯỚC debounce ----------
             * Nếu bấm SW[0,0] mà màn hình hiện "R00" → chip đọc được phím,
             * lỗi nằm ở debounce hoặc USB.
             * Nếu KHÔNG hiện gì → lỗi ở phần cứng hoặc matrix_scan.
             * Xóa đoạn này sau khi debug xong. */
            static uint8_t raw_prev_dbg[2][2] = {0};
            for (int _r = 0; _r < 2; _r++) {
                for (int _c = 0; _c < 2; _c++) {
                    if (matrix_raw[_r][_c] != raw_prev_dbg[_r][_c]) {
                        raw_prev_dbg[_r][_c] = matrix_raw[_r][_c];
                        if (matrix_raw[_r][_c]) {
                            hid_type_string("R");
                            hid_type_int(_r);
                            hid_type_int(_c);
                            hid_type_char(' ');
                        }
                    }
                }
            }
            /* ---- KẾT THÚC DEBUG RAW ---------------------------------------- */

            debounce_process();
            keyboard_process();

            /* Phát hiện thay đổi phím → gõ ra màn hình */
            for (int r = 0; r < 2; r++) {
                for (int c = 0; c < 2; c++) {
                    if (debounced[r][c] != prev_deb[r][c]) {
                        prev_deb[r][c] = debounced[r][c];
                        if (debounced[r][c]) {
                            /* PRESS */
                            hid_type_string("KEY[");
                            hid_type_int(r);
                            hid_type_char(',');
                            hid_type_int(c);
                            hid_type_string("]=");
                            hid_type_string(key_names[r][c]);
                            hid_type_char('\n');
                        } else {
                            /* RELEASE */
                            hid_type_string("REL[");
                            hid_type_int(r);
                            hid_type_char(',');
                            hid_type_int(c);
                            hid_type_string("]\n");
                        }
                    }
                }
            }
        }

        /* Heartbeat mỗi 5 giây — dấu chấm để biết MCU còn sống */
        if ((now - last_heartbeat) >= 5000) {
            last_heartbeat = now;
            hid_type_char('.');
        }
    }
}

/* ============================================================
 *  SystemClock_Config — giữ nguyên từ file gốc
 * ============================================================ */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    RCC_OscInitStruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState        = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue  = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState        = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState    = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL      = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) { while(1); }

    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                      |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) { while(1); }

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
    PeriphClkInit.UsbClockSelection    = RCC_USBCLKSOURCE_PLL_DIV1_5;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) { while(1); }
}

void Error_Handler(void) { __disable_irq(); while(1); }
