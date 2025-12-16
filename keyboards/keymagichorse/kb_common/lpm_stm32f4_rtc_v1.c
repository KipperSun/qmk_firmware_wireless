/* Copyright 2024 keymagichorse
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
// Very few codes are borrowed from https://www.keychron.com
#include "quantum.h"
#include "lpm.h"
#include "matrix.h"
#include "gpio.h"
#include "debounce.h"
#include "usb_util.h"
#include <usb_main.h>
#include "bhq.h"
#include "report_buffer.h"
#include "uart.h"
#include "bhq_common.h"

# if defined(KB_CHECK_BATTERY_ENABLED)
#   include "battery.h"
#endif

#if SHIFT595_ENABLED
#   include "74hc595.h"
#endif

static uint32_t     lpm_timer_buffer = 0;
static bool         lpm_time_up               = false;

// use for config wakeUp Pin
static const pin_t wakeUpRow_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;
static const pin_t wakeUpCol_pins[MATRIX_COLS]   = MATRIX_COL_PINS;

static inline uint32_t rtc_wakeup_calc(uint32_t ms) {
    uint32_t wutr = 0;

    if (ms >= 1000) {
        // 用 1Hz 时钟，单位 = 秒
        uint32_t sec = (ms + 999) / 1000;  // 四舍五入
        wutr = ((uint32_t)4 << 16) | (sec - 1);
    } else {
        // 用 RTC/16 (~2.048 kHz)，单位 ≈ 0.488ms
        uint32_t ticks = (ms * 2048 + 999) / 1000; // 四舍五入成 tick
        if (ticks == 0) ticks = 1;
        wutr = ((uint32_t)0 << 16) | (ticks - 1);
    }

    return wutr;
}

void ws2812power_enabled(void);
void ws2812power_Disabled(void);

void lpm_timer_reset(void) {
    lpm_time_up      = false;
    lpm_timer_buffer = 0;
}

__attribute__((weak)) void lpm_device_power_open(void) ;
__attribute__((weak)) void lpm_device_power_close(void) ;

void lpm_init(void)
{
    // 禁用调试功能以降低功耗
    DBGMCU->CR &= ~DBGMCU_CR_DBG_SLEEP;   // 禁用在Sleep模式下的调试
    DBGMCU->CR &= ~DBGMCU_CR_DBG_STOP;    // 禁用在Stop模式下的调试
    DBGMCU->CR &= ~DBGMCU_CR_DBG_STANDBY; // 禁用在Standby模式下的调试

    lpm_timer_reset();

    gpio_write_pin_high(BHQ_INT_PIN);

    // usb
    gpio_set_pin_input(USB_POWER_SENSE_PIN);
    palEnableLineEvent(USB_POWER_SENSE_PIN, PAL_EVENT_MODE_RISING_EDGE);

    lpm_device_power_open();
}
__attribute__((weak)) void lpm_device_power_open(void) 
{
   
}
__attribute__((weak)) void lpm_device_power_close(void) 
{
   
}

// 将未使用的引脚设置为输入模拟
__attribute__((weak)) void lpm_set_unused_pins_to_input_analog(void)
{

}

void My_PWR_EnterSTOPMode(void)
{
#if STM32_HSE_ENABLED
    /* Switch to HSI */
    RCC->CFGR = (RCC->CFGR & (~STM32_SW_MASK)) | STM32_SW_HSI;
    while ((RCC->CFGR & RCC_CFGR_SWS) != (STM32_SW_HSI << 2));

    /* Set HSE off  */
    RCC->CR &= ~RCC_CR_HSEON;
    while ((RCC->CR & RCC_CR_HSERDY));

    palSetLineMode(LPM_STM32_HSE_PIN_IN, PAL_MODE_INPUT_ANALOG); 
    palSetLineMode(LPM_STM32_HSE_PIN_OUT, PAL_MODE_INPUT_ANALOG); 
#endif
    /* Wake source: Reset pin, all I/Os, BOR, PVD, PVM, RTC, LCD, IWDG,
    COMPx, USARTx, LPUART1, I2Cx, LPTIMx, USB, SWPMI */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    PWR->CR |=
        PWR_CR_MRLVDS |
        PWR_CR_LPLVDS |
        PWR_CR_FPDS |
        PWR_CR_LPDS |
        0;
    __WFI();

    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
}

void enter_low_power_mode_prepare(void)
{
    if (usb_power_connected()) 
    {
       return;
    }
#if SHIFT595_ENABLED
    shift595_write_all(0x00);
    shift595_pin_sleep();
#endif
    lpm_set_unused_pins_to_input_analog();    // 设置没有使用的引脚为模拟输入
#if SHIFT595_ENABLED
    shift595_pin_sleep();
#endif
# if defined(KB_CHECK_BATTERY_ENABLED)
    battery_stop();
#endif

    uint8_t i = 0;
#if (DIODE_DIRECTION == COL2ROW)
    // Set row(low valid), read cols
    for (i = 0; i < matrix_cols(); i++)
    { // set col pull-up input
        if(wakeUpCol_pins[i] == NO_PIN)
        {
            continue;
        } 
        ATOMIC_BLOCK_FORCEON {
            gpio_set_pin_input_high(wakeUpCol_pins[i]);
            palEnableLineEvent(wakeUpCol_pins[i], PAL_EVENT_MODE_RISING_EDGE);
        }
    }
    for (i = 0; i < matrix_rows(); i++)
    { // set row output low level
        if(wakeUpRow_pins[i] == NO_PIN)
        {
            continue;
        } 
        ATOMIC_BLOCK_FORCEON {
            gpio_set_pin_output(wakeUpRow_pins[i]);
            gpio_write_pin_low(wakeUpRow_pins[i]);
        }
    }
#elif (DIODE_DIRECTION == ROW2COL)

    // Set col(low valid), read rows
    for (i = 0; i < matrix_rows(); i++)
    { // set row pull-up input
        if(wakeUpRow_pins[i] == NO_PIN)
        {
            continue;
        } 
        ATOMIC_BLOCK_FORCEON {
            gpio_set_pin_input_high(wakeUpRow_pins[i]);
            palEnableLineEvent(wakeUpRow_pins[i], PAL_EVENT_MODE_RISING_EDGE);
        }
    }

    for (i = 0; i < matrix_cols(); i++)
    { // set col output low level
        if(wakeUpCol_pins[i] == NO_PIN)
        {
            continue;
        } 
        ATOMIC_BLOCK_FORCEON {
            gpio_set_pin_output(wakeUpCol_pins[i]);
            gpio_write_pin_low(wakeUpCol_pins[i]);
        }
    }

#endif

// rtc唤醒
    RTCWakeup wakeupspec;
    wakeupspec.wutr = rtc_wakeup_calc(500);
    rtcSTM32SetPeriodicWakeup(&RTCD1, &wakeupspec);
    rtcSetCallback(&RTCD1, NULL);
// rtc唤醒

    gpio_set_pin_input_low(BHQ_IQR_PIN);
    palEnableLineEvent(BHQ_IQR_PIN, PAL_EVENT_MODE_RISING_EDGE);
    gpio_write_pin_low(BHQ_INT_PIN);

// usb 插入检测
    gpio_set_pin_input(USB_POWER_SENSE_PIN);
    palEnableLineEvent(USB_POWER_SENSE_PIN, PAL_EVENT_MODE_RISING_EDGE);

    /* Usb unit is actived and running, stop and disconnect first */
    sdStop(&UART_DRIVER);
    palSetLineMode(UART_TX_PIN, PAL_MODE_INPUT_ANALOG);
    palSetLineMode(UART_RX_PIN, PAL_MODE_INPUT_ANALOG);

    usbStop(&USBD1);
    usbDisconnectBus(&USBD1);

    bhq_Disable();
    lpm_device_power_close();    // 外围设备 电源 关闭
    My_PWR_EnterSTOPMode();

}

void exit_low_power_mode_prepare(void)
{
    chSysLock();
        stm32_clock_init();
        halInit();
        stInit();
        timer_init();
    chSysUnlock();

    /*  USB D+/D- */
    palSetLineMode(A11, PAL_STM32_OTYPE_PUSHPULL | PAL_STM32_OSPEED_HIGHEST | PAL_STM32_PUPDR_FLOATING | PAL_MODE_ALTERNATE(10U));
    palSetLineMode(A12, PAL_STM32_OTYPE_PUSHPULL | PAL_STM32_OSPEED_HIGHEST | PAL_STM32_PUPDR_FLOATING | PAL_MODE_ALTERNATE(10U));
 
    /* Call debounce_free() to avoiding memory leak of debounce_counters as debounce_init()
    invoked in matrix_init() alloc new memory to debounce_counters */
    debounce_free();
    matrix_init();

    lpm_timer_reset();
    report_buffer_init();
    bhq_init();     // uart_init
#if defined (MOUSEKEY_ENABLE)
    mousekey_clear();
#endif
    // clear_keyboard();
    // layer_clear();
# if defined(KB_CHECK_BATTERY_ENABLED)
    battery_start();
#endif
    lpm_device_power_open();    // 外围设备 电源 关闭
  
    gpio_write_pin_high(BHQ_INT_PIN);
}

bool lowpower_matrix_task(void) 
{
    matrix_scan(); matrix_scan(); matrix_scan(); 
    bool any_key_pressed = false; 
    for (uint8_t row = 0; row < MATRIX_ROWS && !any_key_pressed; row++) 
    { 
        matrix_row_t current_row = matrix_get_row(row); 
        if (current_row) 
        { 
            // 只要某一行有非零，说明有键按下 
            any_key_pressed = true;
        }
    } 
    return any_key_pressed; 
}

void lmp_halInit(void);
void lpm_task(void)
{
    if (usb_power_connected() && USBD1.state == USB_STOP) {
        /*  USB D+/D- */
        palSetLineMode(A11, PAL_STM32_OTYPE_PUSHPULL | PAL_STM32_OSPEED_HIGHEST | PAL_STM32_PUPDR_FLOATING | PAL_MODE_ALTERNATE(10U));
        palSetLineMode(A12, PAL_STM32_OTYPE_PUSHPULL | PAL_STM32_OSPEED_HIGHEST | PAL_STM32_PUPDR_FLOATING | PAL_MODE_ALTERNATE(10U));
        usb_event_queue_init();
        init_usb_driver(&USBD1);
    }
    
    if (usb_power_connected()) 
    {
       return;
    }

    if(report_buffer_is_empty() == false)
    {
        lpm_time_up = false;
        lpm_timer_buffer = 0;
        return;
    }

    if(lpm_time_up == false && lpm_timer_buffer == 0)
    {
        lpm_time_up = true;
        lpm_timer_buffer = sync_timer_read32();
    }

    if (lpm_time_up == true && sync_timer_elapsed32(lpm_timer_buffer) > RUN_MODE_PROCESS_TIME) {
        lpm_time_up = false;
        lpm_timer_buffer = 0;
        enter_low_power_mode_prepare();
// rtc唤醒逻辑 start 
        uint8_t temp_cut = 0;
        uint8_t wireless_init_flag = 0;
        if(wireless_get() == WT_STATE_CONNECTED)
        {
            // 应该是要判断，串口有没有数据来
            // 并且要扫描矩阵
            while(1)
            {
                // usb插入时,直接唤醒
                if(usb_power_connected())
                {
                    exit_low_power_mode_prepare();
                    return;
                }
                if(wireless_init_flag == 0)
                {
                    wireless_init_flag = 1;
                    chSysLock();
                        stm32_clock_init();
                        lmp_halInit();
                        timer_init();
                    chSysUnlock();
                    bhq_init();
                }
                temp_cut++;
                if(gpio_read_pin(BHQ_IQR_PIN) == BHQ_RUN_OR_INT_LEVEL)
                {
                    if(temp_cut <= 2)
                    {
                        gpio_write_pin_high(BHQ_INT_PIN);
                    }
                    else
                    {
                        gpio_write_pin_low(BHQ_INT_PIN);
                    }
                }
                else
                {
                    gpio_write_pin_low(BHQ_INT_PIN);
                }

                if (gpio_read_pin(BHQ_IQR_PIN) == BHQ_RUN_OR_INT_LEVEL && bhq_available() == true)
                {
                    break;
                }
                if(lowpower_matrix_task())
                {
                    break;
                }
                if(temp_cut >= 50)
                {
                    temp_cut = 0;
                    wireless_init_flag = 0;
                    enter_low_power_mode_prepare();
                }
                wait_ms(1);  
            }
        }
        else    // 无线为未连接时
        {
            while(1)
            {
                // usb插入时,直接唤醒
                if(usb_power_connected())
                {
                    exit_low_power_mode_prepare();
                    return;
                }
                if(lowpower_matrix_task())
                {
                    break;
                }
                else
                {
                    temp_cut++;
                    if(temp_cut >= 5)
                    {
                        temp_cut = 0;
                        enter_low_power_mode_prepare();
                    }
                }
                wait_ms(1);
            }
        }
// rtc唤醒逻辑 end 
        exit_low_power_mode_prepare();
    }
}

/**
 * @brief   HAL initialization.
 * @details This function invokes the low level initialization code then
 *          initializes all the drivers enabled in the HAL. Finally the
 *          board-specific initialization is performed by invoking
 *          @p boardInit() (usually defined in @p board.c).
 *
 * @init
 */
void lmp_halInit(void) {

  /* Initializes the OS Abstraction Layer.*/
  osalInit();

  /* Platform low level initializations.*/
  hal_lld_init();

#if (HAL_USE_PAL == TRUE) || defined(__DOXYGEN__)
#if defined(PAL_NEW_INIT)
  palInit();
#else
  palInit(&pal_default_config);
#endif
#endif
#if (HAL_USE_ADC == TRUE) || defined(__DOXYGEN__)
  adcInit();
#endif
#if (HAL_USE_CAN == TRUE) || defined(__DOXYGEN__)
  canInit();
#endif
#if (HAL_USE_CRY == TRUE) || defined(__DOXYGEN__)
  cryInit();
#endif
#if (HAL_USE_DAC == TRUE) || defined(__DOXYGEN__)
  dacInit();
#endif
#if (HAL_USE_EFL == TRUE) || defined(__DOXYGEN__)
  eflInit();
#endif
#if (HAL_USE_GPT == TRUE) || defined(__DOXYGEN__)
  gptInit();
#endif
#if (HAL_USE_I2C == TRUE) || defined(__DOXYGEN__)
  i2cInit();
#endif
#if (HAL_USE_I2S == TRUE) || defined(__DOXYGEN__)
  i2sInit();
#endif
#if (HAL_USE_ICU == TRUE) || defined(__DOXYGEN__)
  icuInit();
#endif
#if (HAL_USE_MAC == TRUE) || defined(__DOXYGEN__)
  macInit();
#endif
#if (HAL_USE_PWM == TRUE) || defined(__DOXYGEN__)
  pwmInit();
#endif
#if (HAL_USE_SERIAL == TRUE) || defined(__DOXYGEN__)
  sdInit();
#endif
#if (HAL_USE_SDC == TRUE) || defined(__DOXYGEN__)
  sdcInit();
#endif
#if (HAL_USE_SIO == TRUE) || defined(__DOXYGEN__)
  sioInit();
#endif
#if (HAL_USE_SPI == TRUE) || defined(__DOXYGEN__)
  spiInit();
#endif
#if (HAL_USE_TRNG == TRUE) || defined(__DOXYGEN__)
  trngInit();
#endif
#if (HAL_USE_UART == TRUE) || defined(__DOXYGEN__)
  uartInit();
#endif
#if (HAL_USE_USB == TRUE) || defined(__DOXYGEN__)
  usbInit();
#endif
#if (HAL_USE_MMC_SPI == TRUE) || defined(__DOXYGEN__)
  mmcInit();
#endif
#if (HAL_USE_SERIAL_USB == TRUE) || defined(__DOXYGEN__)
  sduInit();
#endif
#if (HAL_USE_RTC == TRUE) || defined(__DOXYGEN__)
  rtcInit();
#endif
#if (HAL_USE_WDG == TRUE) || defined(__DOXYGEN__)
  wdgInit();
#endif
#if (HAL_USE_WSPI == TRUE) || defined(__DOXYGEN__)
  wspiInit();
#endif

  /* Community driver overlay initialization.*/
#if defined(HAL_USE_COMMUNITY) || defined(__DOXYGEN__)
#if (HAL_USE_COMMUNITY == TRUE) || defined(__DOXYGEN__)
  halCommunityInit();
#endif
#endif

  /* Board specific initialization.*/
//   boardInit();

/*
 *  The ST driver is a special case, it is only initialized if the OSAL is
 *  configured to require it.
 */
#if OSAL_ST_MODE != OSAL_ST_MODE_NONE
  stInit();
#endif
}
