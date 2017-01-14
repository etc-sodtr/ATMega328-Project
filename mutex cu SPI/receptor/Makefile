# Demo makefile for a simple project starting from scratch using FreeRTOS
#
# ©2016 KCs <csaba.kertesz@etc.unitbv.ro>
#
# based on FreeRTOS demo projects, adapted to understand the usage philosophy
# in the SODTR laboratory

# avoid displaying commands
export MAKEFLAGS += --silent

# name of the project
APP = frdemo

 
# ==============================================================================
# Paths
# ==============================================================================

BIN_PATH = bin
OBJ_PATH = obj
SRC_PATH = src
INC_PATH = include



# insert source paths into search path
vpath %.c $(SRC_PATH)

# ==============================================================================
# Source files
# ==============================================================================

FREERTOS_SRC = \
               tasks.c \
               queue.c \
               list.c \
               port.c \
               heap_1.c \

SPI_SRC = \
               spi.c \
               wl_module.c \

APP_SRC = \
          main.c \




SRC = $(APP_SRC) $(FREERTOS_SRC) $(SPI_SRC)

# ==============================================================================
# Headers
# ==============================================================================

HEADERS = $(addprefix -I,$(INC_PATH))

# ==============================================================================
# target configuration
# ==============================================================================

# mcu type
MCU = atmega328p

# debug level: only affects ELF, not translated into the HEX, so can be left
# to highest level for easier debugging
DEBUG = -g2

# optimization: either optimize for size or for speed, when optimizing
# for speed level 2 is usually better overall than level 3 for the size
# constraints of AVR microconrollers
OPT = -Os

# warnings
# show all warnings and treat them as errors to reduce human error
WARNINGS = -Wall -Wextra -Werror \
           -Wshadow -Wpointer-arith -Wbad-function-cast -Wcast-align \
           -Wsign-compare -Waggregate-return -Wstrict-prototypes \
           -Wmissing-prototypes -Wmissing-declarations -Wunused

# extra flags
# recommended for AVR microcontrollers
EXTRA_FLAGS = -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums

# language standard
# use latest standard with GNU extensions
C_STD = gnu99

# ==============================================================================
# defines
# ==============================================================================

DEFS = \
       GCC_MEGA_AVR \
       F_CPU=8000000UL \
	 #adaugat de mine

# ==============================================================================
# compiler flags
# ==============================================================================

CFLAGS = \
         -mmcu=$(MCU) \
         $(DEBUG) \
         $(OPT) \
         $(WARNINGS) \
         $(EXTRA_FLAGS) \
         -std=$(C_STD) \
         $(addprefix -D,$(DEFS)) \
         -Wa,-adhlns=$(@:.o=.lst) \


ASFLAGS = \
          -Wa,-adhlns=$(@:.o=.lst),-gstabs


LDFLAGS = \
          -mmcu=$(MCU) \
          $(DEBUG) \
          $(OPT) \
          $(WARNINGS) \
          -Wl,-Map=$(OBJ_PATH)/$(APP).map,--cref

# additional libraries like math
# be aware that floating point mathematics are only available as software
# implementation and are extremely costly to run on AVR
LIBS = #-lm

# ISP programmer
ISP = usbasp

#ISP_FLAGS = \
#            -P /dev/ttyACM0

# ==============================================================================
# tools
# ==============================================================================

CROSS := avr-
CC    := $(CROSS)gcc
LD    := $(CROSS)gcc
OBJCOPY := $(CROSS)objcopy
OBJDUMP := $(CROSS)objdump

AVRDUDE := avrdude

RM      := rm -f
RMDIR   := rm -rf
MKDIR   := mkdir -p
CP      := cp

# ==============================================================================
# commands
# ==============================================================================

TARGET = $(BIN_PATH)/$(APP)
TARGET_ELF = $(TARGET).elf
TARGET_HEX = $(TARGET).hex
TARGET_EEP = $(TARGET).eep
TARGET_LSS = $(OBJ_PATH)/$(APP).lss

# program into the flash
AVRDUDE_FLASH = -U flash:w:$(TARGET_HEX)
#AVRDUDE_EEPROM = -U eeprom:w:$(TARGET_EEP)

INSTALL = sudo $(AVRDUDE) -p $(MCU) -c $(ISP) $(ISP_FLAGS) \
                     $(AVRDUDE_FLASH) \
                     $(AVRDUDE_EEPROM)


# Define all object files.
OBJ = $(addprefix $(OBJ_PATH)/, $(SRC:.c=.o) $(ASRC:.S=.o))

# Define all listing files.
#LST = $(ASRC:.S=.lst) $(SRC:.c=.lst)


CCRUN = $(CC) -c $(CFLAGS) $(HEADERS) -MMD -MP -MT $@ -o $@ $<


# ==============================================================================
# rules
# ==============================================================================

# Default target.
all: directories $(TARGET_ELF) $(TARGET_HEX) $(TARGET_EEP) $(TARGET_LSS)

# main binary in elf format
$(TARGET_ELF): $(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) $(LIBS) -o $@


# other output from the elf
$(TARGET_HEX): $(TARGET_ELF)
	$(OBJCOPY) -O ihex -R .eeprom $< $@

$(TARGET_EEP): $(TARGET_ELF)
	$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 -O ihex $< $@

$(TARGET_LSS): $(TARGET_ELF)
	$(OBJDUMP) -h -S $< > $@


# Program the device.
install: $(TARGET).hex $(TARGET).eep
	$(INSTALL)


# create directories needed for project
directories:
	$(MKDIR) $(sort $(dir $(TARGET_ELF) $(TARGET_HEX) $(TARGET_EEP) $(TARGET_LST) $(OBJ)))

# Target: clean project.
clean:
	$(RMDIR) $(BIN_PATH)/*
	$(RMDIR) $(OBJ_PATH)/*


# Phony targets
.PHONY: all clean directories


# Implicit rules

$(foreach path, $(sort $(dir $(OBJ))), $(path)%.o): %.c
	@echo "(CC) $<"
	$(CCRUN)

#$(foreach path, $(sort $(dir $(OBJ))), $(path)%.o): %.S
#	@echo (AS) $<
#	$(ASRUN)


# Dependencies
ifneq "$(MAKECMDGOALS)" "clean"
-include $(OBJ:.o=.d)
endif
