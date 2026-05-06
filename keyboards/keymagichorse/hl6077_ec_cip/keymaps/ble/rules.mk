
# 是否使能低功耗
KB_LPM_ENABLED = no
KB_LPM_DRIVER = lpm_stm32f4_rtc_ec_v1
# 是否使能QMK端读取电池电压
KB_CHECK_BATTERY_ENABLED = no
# 开启键盘层DEBUG  
KB_DEBUG = uart_bhq

# 静电容使能
include keyboards/keymagichorse/kb_common/kb_common.mk

VIA_ENABLE = yes
