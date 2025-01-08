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

# Add your Source files to this variable

SRCS =\
	src/main.c \
	src/scanner.c \
	src/token.c \
	src/lexer.c \
    src/parser.c \
    src/symbol_table.c

# Add your include paths to this variable

INCLUDES =\
	-Iinclude

