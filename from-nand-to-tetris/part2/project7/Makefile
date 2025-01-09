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

SIZE = size

CC = gcc

LD = ld
TARGET = bin/VMTranslator
LDFLAGS= -Xlinker -Map=$(TARGET).map

CFLAGS = $(INCLUDES) -Wall -Werror -g -O0 -std=c99 

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
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@
	$(SIZE) --totals --common $(TARGET)

.PHONY: clean
clean: 
	rm -f src/*.o $(TARGET) $(TARGET).map *.i *.asm

