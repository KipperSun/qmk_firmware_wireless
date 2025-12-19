# MCU name
MCU = STM32F072
BOOTLOADER = stm32-dfu


RGB_MATRIX_ENABLE = yes
RGB_MATRIX_DRIVER = ws2812

BOOTMAGIC_ENABLE ?= no     # Virtual DIP switch configuration(+1000)
MOUSEKEY_ENABLE ?= no      # Mouse keys(+4700)
EXTRAKEY_ENABLE ?= no      # Audio control and System control(+450)
CONSOLE_ENABLE ?= yes      # Console for debug(+400)
COMMAND_ENABLE ?= yes      # Commands for debug and configuration
SLEEP_LED_ENABLE ?= no     # Breathing sleep LED during USB suspend
NKRO_ENABLE ?= yes         # USB Nkey Rollover
BACKLIGHT_ENABLE ?= no     # Enable keyboard backlight functionality
AUDIO_ENABLE ?= no
BOOTMAGIC_ENABLE = no
DEFERRED_EXEC_ENABLE = yes

CUSTOM_MATRIX = lite
ANALOG_DRIVER_REQUIRED = yes

# extrakey : 492 bytes
# mousekey : 1774 bytes
# nkro     : 440 bytes
# qmk locking : 216 bytes
# rgb backlight  : 5698 bytes
# rgb matrix     : 10196 bytes
# vial backlight : 4 bytes
# vial rgb : 520 bytes