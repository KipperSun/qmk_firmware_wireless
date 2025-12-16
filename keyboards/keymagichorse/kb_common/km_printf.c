/* Copyright 2025 keykmhorse
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
#include <stdarg.h>
#include "km_printf.h"

#if defined(KB_DEBUG_RTT)
    #include "SEGGER_RTT.h"
#elif defined(KB_DEBUG_UART_BHQ)
    #include "uart.h"
#endif

void km_printf_init(void)
{
#if defined(KB_DEBUG_RTT)
    SEGGER_RTT_Init();
#elif defined(KB_DEBUG_UART_BHQ)
    // BHQ 驱动已含
#endif

}


void km_putchar(char c)
{
#if defined(KB_DEBUG_RTT)
    SEGGER_RTT_printf(0, "%c", c);
#elif defined(KB_DEBUG_UART_BHQ)
    uart_write(c);
#endif
}

int km_printf(const char* format, ...) {
#if defined(KB_DEBUG)
    va_list args;
    va_start(args, format);
    
    int count = 0;
    
    while (*format) {
        if (*format == '%' && format[1] == 'd') {
            format++;
            unsigned int value = va_arg(args, unsigned int);
            char buf[10];
            char *p = buf + 9;
            *p = '\0';
            
            do {
                *--p = '0' + (value % 10);
                value /= 10;
            } while (value);
            
            while (*p) {
                km_putchar(*p++);
                count++;
            }
        }
        else if (*format == '%' && format[1] == '0' && format[2] == '2' && format[3] == 'x') {
            format += 3;
            unsigned int value = va_arg(args, unsigned int);
            
            km_putchar("0123456789abcdef"[(value >> 4) & 0x0F]);
            km_putchar("0123456789abcdef"[value & 0x0F]);
            count += 2;
        }
        else if (*format == '%' && format[1] == '0' && format[2] == '4' && format[3] == 'x') {
            format += 3;
            unsigned int value = va_arg(args, unsigned int);
            
            km_putchar("0123456789abcdef"[(value >> 12) & 0x0F]);
            km_putchar("0123456789abcdef"[(value >> 8) & 0x0F]);
            km_putchar("0123456789abcdef"[(value >> 4) & 0x0F]);
            km_putchar("0123456789abcdef"[value & 0x0F]);
            count += 4;
        }
        else if (*format == '%' && format[1] == '0' && format[2] == '8' && format[3] == 'x') {
            format += 3;
            unsigned int value = va_arg(args, unsigned int);
            
            km_putchar("0123456789abcdef"[(value >> 28) & 0x0F]);
            km_putchar("0123456789abcdef"[(value >> 24) & 0x0F]);
            km_putchar("0123456789abcdef"[(value >> 20) & 0x0F]);
            km_putchar("0123456789abcdef"[(value >> 16) & 0x0F]);
            km_putchar("0123456789abcdef"[(value >> 12) & 0x0F]);
            km_putchar("0123456789abcdef"[(value >> 8) & 0x0F]);
            km_putchar("0123456789abcdef"[(value >> 4) & 0x0F]);
            km_putchar("0123456789abcdef"[value & 0x0F]);
            count += 8;
        }
        else {
            km_putchar(*format);
            count++;
        }
        format++;
    }
    
    va_end(args);
    return count;
#else
    return 0;
#endif
}
