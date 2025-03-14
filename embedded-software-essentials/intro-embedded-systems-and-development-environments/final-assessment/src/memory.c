/******************************************************************************
 * Copyright (C) 2017 by Alex Fosdick - University of Colorado
 *
 * Redistribution, modification or use of this software in source or binary
 * forms is permitted as long as the files maintain this copyright. Users are 
 * permitted to modify this and use it to learn about the field of embedded
 * software. Alex Fosdick and the University of Colorado are not liable for any
 * misuse of this material. 
 *
 *****************************************************************************/
/**
 * @file memory.c
 * @brief Abstraction of memory read and write operations
 *
 * This implementation file provides an abstraction of reading and
 * writing to memory via function calls. There is also a globally
 * allocated buffer array used for manipulation.
 *
 * @author Alex Fosdick
 * @date April 1 2017
 *
 */
#include "memory.h"
#include <stdlib.h>
#include <stdio.h>

/***********************************************************
 Function Definitions
***********************************************************/
void set_value(char * ptr, unsigned int index, char value){
  ptr[index] = value;
}

void clear_value(char * ptr, unsigned int index){
  set_value(ptr, index, 0);
}

char get_value(char * ptr, unsigned int index){
  return ptr[index];
}

void set_all(char * ptr, char value, unsigned int size){
  unsigned int i;
  for(i = 0; i < size; i++) {
    set_value(ptr, i, value);
  }
}

void clear_all(char * ptr, unsigned int size){
  set_all(ptr, 0, size);
}

uint8_t * my_memmove(uint8_t * src, uint8_t * dst, size_t length)
{
    if (dst > src){
        uint8_t * end_src = src + length - 1;
        uint8_t * end_dst = dst + length - 1;

        while (end_src >= src){
            *end_dst = *end_src;
            end_dst--;
            end_src--;
            
        }
    }
    else{
        my_memcopy(src, dst, length);
    }
   return dst; 
}

uint8_t * my_memcopy(uint8_t * src, uint8_t * dst, size_t length)
{
    uint8_t * tmp_dst = dst;
    while (length>0){
        *tmp_dst = *src;
        src++;
        tmp_dst++;
        length--;
    }
    return dst;
}

uint8_t * my_memset(uint8_t * src, size_t length, uint8_t value)
{
    uint8_t * tmp_src = src;
    while (length > 0)
    {
        *tmp_src = value;
        tmp_src++;
        length--;
    }
    return src;
}

uint8_t * my_memzero(uint8_t * src, size_t length)
{
    return my_memset(src, length, 0);
}

uint8_t * my_reverse(uint8_t * src, size_t length)
{
    uint8_t * tmp_src = src;
    uint8_t * end = src + length - 1;

    while (tmp_src < end)
    {
        uint8_t tmp = *tmp_src;
        *tmp_src = *end;
        *end = tmp;
        tmp_src++;
        end--;
    }
    return src;
}

int32_t * reserve_words(size_t length)
{
    return malloc(sizeof(uint32_t) * length);
}

void free_words(uint32_t * src)
{
    free(src);
}



