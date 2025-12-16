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
#include "quantum.h"
#include "rgb_matrix.h"
#include "rgb_matrix_blink_effect.h"

static rgb_matrix_blink_t rgb_blink_tasks[MAX_RGB_MATRIX_BLINK_TASKS] = {0};

void rgb_matrix_blink_effect_init(void)
{
    for (uint8_t i = 0; i < MAX_RGB_MATRIX_BLINK_TASKS; i++) {
        rgb_blink_tasks[i].index        = 0;
        rgb_blink_tasks[i].blink_nums   = 0;
        rgb_blink_tasks[i].on_time      = 0;
        rgb_blink_tasks[i].off_time     = 0;
        rgb_blink_tasks[i].red          = 0;
        rgb_blink_tasks[i].green        = 0;
        rgb_blink_tasks[i].blue         = 0;
        rgb_blink_tasks[i].active       = 0;
        rgb_blink_tasks[i].is_on        = 0;
        rgb_blink_tasks[i].counter      = 0;
    }
}

void rgb_matrix_blink(int index, uint8_t red, uint8_t green, uint8_t blue, uint16_t blink_nums, uint16_t on_ms, uint16_t off_ms) 
{
    if(blink_nums == 0)
    {
        blink_nums = blink_nums;
    }
    for (int i = 0; i < MAX_RGB_MATRIX_BLINK_TASKS; i++) {
        if (!rgb_blink_tasks[i].active) {
            rgb_blink_tasks[i].index        = index;
            rgb_blink_tasks[i].blink_nums   = blink_nums;
            rgb_blink_tasks[i].on_time      = on_ms;
            rgb_blink_tasks[i].off_time     = off_ms;
            rgb_blink_tasks[i].red          = red;
            rgb_blink_tasks[i].green        = green;
            rgb_blink_tasks[i].blue         = blue;
            rgb_blink_tasks[i].active       = 1;
            rgb_blink_tasks[i].is_on        = 1;
            rgb_blink_tasks[i].counter      = 0;
            break;
        }
    }
}
void rgb_matrix_all_unblink(void) 
{
    for (uint8_t i = 0; i < MAX_RGB_MATRIX_BLINK_TASKS; i++) {
        rgb_blink_tasks[i].index        = 0;
        rgb_blink_tasks[i].blink_nums   = 0;
        rgb_blink_tasks[i].on_time      = 0;
        rgb_blink_tasks[i].off_time     = 0;
        rgb_blink_tasks[i].red          = 0;
        rgb_blink_tasks[i].green        = 0;
        rgb_blink_tasks[i].blue         = 0;
        rgb_blink_tasks[i].active       = 0;
        rgb_blink_tasks[i].is_on        = 0;
        rgb_blink_tasks[i].counter      = 0;
    }
}




bool rgb_matrix_blink_effect_hook(uint8_t led_min, uint8_t led_max)
{
    for (int i = 0; i < MAX_RGB_MATRIX_BLINK_TASKS; i++) {
        if (!rgb_blink_tasks[i].active) continue;
        // 时间推进
        rgb_blink_tasks[i].counter++;
        if (rgb_blink_tasks[i].is_on) {
            if (rgb_blink_tasks[i].counter >= rgb_blink_tasks[i].on_time) {
                rgb_blink_tasks[i].is_on = 0;
                rgb_blink_tasks[i].counter = 0;
                if (rgb_blink_tasks[i].blink_nums > 0 && --rgb_blink_tasks[i].blink_nums == 0) {
                    rgb_blink_tasks[i].active = 0;
                }
            }
        } else {
            if (rgb_blink_tasks[i].counter >= rgb_blink_tasks[i].off_time) {
                rgb_blink_tasks[i].is_on = 1;
                rgb_blink_tasks[i].counter = 0;
            }
        }
        if (rgb_blink_tasks[i].active && rgb_blink_tasks[i].is_on) {
            rgb_matrix_set_color(rgb_blink_tasks[i].index, rgb_blink_tasks[i].red, rgb_blink_tasks[i].green, rgb_blink_tasks[i].blue); 
        } else {
            rgb_matrix_set_color(rgb_blink_tasks[i].index, 0, 0, 0);  
        }
    }
    return false;
}