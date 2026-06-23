#include "main.h"
#include "usb_device.h"
#include "matrix.h"
#include "debounce.h"
#include "keyboard.h"
#include "usb_hid.h"
#include "usbd_hid.h"
#include <string.h>

extern USBD_HandleTypeDef hUsbDeviceFS;
UART_HandleTypeDef huart1;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    MX_USB_DEVICE_Init();
    matrix_init();
    debounce_init();
    keyboard_init();
    usb_hid_init();

    /* Chờ USB enumerate xong */
    HAL_Delay(1500);

    uint32_t last_tick = 0;

    while (1) {
        uint32_t now = HAL_GetTick();

        /* Chạy mỗi 1ms */
        if (now - last_tick >= 1) {
            last_tick = now;
            matrix_scan();
            debounce_process();
            keyboard_task();
        }
    }
}

void SystemClock_Config(void) {
    RCC_OscInitTypeDef o = {0};
    RCC_ClkInitTypeDef c = {0};
    RCC_PeriphCLKInitTypeDef p = {0};
    o.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
    o.HSEState        = RCC_HSE_ON;
    o.HSEPredivValue  = RCC_HSE_PREDIV_DIV1;
    o.HSIState        = RCC_HSI_ON;
    o.PLL.PLLState    = RCC_PLL_ON;
    o.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
    o.PLL.PLLMUL      = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&o) != HAL_OK) {
        o.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
        o.HSEState            = RCC_HSE_OFF;
        o.HSIState            = RCC_HSI_ON;
        o.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
        o.PLL.PLLState        = RCC_PLL_ON;
        o.PLL.PLLSource       = RCC_PLLSOURCE_HSI_DIV2;
        o.PLL.PLLMUL          = RCC_PLL_MUL12;
        if (HAL_RCC_OscConfig(&o) != HAL_OK) { Error_Handler(); }
        p.PeriphClockSelection = RCC_PERIPHCLK_USB;
        p.UsbClockSelection    = RCC_USBCLKSOURCE_PLL;
        if (HAL_RCCEx_PeriphCLKConfig(&p) != HAL_OK) { Error_Handler(); }
        c.ClockType      = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                          |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
        c.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
        c.AHBCLKDivider  = RCC_SYSCLK_DIV1;
        c.APB1CLKDivider = RCC_HCLK_DIV2;
        c.APB2CLKDivider = RCC_HCLK_DIV1;
        if (HAL_RCC_ClockConfig(&c, FLASH_LATENCY_1) != HAL_OK) { Error_Handler(); }
        return;
    }
    c.ClockType      = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                      |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    c.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    c.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    c.APB1CLKDivider = RCC_HCLK_DIV2;
    c.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&c, FLASH_LATENCY_2) != HAL_OK) { Error_Handler(); }
    p.PeriphClockSelection = RCC_PERIPHCLK_USB;
    p.UsbClockSelection    = RCC_USBCLKSOURCE_PLL_DIV1_5;
    if (HAL_RCCEx_PeriphCLKConfig(&p) != HAL_OK) { Error_Handler(); }
}

static void MX_USART1_UART_Init(void) {
    huart1.Instance          = USART1;
    huart1.Init.BaudRate     = 115200;
    huart1.Init.WordLength   = UART_WORDLENGTH_8B;
    huart1.Init.StopBits     = UART_STOPBITS_1;
    huart1.Init.Parity       = UART_PARITY_NONE;
    huart1.Init.Mode         = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK) { Error_Handler(); }
}

static void MX_GPIO_Init(void) {
    GPIO_InitTypeDef g = {0};
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* ROW — PA0–PA4, OUTPUT Push-Pull, default HIGH (inactive) */
    HAL_GPIO_WritePin(GPIOA,
        GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4,
        GPIO_PIN_SET);
    g.Pin   = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4;
    g.Mode  = GPIO_MODE_OUTPUT_PP;
    g.Pull  = GPIO_NOPULL;
    g.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &g);

    /* COL — chỉ các chân thực sự dùng, INPUT Pull-Up
     * PB0,1,3,4,5,6,7,8,9,10,11,12,13,14,15 (không có PB2) */
    g.Pin  = GPIO_PIN_0 |GPIO_PIN_1 |GPIO_PIN_3 |GPIO_PIN_4
            |GPIO_PIN_5 |GPIO_PIN_6 |GPIO_PIN_7 |GPIO_PIN_8
            |GPIO_PIN_9 |GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12
            |GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    g.Mode = GPIO_MODE_INPUT;
    g.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &g);
}

void Error_Handler(void) { __disable_irq(); while (1) {} }
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) {}
#endif
