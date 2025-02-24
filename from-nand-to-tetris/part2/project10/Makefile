include sources.mk

SIZE = size

CC = gcc

LD = ld
TARGET = bin/JackAnalyzer
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

.PHONY: all
all: $(TARGET)

.PHONY: compile-all
compile-all : $(OBJS) 

.PHONY: build
build: all

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@
	$(SIZE) --totals --common $(TARGET)

.PHONY: clean
clean: 
	rm -f src/*.o $(TARGET) $(TARGET).map *.i *.asm

