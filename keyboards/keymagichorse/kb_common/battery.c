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
#include "timer.h"
#include "bhq_common.h"
#include "km_analog.h"
#include "bhq.h"

uint8_t battery_percent = 100;  // 电池电量百分比
uint8_t battery_is_valid = 0;   // 电池电量是否有效
uint16_t battery_mv = 0;        // 电池毫伏
uint32_t battery_timer = 0;     // 电池采样计时

uint8_t battery_update_ble_flag = 0;    // 是否更新电量百分比到蓝牙模块
uint8_t battery_is_read_flag = 0;        // 是否允许读取电量



__attribute__((weak))  void battery_percent_changed_user(uint8_t level){}
__attribute__((weak))  void battery_percent_changed_kb(uint8_t level){}
void battery_percent_changed(uint8_t level)
{
    if(battery_update_ble_flag == 1)
    {
        bhq_update_battery_percent(battery_percent, battery_mv);
    }
    battery_percent_changed_user(level);
    battery_percent_changed_kb(level);
}


// 电池电压转百分比
uint8_t calculate_battery_percentage(uint16_t current_mv) {
    if (current_mv >= BATTERY_MAX_MV) {
        return 100;
    } else if (current_mv <= BATTERY_MIN_MV) {
        return 0;
    } else {
        uint16_t percentage = ((current_mv - BATTERY_MIN_MV) * 100) / (BATTERY_MAX_MV - BATTERY_MIN_MV);
        // 如果百分比超过100，确保其被限制在100以内
        if (percentage > 100) {
            percentage = 100;
        }
        return (uint8_t)percentage;
    }
}

// 读取电量百分比
uint8_t battery_read_percent(void)
{
    uint8_t sta = 1;
    /* USB 供电时固定 100% */
    if (usb_power_connected()) {
        battery_percent = 100;
        return 0;
    }

    uint32_t sum = 0;
    uint16_t max_v = 0;
    uint16_t min_v = UINT16_MAX;
    const uint8_t NUM_SAMPLES = 10;
    static uint8_t last_sample = 0xff;        // 改为uint8_t，因为存储的是百分比
    static uint8_t stable_count = 0;

    // ADC采样
    km_analogReadPin(BATTERY_ADC_PIN);
    wait_us(50);

    for (uint8_t i = 0; i < NUM_SAMPLES; i++) 
    {
        uint16_t v = km_analogReadPin(BATTERY_ADC_PIN) >> 2;
        
        // 异常值处理
        if (v < 5) {
            wait_us(10); 
            v = km_analogReadPin(BATTERY_ADC_PIN) >> 2;
            if (v < 5) {
                wait_us(50);
                km_analogAdcStop(BATTERY_ADC_PIN);
                sta = 0;
                break;  // 采样失败直接返回
            }
        }
        
        sum += v;
        if (v > max_v) max_v = v;
        if (v < min_v) min_v = v;
    }

    /* 去极值平均 */
    sum -= (uint32_t)max_v + (uint32_t)min_v;
    uint16_t adc = (uint16_t)(sum / (NUM_SAMPLES - 2));

    /* 转换为电压（mV）- 修复精度问题 */
    uint16_t voltage_mV_Fenya = (adc * 3300UL) / 1023;
    uint16_t voltage_mV_actual = (voltage_mV_Fenya * (BAT_R_UPPER + BAT_R_LOWER)) / BAT_R_LOWER;

    /* 计算电量百分比 */
    uint8_t new_percent = calculate_battery_percentage(voltage_mV_actual);

    /* 5% 一档 */
    new_percent = ((new_percent + 2) / 5) * 5;
    if (new_percent > 100) new_percent = 100;

    km_analogAdcStop(BATTERY_ADC_PIN);
    
    // 消抖逻辑
    if (new_percent == last_sample) {
        stable_count++;
        if(stable_count > 254)  // 避免临界值
        {
            stable_count = 254;
        }
        km_printf("bat stable++: %d\n", stable_count);
    } else {
        km_printf("bat %d!=%d\n", last_sample, new_percent);
        stable_count = 1;
        last_sample = new_percent;
    }
    
    if (stable_count >= 6) {
        battery_mv = voltage_mV_actual;
        if(battery_percent != new_percent)
        {
            battery_percent_changed(battery_percent);
        }
        battery_percent = new_percent;
        km_printf("stable success: %dmV -> %d\n", battery_mv, battery_percent);
        // stable_count = 0; 
    }
    
    return sta;  
}

void battery_init(void)
{
    battery_is_read_flag = 1;        // 是否允许读取电量
    battery_is_valid = 0;
}

void battery_task(void)
{ 
    uint8_t sta = 0;
    // 定时任务，2秒执行一次
    if (timer_elapsed32(battery_timer) > 500) 
    {
        battery_timer = timer_read32();
        if(battery_is_read_flag == 1)
        {
            sta = battery_read_percent();
            battery_is_valid = sta;
        }
        km_printf("%d %d\n",battery_is_read_flag,battery_update_ble_flag);
    }
}

void battery_reset_timer(void)
{
    battery_timer = timer_read32();
}

uint8_t battery_percent_get(void)
{
    if(battery_is_valid == 1)
    {
        return battery_percent;
    }
    else
    {
        return 0xff;
    }
}

/**
 * @brief 使能电池电压读取
 */
void battery_enable_read(void)
{
    battery_is_read_flag = 1;
}

/**
 * @brief 禁用电池电压读取
 */
void battery_disable_read(void)
{
    battery_is_read_flag = 0;
}


/**
 * @brief 使能电池电量更新到蓝牙
 */
void battery_enable_ble_update(void)
{
    battery_update_ble_flag = 1;
}

/**
 * @brief 禁用电池电量更新到蓝牙  
 */
void battery_disable_ble_update(void)
{
    battery_update_ble_flag = 0;
}
