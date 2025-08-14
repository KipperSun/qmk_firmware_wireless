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

#if defined(KM_DEBUG)
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

#   if defined(KM_DEBUG)
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
		{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
		{15, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, NO_LED,NO_LED},
		{30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45},
		{46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61},
		{62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 74, 76, 77,},
	}, 
    // LED Index to Physical Position
    {
        // Row 0: 16 keys
        { 0, 0 }, { 15, 0 }, { 30, 0 }, { 45, 0 }, { 60, 0 }, { 75, 0 }, { 90, 0 }, { 105, 0 },
        { 120, 0 }, { 135, 0 }, { 150, 0 }, { 165, 0 }, { 180, 0 }, { 195, 0 }, { 210, 0 }, { 224, 0 },

        // Row 1: 14 keys
        { 0, 16 }, { 17, 16 }, { 34, 16 }, { 51, 16 }, { 68, 16 }, { 85, 16 }, { 102, 16 }, { 119, 16 },
        { 136, 16 }, { 153, 16 }, { 170, 16 }, { 187, 16 }, { 204, 16 }, { 224, 16 },

        // Row 2: 16 keys
        { 0, 32 }, { 15, 32 }, { 30, 32 }, { 45, 32 }, { 60, 32 }, { 75, 32 }, { 90, 32 }, { 105, 32 },
        { 120, 32 }, { 135, 32 }, { 150, 32 }, { 165, 32 }, { 180, 32 }, { 195, 32 }, { 210, 32 }, { 224, 32 },

        // Row 3: 16 keys
        { 0, 48 }, { 15, 48 }, { 30, 48 }, { 45, 48 }, { 60, 48 }, { 75, 48 }, { 90, 48 }, { 105, 48 },
        { 120, 48 }, { 135, 48 }, { 150, 48 }, { 165, 48 }, { 180, 48 }, { 195, 48 }, { 210, 48 }, { 224, 48 },

        // Row 4: 16 keys
        { 0, 64 }, { 15, 64 }, { 30, 64 }, { 45, 64 }, { 60, 64 }, { 75, 64 }, { 90, 64 }, { 105, 64 },
        { 120, 64 }, { 135, 64 }, { 150, 64 }, { 165, 64 }, { 180, 64 }, { 195, 64 }, { 210, 64 }, { 224, 64 },
    },
    {
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,         // 16 
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,               // 14
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,         // 16 
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,         // 16 
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,         // 16 
	}
};




// led_config_t g_led_config = {
//     {
//         // Key Matrix to LED Index (70 按键)
//         {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13 },
//         { 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27 },
//         { 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41 },
//         { 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55 },
//         { 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69 }
//     },
//     {
//         // LED Index to Physical Position {x, y} (78 LEDs)
//         // 第一行
//         {  0,  0}, { 16,  0}, { 32,  0}, { 48,  0}, { 64,  0}, { 80,  0}, { 96,  0}, {112,  0}, {128,  0}, {144,  0}, {160,  0}, {176,  0}, {192,  0}, {208,  0}, {224,  0}, {232,  0},
//         // 第二行
//         {  0, 16}, { 16, 16}, { 32, 16}, { 48, 16}, { 64, 16}, { 80, 16}, { 96, 16}, {112, 16}, {128, 16}, {144, 16}, {160, 16}, {176, 16}, {192, 16}, {208, 16}, {224, 16}, {232, 16},
//         // 第三行
//         {  0, 32}, { 16, 32}, { 32, 32}, { 48, 32}, { 64, 32}, { 80, 32}, { 96, 32}, {112, 32}, {128, 32}, {144, 32}, {160, 32}, {176, 32}, {192, 32}, {208, 32}, {224, 32}, {232, 32},
//         // 第四行
//         {  0, 48}, { 16, 48}, { 32, 48}, { 48, 48}, { 64, 48}, { 80, 48}, { 96, 48}, {112, 48}, {128, 48}, {144, 48}, {160, 48}, {176, 48}, {192, 48}, {208, 48}, {224, 48}, {232, 48},
//         // 第五行
//         {  0, 64}, { 16, 64}, { 32, 64}, { 48, 64}, { 64, 64}, { 80, 64}, { 96, 64}, {112, 64}, {128, 64}, {144, 64}, {160, 64}, {176, 64}, {192, 64}, {208, 64}, {224, 64}, {232, 64}
//     },
//     {
//         // LED Index to Flag (78 LEDs)
//         4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
//         4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
//         4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
//         4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
//         4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
//     }
// };

#endif


