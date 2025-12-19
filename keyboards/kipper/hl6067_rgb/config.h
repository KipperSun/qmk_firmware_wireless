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
#   define SERIAL_NUMBER "mjl_hl6095"
#endif

/*******************************************************matrix********************************************************/
/* key matrix size */
#define MATRIX_ROWS 5
#define MATRIX_COLS 15
/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION COL2ROW

/* key matrix pins */
#define MATRIX_ROW_PINS { A8, A9, A10, B10, B9 }
#define MATRIX_COL_PINS { A3, A2, A1, A7, B0, B15, B8, B14, B13, B12, B1, B3, A15, A14, A13 }

/*******************************************************matrix********************************************************/

#ifdef BLUETOOTH_BHQ
// Its active level is "BHQ_IRQ_AND_INT_LEVEL of bhq.h "
#   define BHQ_INT_PIN          B4
#   define BHQ_IQR_PIN          B5

#   define USB_POWER_SENSE_PIN  A5             // USB插入检测引脚
#   define USB_POWER_CONNECTED_LEVEL   1

#   define UART_DRIVER          SD1
#   define UART_TX_PIN          B6
#   define UART_TX_PAL_MODE     7
#   define UART_RX_PIN          B7
#   define UART_RX_PAL_MODE	    7

// STM32使用到的高速晶振引脚号，做低功耗需要用户配置，每款芯片有可能不一样的
#define LPM_STM32_HSE_PIN_IN     H0
#define LPM_STM32_HSE_PIN_OUT    H1

#define REPORT_BUFFER_QUEUE_SIZE    68
#define BATTER_ADC_PIN              A6
#define BATTER_ADC_DRIVER           ADCD1

#endif

#define WS2812_POWER_PIN    A4
#define WS2812_BYTE_ORDER   WS2812_BYTE_ORDER_GRB
#define WS2812_POWER_ON_LEVEL   1



#    define WS2812_PWM_DRIVER PWMD5  // default: PWMD2
#    define WS2812_PWM_CHANNEL 1  // default: 2
#    define WS2812_PWM_PAL_MODE 2  // Pin "alternate function", see the respective datasheet for the appropriate values for your MCU. default: 2
#    define WS2812_DMA_STREAM STM32_DMA1_STREAM6  // DMA Stream for TIMx_UP, see the respective reference manual for the appropriate values for your MCU.
#    define WS2812_DMA_CHANNEL 6  // DMA Channel for TIMx_UP, see the respective reference manual for the appropriate values for your MCU.
