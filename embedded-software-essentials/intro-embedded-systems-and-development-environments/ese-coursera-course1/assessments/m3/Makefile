#******************************************************************************
# Copyright (C) 2017 by Alex Fosdick - University of Colorado
#
# Redistribution, modification or use of this software in source or binary
# forms is permitted as long as the files maintain this copyright. Users are 
# permitted to modify this and use it to learn about the field of embedded
# software. Alex Fosdick and the University of Colorado are not liable for any
# misuse of this material. 
#
#*****************************************************************************

#------------------------------------------------------------------------------
# <Put a Description Here>
#
# Use: make [TARGET] [PLATFORM-OVERRIDES]
#
# Build Targets:
#      <Put a description of the supported targets here>
#
# Platform Overrides:
#      <Put a description of the supported Overrides here
#
#------------------------------------------------------------------------------
include sources.mk

# Platform Overrides
PLATFORM = HOST

# Architectures Specific Flags
LINKER_FILE = msp432p401r.lds
CPU = cortex-m4
ARCH = armv7e-m
SPECS = nosys.specs

# Compiler Flags and Defines

ifeq ($(PLATFORM), HOST)
SIZE = size
NM = nm
else
SIZE = /opt/arm-none-eabi/bin/arm-none-eabi-size
NM = /opt/arm-none-eabi/bin/arm-none-eabi-nm
endif

ifeq ($(PLATFORM), HOST)
CC = gcc
else
CC = /opt/arm-none-eabi/bin/arm-none-eabi-gcc
endif

ifeq ($(PLATFORM), HOST)
LD = ld
else
LD = /opt/arm-none-eabi/bin/arm-none-eabi-ld 
endif
TARGET = main
ifeq ($(PLATFORM), HOST)
LDFLAGS= -Xlinker -Map=$(TARGET).map
else
LDFLAGS = -T$(LINKER_FILE) -Xlinker -Map=$(TARGET).map
endif

CFLAGS = $(INCLUDES) -D$(PLATFORM) -Wall -Werror -g -O0 -std=c99
ifeq ($(PLATFORM), MSP432)
CFLAGS += -mcpu=$(CPU) -mthumb -march=$(ARCH) -mfloat-abi=hard -mfpu=fpv4-sp-d16 --specs=$(SPECS)
endif

CPPFLAGS = 

OBJS = $(SRCS:.c=.o)

%.o : %.c
	$(CC) -c $< $(CFLAGS) -o $@

%.i : %.c
	$(CC) -E $< $(CFLAGS) -o $@

%.asm : %.c
	$(CC) -S $< $(CFLAGS) -o $@

.PHONY: compile-all
compile-all : $(OBJS) 

.PHONY: build
build: all

.PHONY: all
all: $(TARGET).out

$(TARGET).out: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@
	$(SIZE) --totals --common $(TARGET).out
	$(NM) $(TARGET).out > $(TARGET).sym

.PHONY: clean
clean: 
	rm -f *.o $(TARGET).out $(TARGET).map *.i *.asm

