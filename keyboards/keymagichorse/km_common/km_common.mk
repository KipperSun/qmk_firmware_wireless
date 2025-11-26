VPATH += keyboards/keymagichorse/km_common/

# 74HC595 io扩展
ifeq ($(strip $(SHIFT595_ENABLED)), yes)
    OPT_DEFS += -DSHIFT595_ENABLED
    VPATH += keyboards/keymagichorse/km_common/74hc595/
    SRC += km_common/74hc595/74hc595.c
endif

# 日记类型 不写默认就是不打开 uart是借用蓝牙的串口的简单调试可以
# rtt这个低功耗之后可能就用不了了，用起来就很麻烦，所以添加了uart借用bhq的串口用一下
KM_DEBUG ?= not_debug
SRC+= km_common/km_printf.c
ifeq ($(strip $(KM_DEBUG)), rtt)
	OPT_DEFS += -DKM_DEBUG
	OPT_DEFS += -DKM_DEBUG_RTT
	SRC+= km_common/rtt/SEGGER_RTT.c
	SRC+= km_common/rtt/SEGGER_RTT_printf.c
	VPATH += keyboards/keymagichorse/km_common/km_common/rtt
endif   
ifeq ($(strip $(KM_DEBUG)), uart_bhq)
	OPT_DEFS += -DKM_DEBUG_UART_BHQ
	OPT_DEFS += -DKM_DEBUG
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
    VPATH += keyboards/keymagichorse/km_common/ec_lib/
    SRC += km_common/ec_lib/mux_74hc40xx.c
    # custom matrix setup
    CUSTOM_MATRIX = lite	
    SRC += km_common/ec_lib/ec_matrix.c
endif

# 蓝牙
ifeq ($(strip $(BLUETOOTH_DRIVER)), bhq)
    # 低功耗
    ifeq ($(strip $(KB_LPM_ENABLED)), yes)
        OPT_DEFS += -DKB_LPM_ENABLED
        OPT_DEFS += -DKB_LPM_DRIVER
        SRC += km_common/${KB_LPM_DRIVER}.c
    endif
    # 电池
    ifeq ($(strip $(KB_CHECK_BATTERY_ENABLED)), yes)
        OPT_DEFS += -DKB_CHECK_BATTERY_ENABLED
	    KM_ANALOG_ENABLED = yes
        SRC += km_common/battery.c
    endif

    SRC += km_common/bhq_common.c
    SRC += km_common/transport.c
    SRC += km_common/wireless.c

endif

# ADC  改为自己修改过的 km_analog.c
ifeq ($(strip $(KM_ANALOG_ENABLED)), yes)
    OPT_DEFS += -DHAL_USE_ADC=TRUE
    SRC += km_common/km_analog.c
endif
