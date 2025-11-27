/* Copyright 2021 @ Keychron (https://www.keychron.com)
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

#include "matrix.h"
#include <string.h>
#include "atomic_util.h"
#include "config.h"
#include "wait.h"
#include "analog.h"
#include "km_printf.h"
#include "timer.h"
#include "ec_filter.h"

#define ECOUT_ADC_PIN   A1
#define DISCHARGE_PIN   A3

#define MUX1_EN     C13
#define MUX_SET0    C14
#define MUX_SET1    C15
#define MUX_SET2    A0

uint32_t matrix_debug_timer = 0;
// A1   ECOUT_ADC  
// A3   DISCHARGE_PIN
// C13  MUX1_EN
// C14  MUX_SET0
// C15  MUX_SET1
// A0   MUX_SET2
#define MUX_SEL_PINS {C14, C15, A0}



#ifdef MATRIX_ROW_PINS
static const pin_t row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;
#endif // MATRIX_ROW_PINS
// #ifdef MATRIX_COL_PINS
// static const pin_t col_pins[MATRIX_COLS] = MATRIX_COL_PINS;
// #endif // MATRIX_COL_PINS
struct ec_key_t {
    uint16_t adc_min;
    uint16_t adc_max;
    uint16_t adc_diff;  // 最大-最小
    uint16_t diff_idx;  // 缩放后的ad值
    uint16_t adc_value;
} ec_keys[MATRIX_ROWS][MATRIX_COLS];


ec_filter_RingBuffer ec_adc_buffs[MATRIX_ROWS][MATRIX_COLS];

static const pin_t mux_sel_pins[3] = MUX_SEL_PINS;



// 放电
static inline void discharge_capacitor(void) {
    setPinOutput(DISCHARGE_PIN);
    writePinLow(DISCHARGE_PIN);
}
// 充电
static inline void charge_capacitor(uint8_t row) {
    setPinInput(DISCHARGE_PIN); // Z state
    // charge select row
    writePinHigh(row_pins[row]);
}
// 行拉低
static inline void clear_row_pin(uint8_t row) {
    writePinLow(row_pins[row]);
}
#define MUX_CH0 0  // S2=0, S1=0, S0=0
#define MUX_CH1 1  // S2=0, S1=0, S0=1
#define MUX_CH2 2  // S2=0, S1=1, S0=0
#define MUX_CH3 3  // S2=0, S1=1, S0=1
#define MUX_CH4 4  // S2=1, S1=0, S0=0
#define MUX_CH5 5  // S2=1, S1=0, S0=1
#define MUX_CH6 6  // S2=1, S1=1, S0=0
#define MUX_CH7 7  // S2=1, S1=1, S0=1

static const uint8_t col_channels[MATRIX_COLS] = {MUX_CH3, MUX_CH0, MUX_CH1, MUX_CH2, MUX_CH6};
// 选择mux
static inline void select_mux(uint8_t col) {
    uint8_t ch = col_channels[col];
    writePin(mux_sel_pins[0], ch & 1);
    writePin(mux_sel_pins[1], ch & 2);
    writePin(mux_sel_pins[2], ch & 4);
}

// 读取按键电容值
static uint16_t ecsm_readkey_raw(uint8_t row, uint8_t col) {
    uint16_t sw_value = 0;

    charge_capacitor(row); // 拉高这一行, 给这一行的电容充电
    // 理论上这里要等下等待充电完成
    wait_us(5);

// 获取滤波后ad值
    ec_filter_alpha_beta(&ec_adc_buffs[row][col],analogReadPin(ECOUT_ADC_PIN));
    sw_value = ec_filter_get_avg_value(&ec_adc_buffs[row][col]);
    // sw_value = analogReadPin(ECOUT_ADC_PIN);
// 获取滤波后ad值

    discharge_capacitor();
    clear_row_pin(row);

    wait_us(5); // 5*1nf*1k = 5us
    return sw_value;
}

// Update press/release state of key at (row, col)
static bool ecsm_update_key(matrix_row_t* current_row, uint8_t row, uint8_t col) {
    bool current_state = (*current_row >> col) & 1;


    uint32_t diff_idx = 0;
    uint32_t scal_10 = 2048;

    if (ec_keys[row][col].adc_max > ec_keys[row][col].adc_min + 1000) {
        scal_10 = (2048<<11) / (ec_keys[row][col].adc_max - ec_keys[row][col].adc_min); // avoid float
    } else {
        scal_10 = 2048;
    }

    diff_idx = (scal_10 * (ec_keys[row][col].adc_value - ec_keys[row][col].adc_min))>>11;

    if (diff_idx > 2049 - 1) {
        diff_idx = 2048;
    }
    ec_keys[row][col].diff_idx = diff_idx;

    bool change = false;

    // press to release
    if (current_state && diff_idx < 600) {
        *current_row &= ~(1 << col);
        change = true;
    }

    // 中间带不管

    // release to press
    if ((!current_state) && diff_idx > 1024) {
        *current_row |= (1 << col);
        change = true;
    }

    if (change) {
        km_printf("%d,%d: %ld\n", row, col, diff_idx);
    }

    return change;
}



static bool ecsm_matrix_scan(matrix_row_t current_matrix[]) {
    bool updated = false;
    discharge_capacitor();
    wait_us(20);
    for (int col = 0; col < MATRIX_COLS; col++) {
        select_mux(col);
        writePinLow(MUX1_EN);
        for (int row = 0; row < MATRIX_ROWS; row++) {
            ec_keys[row][col].adc_value = ecsm_readkey_raw(row, col);
            ec_keys[row][col].adc_min = MIN(ec_keys[row][col].adc_min,  ec_keys[row][col].adc_value);
            ec_keys[row][col].adc_max = MAX(ec_keys[row][col].adc_max,  ec_keys[row][col].adc_value);
            ec_keys[row][col].adc_diff = ec_keys[row][col].adc_max - ec_keys[row][col].adc_min;
            updated |= ecsm_update_key(&current_matrix[row], row, col);
        }
        writePinHigh(MUX1_EN);
    }
    return updated;

}



bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    if (timer_elapsed32(matrix_debug_timer) > 500)     // 1分钟
    {
        matrix_debug_timer = timer_read32();
        km_printf("rowxx ");
        for (uint8_t coli = 0; coli < MATRIX_COLS; coli++)
        {
            km_printf("col%02d\t",coli);
        }
        km_printf("\r\n");

        for (uint8_t row = 0; row < MATRIX_ROWS; row++)
        {
            km_printf("row%02d:",row);
            for (uint8_t col = 0; col < MATRIX_COLS; col++)
            {
                km_printf("\t%04d:%04d:%04d:%04d |", ec_keys[row][col].adc_max, ec_keys[row][col].adc_min, ec_keys[row][col].adc_diff, ec_keys[row][col].adc_value);
            }
            km_printf("\r\n");
        }
        km_printf("\r\n");
    }


    uint8_t isUpdate = ecsm_matrix_scan(current_matrix);
    
    if(isUpdate)
    {
        km_printf("matrix update\n");
    }

    return isUpdate;
}




static void matrix_init_pins(void) {

    for (int i = 0; i < MATRIX_ROWS; i++) {
        setPinOutput(row_pins[i]);
        writePinLow(row_pins[i]);
    }

    

    setPinOutput(DISCHARGE_PIN);
    writePinLow(DISCHARGE_PIN);

    setPinOutput(MUX1_EN);
    writePinLow(MUX1_EN);

    setPinOutput(mux_sel_pins[0]);
    writePinLow(mux_sel_pins[0]);

    setPinOutput(mux_sel_pins[1]);
    writePinLow(mux_sel_pins[1]);

    setPinOutput(mux_sel_pins[2]);
    writePinLow(mux_sel_pins[2]);


    // writePinHigh(mux_sel_pins[0]);
    // writePinLow(mux_sel_pins[1]);
    // writePinLow(mux_sel_pins[2]);
    // writePinLow(MUX1_EN);

    analogReadPin(ECOUT_ADC_PIN);   
    matrix_debug_timer = timer_read32(); 
}
void matrix_init_custom(void) {
    // initialize key pins
    matrix_init_pins();

    for (uint8_t row = 0; row < MATRIX_ROWS; row++)
    {
        for (uint8_t col = 0; col < MATRIX_COLS; col++)
        {
            ec_keys[row][col].adc_value = 0;
            ec_keys[row][col].adc_min = 9999;
            ec_keys[row][col].adc_value = 0;
            ec_filter_initqueue(&ec_adc_buffs[row][col]);
        }
    }
}
