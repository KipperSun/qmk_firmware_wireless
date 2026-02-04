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
#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(
    KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_HOME,
    KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_PGUP,
    KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  KC_PGDN,
    KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_UP,   KC_DEL,
    KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             MO(1),   KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT),
  [1] = LAYOUT(
    KC_GRV , KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_TRNS, KC_TRNS,
    KC_TRNS, BL_SW_0, BL_SW_1, BL_SW_2, RF_TOG,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, USB_TOG, NK_TOGG, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS,
    KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, RM_TOGG, RM_NEXT, RM_PREV, KC_TRNS, KC_TRNS, KC_TRNS, KC_BRIU, KC_TRNS,
    KC_TRNS, GU_TOGG, KC_TRNS,                            KC_TRNS,                            KC_TRNS, RGB_BAT, KC_VOLD, KC_BRID, KC_VOLU),
  [2] = LAYOUT(
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS,
    KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS,                            KC_TRNS,                            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
  [3] = LAYOUT(
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS,
    KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS,                            KC_TRNS,                            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS)
};

//以下为RGB配置
//写在keymap.c里面可以用精准的带小数的坐标
//写在rgb.c里面只能用int类型的坐标

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
        { 8.75,64}, {26.25,64}, {43.75,64}, {70.00,64},                          { 96.25,64},                           {122.50,64}, {148.75,64}, {166.25,64}, {189.00,64}, {203.00,48}, {217.00,64},
    },
    {
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,      // 15
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,      // 15
		8, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,         // 14 第一个是大写灯
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,         // 14
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4                   // 11
	}
};
