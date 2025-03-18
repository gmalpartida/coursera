CURRENT_DIR = $(shell pwd)
INCLUDES = 

CC = gcc
CFLAGS = -g -Wall
SRC = *.c
BIN = JackCompiler
LIBS = 

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $^ $(CFLAGS) $(INCLUDES) $(LIBS) -o $@

clean: 
	rm -f *.o $(BIN)
