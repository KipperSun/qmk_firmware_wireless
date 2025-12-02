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

#include "uart.h"

void board_init(void) 
{
#if defined(BLUETOOTH_BHQ)
    bhq_common_init();
#   if defined(KB_LPM_ENABLED)
    lpm_init();
#   endif
#endif
}

uint32_t temp_log_cut1 = 0;
uint8_t hello_log_2[] = "hello debug11\n";
void housekeeping_task_kb(void) {
#if defined(BLUETOOTH_BHQ)
    bhq_wireless_task();
    #   if defined(KB_LPM_ENABLED)
        lpm_task();
    #   endif
#endif
    if (timer_elapsed32(temp_log_cut1) > 500) 
    {
        temp_log_cut1 = timer_read32();
		uart_transmit(hello_log_2,14);
    }
}

