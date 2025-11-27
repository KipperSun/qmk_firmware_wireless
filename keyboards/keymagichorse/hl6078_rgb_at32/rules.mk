MAKEFLAGS += -j10
MCU_LDSCRIPT = AT32F415xB

# custom matrix setup
CUSTOM_MATRIX = lite	
SRC  += matrix.c

VPATH += $(TOP_DIR)/keyboards/keymagichorse
