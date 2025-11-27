/* Copyright 2025 keymagichorse
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
#include "battery.h"
#include "km_analog.h"
#include "bhq.h"
#include "bhq_common.h"
#include "wireless.h"
#include "transport.h"
#include "timer.h"

uint32_t battery_timer = 0;
uint8_t battery_percent = 100;
// 其实这个标志位算是是否上报电池电量到蓝牙模块。
uint8_t battery_is_start = 0;


// 电池电压转百分比
uint8_t calculate_battery_percentage(uint16_t current_mv) {
    if (current_mv >= BATTER_MAX_MV) {
        return 100;
    } else if (current_mv <= BATTER_MIN_MV) {
        return 0;
    } else {
        uint16_t percentage = ((current_mv - BATTER_MIN_MV) * 100) / (BATTER_MAX_MV - BATTER_MIN_MV);
        // 如果百分比超过100，确保其被限制在100以内
        if (percentage > 100) {
            percentage = 100;
        }
        return (uint8_t)percentage;
    }
}

// 读取并更新数据
void battery_read_and_update_data(void)
{
    uint32_t sum   = 0;
    uint16_t max_v = 0;
    uint16_t min_v = 0xFFFF;
    const uint8_t NUM_SAMPLES = 10;

    km_analogReadPin(BATTER_ADC_PIN);
    wait_us(50);

    for (uint8_t i = 0; i < NUM_SAMPLES; i++) {
        uint16_t v = km_analogReadPin(BATTER_ADC_PIN) >> 2;    // 20251015：这里使用km_analog.c没有缩放，所以这里缩放
        if (v == 0) {
            return;
        }
        sum += v;
        if (v > max_v) max_v = v;
        if (v < min_v) min_v = v;
    }

    /* 去极值平均 */
    sum -= (uint32_t)max_v + (uint32_t)min_v;
    uint16_t adc = (uint16_t)(sum / (NUM_SAMPLES - 2));

    /* 转换为电压（mV） */
    uint16_t voltage_mV_Fenya  = (adc * 3300) / 1023;
    uint16_t voltage_mV_actual = voltage_mV_Fenya * (1 + (BAT_R_UPPER / BAT_R_LOWER));

    /* 计算电量百分比 */
    uint8_t new_percent = calculate_battery_percentage(voltage_mV_actual);

    /* 5% 一档 */
    new_percent = ((new_percent + 2) / 5) * 5;
    if (new_percent > 100) new_percent = 100;

    km_printf("adc:%d fymv:%d sjmv:%d bfb:%d\r\n",
              adc, voltage_mV_Fenya, voltage_mV_actual, new_percent);
    km_printf("adcState:%d\r\n", ADCD1.state);

    analogAdcStop(BATTER_ADC_PIN);

    /* USB 供电时固定 100% */
    if (usb_power_connected()) {
        battery_percent = 100;
        km_printf("usb_power_connected\r\n");
        return;
    }

    battery_percent = new_percent;

    if (battery_is_start == 0)
        return;

    bhq_update_battery_percent(battery_percent, voltage_mV_actual);
}


void battery_percent_read_task(void)
{ 

    if (battery_timer == 0) {
        battery_timer = timer_read32();
        battery_read_and_update_data();
    }

    // 定时任务，2秒执行一次
    if (timer_elapsed32(battery_timer) > 2000) {
        battery_timer = timer_read32();
        battery_read_and_update_data();
    }
}
void battery_reset_timer(void)
{
    battery_timer = timer_read32();
}

uint8_t battery_get(void)
{
    return battery_percent;
}

void battery_stop(void)
{
    battery_is_start = 0;
}

void battery_start(void)
{
    battery_is_start = 1;
    km_analogReadPin(BATTER_ADC_PIN);
}

void battery_init(void)
{
    // battery_reset_timer();
}