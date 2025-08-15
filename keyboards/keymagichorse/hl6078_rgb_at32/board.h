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

#include_next <board.h>
// at32的board.h里面默认宏定义了一个AT32F415KB
// 默认开启了GPIOC GPIOF，使用AT32F415CB就会报错。需要取消AT32F415KB宏定义
#undef AT32F415KB
#define AT32F415CB

#undef AT32_HSECLK
// #define AT32_HSECLK 16000000
#define AT32_HSECLK 8000000
// #define STM32_HSECLK 25000000

