VPATH += keyboards/keymagichorse/kb_common/

# 74HC595 io扩展
ifeq ($(strip $(SHIFT595_ENABLED)), yes)
    OPT_DEFS += -DSHIFT595_ENABLED
    VPATH += keyboards/keymagichorse/kb_common/74hc595/
    SRC += kb_common/74hc595/74hc595.c
endif

# 日记类型 不写默认就是不打开 uart是借用蓝牙的串口的简单调试可以
# rtt这个低功耗之后可能就用不了了，用起来就很麻烦，所以添加了uart借用bhq的串口用一下
KB_DEBUG ?= not_debug
SRC+= kb_common/km_printf.c
ifeq ($(strip $(KB_DEBUG)), rtt)
	OPT_DEFS += -DKB_DEBUG
	OPT_DEFS += -DKB_DEBUG_RTT
	SRC+= kb_common/rtt/SEGGER_RTT.c
	SRC+= kb_common/rtt/SEGGER_RTT_printf.c
	VPATH += keyboards/keymagichorse/kb_common/kb_common/rtt
endif   
ifeq ($(strip $(KB_DEBUG)), uart_bhq)
	OPT_DEFS += -DKB_DEBUG_UART_BHQ
	OPT_DEFS += -DKB_DEBUG
# 
endif   

# 静电容
ifeq ($(strip $(KB_EC_ENABLED)), yes)
	OPT_DEFS += -DKB_EC_ENABLED
	KM_ANALOG_ENABLED = yes
    ifeq ($(strip $(MUX_TYPE)), USE_74HC4051)
	    OPT_DEFS += -DUSE_74HC4051
    endif

    ifeq ($(strip $(MUX_TYPE)), USE_74HC4067)
	    OPT_DEFS += -DUSE_74HC4067
    endif
    VPATH += keyboards/keymagichorse/kb_common/ec_lib/
    SRC += kb_common/ec_lib/mux_74hc40xx.c
    # custom matrix setup
    CUSTOM_MATRIX = lite	
    SRC += kb_common/ec_lib/ec_matrix.c
endif

# 蓝牙
ifeq ($(strip $(BLUETOOTH_DRIVER)), bhq)
    # 低功耗
    ifeq ($(strip $(KB_LPM_ENABLED)), yes)
        OPT_DEFS += -DKB_LPM_ENABLED
        OPT_DEFS += -DKB_LPM_DRIVER
        SRC += kb_common/${KB_LPM_DRIVER}.c
    endif
    # 电池
    ifeq ($(strip $(KB_CHECK_BATTERY_ENABLED)), yes)
        OPT_DEFS += -DKB_CHECK_BATTERY_ENABLED
	    KM_ANALOG_ENABLED = yes
        SRC += kb_common/battery.c
    endif

    SRC += kb_common/bhq_common.c
    SRC += kb_common/transport.c
    SRC += kb_common/wireless.c

endif

# ADC  改为自己修改过的 km_analog.c
ifeq ($(strip $(KM_ANALOG_ENABLED)), yes)
    OPT_DEFS += -DHAL_USE_ADC=TRUE
    SRC += kb_common/km_analog.c
endif

ifeq ($(strip $(RGB_MATRIX_CUSTOM_BLINK_EFFECT)), yes)
ifeq ($(strip $(RGB_MATRIX_ENABLE)), no)
  $(error RGB_MATRIX_CUSTOM_BLINK_EFFECT depends on the RGB_MATRIX_ENABLE)
endif
    OPT_DEFS += -DRGB_MATRIX_CUSTOM_BLINK_EFFECT
    VPATH += keyboards/keymagichorse/kb_common/rgb_matrix_custom_effect/rgb_matrix_blink_effect
    SRC += kb_common/rgb_matrix_custom_effect/rgb_matrix_blink_effect/rgb_matrix_blink_effect.c
endif

ifeq ($(strip $(RGB_MATRIX_CUSTOM_BATTERY_EFFECT)), yes)
ifeq ($(strip $(RGB_MATRIX_ENABLE)), no)
  $(error RGB_MATRIX_CUSTOM_BATTERY_EFFECT depends on the RGB_MATRIX_ENABLE)
endif
ifeq ($(strip $(KB_CHECK_BATTERY_ENABLED)), no)
  $(error RGB_MATRIX_CUSTOM_BATTERY_EFFECT depends on the KB_CHECK_BATTERY_ENABLED)
endif
    OPT_DEFS += -DRGB_MATRIX_CUSTOM_BATTERY_EFFECT
    VPATH += keyboards/keymagichorse/kb_common/rgb_matrix_custom_effect/rgb_matrix_battery_effect
    SRC += kb_common/rgb_matrix_custom_effect/rgb_matrix_battery_effect/rgb_matrix_battery_effect.c
endif


