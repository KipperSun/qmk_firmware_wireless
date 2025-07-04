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
#pragma once


#ifndef SERIAL_NUMBER
#   define SERIAL_NUMBER "mjl_juicy65"
#endif

#ifdef BLUETOOTH_BHQ
// Its active level is "BHQ_IRQ_AND_INT_LEVEL of bhq.h " 
#   define BHQ_IQR_PIN          A1             
#   define BHQ_INT_PIN          A0             
#   define USB_POWER_SENSE_PIN  A10             // USB插入检测引脚  

#   define UART_DRIVER          SD2
#   define UART_TX_PIN          A2
#   define UART_TX_PAL_MODE     7
#   define UART_RX_PIN          A3
#   define UART_RX_PAL_MODE	    7

// STM32使用到的高速晶振引脚号，做低功耗需要用户配置，每款芯片有可能不一样的
#define LPM_STM32_HSE_PIN_IN     H1  
#define LPM_STM32_HSE_PIN_OUT    H0

#define REPORT_BUFFER_QUEUE_SIZE    68
#define BATTER_ADC_PIN              B1
#define BATTER_ADC_DRIVER           ADCD1

#define BLE_TOG     QK_USER_0    // 切换蓝牙输出 并 开启蓝牙广播（非配对类型）
#define RF_TOG      QK_USER_1    // 切换 2.4ghz输出
#define USB_TOG     QK_USER_2    // 打开USB
#define BL_SW_0     QK_USER_3    // 开启蓝牙通道0（需要打开蓝牙的条件下才行） 短按打开广播 长按开启配对广播
#define BL_SW_1     QK_USER_4    // 开启蓝牙通道1（需要打开蓝牙的条件下才行） 短按打开广播 长按开启配对广播
#define BL_SW_2     QK_USER_5    // 开启蓝牙通道2（需要打开蓝牙的条件下才行） 短按打开广播 长按开启配对广播
#define BLE_RESET   QK_USER_6    // 蓝牙复位
#define BLE_OFF     QK_USER_7    // 关闭蓝牙连接

#endif

#define WS2812_POWER_PIN    B7
#define WS2812_BYTE_ORDER   WS2812_BYTE_ORDER_GRB

// #define RGBLIGHT_LAYERS
// #define RGBLIGHT_LAYER_BLINK
// #define RGBLIGHT_LAYERS_OVERRIDE_RGB_OFF
// #define RGBLIGHT_LAYERS_RETAIN_VAL

/*************************matrix**************************/
/* key matrix size */
#define MATRIX_ROWS 5
#define MATRIX_COLS 15
/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION ROW2COL

/* key matrix pins */
#define MATRIX_ROW_PINS { B15, B14, B13, B12, A8 }
                        //  Q7      Q6      Q5     Q4
#define MATRIX_COL_PINS { NO_PIN, NO_PIN, NO_PIN, NO_PIN, NO_PIN, NO_PIN, NO_PIN, NO_PIN, NO_PIN, NO_PIN, NO_PIN, NO_PIN, NO_PIN, NO_PIN, NO_PIN}
// 列 接到 74HC595 的哪个 Qx
#define COL_TO_74HC595_PINS  {_2_595_Q1, _2_595_Q2, _2_595_Q3, _2_595_Q4, _2_595_Q5, _2_595_Q6, _2_595_Q7, _1_595_Q0, _1_595_Q2, _1_595_Q3, _1_595_Q1, _1_595_Q4, _1_595_Q5, _1_595_Q6, _1_595_Q7}
// 定义列映射到 74HC595 的管脚数量
#define COL_TO_74HC595_PINS_COUNT   15
// 第几个列开始接到595
#define COL_TO_74HC595_START_INDEX  0
// 到第几个列结束到595
#define COL_TO_74HC595_END_INDEX    14
#define I_595_NUM   2       // 用了多少个595
/* Pin connected to DS of 74HC595 */
#define DS_PIN_74HC595      A4
/* Pin connected to SH_CP of 74HC595 */
#define SHCP_PIN_74HC595    A6
/* Pin connected to ST_CP of 74HC595 */
#define STCP_PIN_74HC595    A5



#    define WS2812_PWM_DRIVER PWMD3  // default: PWMD2
#    define WS2812_PWM_CHANNEL 2  // default: 2
#    define WS2812_PWM_PAL_MODE 2  // Pin "alternate function", see the respective datasheet for the appropriate values for your MCU. default: 2
#    define WS2812_DMA_STREAM STM32_DMA1_STREAM2  // DMA Stream for TIMx_UP, see the respective reference manual for the appropriate values for your MCU.
#    define WS2812_DMA_CHANNEL 5  // DMA Channel for TIMx_UP, see the respective reference manual for the appropriate values for your MCU.