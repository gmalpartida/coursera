#ifndef DATA_H
#define DATA_H

#include <stdint.h>

#define BASE_10 10
#define BASE_16 16

uint8_t my_itoa(int32_t data, uint8_t * ptr, uint32_t base);

int32_t my_atoi(uint8_t * ptr, uint8_t digits, uint32_t base);


#endif
