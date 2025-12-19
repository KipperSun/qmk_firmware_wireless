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
#include "quantum.h"

#if defined(BLUETOOTH_BHQ)
#   include "bhq.h"
#   include "bhq_common.h"
#endif

#if defined(KB_LPM_ENABLED)
#   include "lpm.h"
#endif

#if defined(KB_DEBUG)
#   include "km_printf.h"
#endif
#include "battery.h"



void board_init(void)
{
#if defined(BLUETOOTH_BHQ)
    bhq_common_init();
#   if defined(KB_LPM_ENABLED)
    lpm_init();
#   endif
#endif

#   if defined(KB_DEBUG)
    km_printf_init();
    km_printf("hello rtt log1111111\r\n");
#   endif
}
void housekeeping_task_kb(void) {
#if defined(BLUETOOTH_BHQ)
    bhq_wireless_task();
    #   if defined(KB_LPM_ENABLED)
        lpm_task();
    #   endif
#endif
}

#ifdef RGB_MATRIX_ENABLE

led_config_t g_led_config = {
	{
    {     54,     55,     56,     57,     58,     59,     60,     61,     62,     63,     64,     65,     66,     67,     68 },
		{     53,     52,     51,     50,     49,     48,     47,     46,     45,     44,     43,     42,     41,     40,     39 },
		{     25,     26,     27,     28,     29,     30,     31,     32,     33,     34,     35,     36, NO_LED,     37,     38 },
		{     24, NO_LED,     23,     22,     21,     20,     19,     18,     17,     16,     15,     14,     13,     12,     11 },
		{      0,      1,      2,      3, NO_LED, NO_LED,      4, NO_LED, NO_LED,      5,      6,      7,      8,      9,     10 }
	},
    // LED Index to Physical Position - 已四舍五入取整
    {
        // Row 0: 15 keys
        { 7, 0}, {21, 0}, {35, 0}, {49, 0}, {63, 0}, {77, 0}, {91, 0}, {105, 0}, {119, 0}, {133, 0}, {147, 0}, {161, 0}, {175, 0}, {196, 0}, {217, 0},

        // Row 1: 15 keys
        {11, 16}, {28, 16}, {42, 16}, {56, 16}, {70, 16}, {84, 16}, {98, 16}, {112, 16}, {126, 16}, {140, 16}, {154, 16}, {168, 16}, {182, 16}, {200, 16}, {217, 16},

        // Row 2: 14 keys
        {12, 32}, {32, 32}, {46, 32}, {60, 32}, {74, 32}, {88, 32}, {102, 32}, {116, 32}, {130, 32}, {144, 32}, {158, 32}, {172, 32}, {194, 32}, {217, 32},

        // Row 3: 14 keys
        {16, 48}, {39, 48}, {53, 48}, {67, 48}, {81, 48}, {95, 48}, {109, 48}, {123, 48}, {137, 48}, {151, 48}, {164, 48}, {184, 48}, {203, 48}, {217, 48},

        // Row 4: 11 keys
        {9, 64}, {26, 64}, {44, 64}, {61, 64}, {96, 64}, {131, 64}, {149, 64}, {166, 64}, {189, 64}, {203, 64}, {217, 64},
    },
    {
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,      // 15
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,      // 15
		8, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,         // 14 第一个是大写灯
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,         // 14
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4                   // 11
	}
};

#endif


//以下为精准坐标，待QMK有朝一日修改即可启用

/*#ifdef RGB_MATRIX_ENABLE

led_config_t g_led_config = {
	{
    {     54,     55,     56,     57,     58,     59,     60,     61,     62,     63,     64,     65,     66,     67,     68 },
		{     53,     52,     51,     50,     49,     48,     47,     46,     45,     44,     43,     42,     41,     40,     39 },
		{     25,     26,     27,     28,     29,     30,     31,     32,     33,     34,     35,     36, NO_LED,     37,     38 },
		{     24, NO_LED,     23,     22,     21,     20,     19,     18,     17,     16,     15,     14,     13,     12,     11 },
		{      0,      1,      2,      3, NO_LED, NO_LED,      4, NO_LED, NO_LED,      5,      6,      7,      8,      9,     10 }
	},
    // LED Index to Physical Position
    {
        // Row 0: 15 keys
        { 7.00, 0}, {21.00, 0}, {35.00, 0}, {49.00, 0}, {63.00, 0}, { 77.00, 0}, { 91.00, 0}, {105.00, 0}, {119.00, 0}, {133.00, 0}, {147.00, 0}, {161.00, 0}, {175.00, 0}, {196.00, 0}, {217.00, 0},

        // Row 1: 15 keys
        {10.50,16}, {28.00,16}, {42.00,16}, {56.00,16}, {70.00,16}, { 84.00,16}, { 98.00,16}, {112.00,16}, {126.00,16}, {140.00,16}, {154.00,16}, {168.00,16}, {182.00,16}, {199.50,16}, {217.00,16},

        // Row 2: 14 keys
        {12.25,32}, {31.50,32}, {45.50,32}, {59.50,32}, {73.50,32}, { 87.50,32}, {101.50,32}, {115.50,32}, {129.50,32}, {143.50,32}, {157.50,32}, {171.50,32},              {194.25,32}, {217.00,32},

        // Row 3: 14 keys
        {15.75,48},             {38.50,48}, {52.50,48}, {66.50,48}, { 80.50,48}, { 94.50,48}, {108.50,48}, {122.50,48}, {136.50,48}, {150.50,48}, {164.50,48}, {183.75,48}, {203.00,48}, {217.00,48},

        // Row 4: 11 keys
        { 8.75,64}, {26.25,64}, {43.75,64}, {61.25,64},                          { 96.25,64},                           {131.25,64}, {148.75,64}, {166.25,64}, {189.00,64}, {203.00,48}, {217.00,64},
    },
    {
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,      // 15
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,      // 15
		8, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,         // 14 第一个是大写灯
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,         // 14
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4                   // 11
	}
};

#endif*/
