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
#define MATRIX_COLS 16  // 做两列假灯
/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION ROW2COL

/* key matrix pins */
#define MATRIX_ROW_PINS { A15, B3, B4, B5, B6 }
                        //  Q7      Q6      Q5     Q4
#define MATRIX_COL_PINS { NO_PIN, NO_PIN, NO_PIN, NO_PIN, NO_PIN, NO_PIN, NO_PIN, NO_PIN, NO_PIN, NO_PIN, NO_PIN, NO_PIN, NO_PIN, NO_PIN, NO_PIN, NO_PIN}
// 列 接到 74HC595 的哪个 Qx
#define COL_TO_74HC595_PINS  {_1_595_Q1, _1_595_Q2, _1_595_Q3, _1_595_Q4, _1_595_Q5, _1_595_Q6, _1_595_Q7, _1_595_Q0, _2_595_Q7, _2_595_Q6, _2_595_Q5, _2_595_Q4, _2_595_Q2, _2_595_Q3}
// 定义列映射到 74HC595 的管脚数量
#define COL_TO_74HC595_PINS_COUNT   14
// 第几个列开始接到595
#define COL_TO_74HC595_START_INDEX  0
// 到第几个列结束到595
#define COL_TO_74HC595_END_INDEX    13
#define I_595_NUM   2       // 用了多少个595
/* Pin connected to DS of 74HC595 */
#define DS_PIN_74HC595      B9
/* Pin connected to SH_CP of 74HC595 */
#define SHCP_PIN_74HC595    B7
/* Pin connected to ST_CP of 74HC595 */
#define STCP_PIN_74HC595    B8
/*******************************************************matrix********************************************************/

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

#endif

#define WS2812_POWER_PIN    B0
#define WS2812_BYTE_ORDER   WS2812_BYTE_ORDER_GRB
