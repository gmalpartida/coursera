#include "data.h"
#include <stdio.h>
#include <stdint.h>

int main(void)
{
    char ptr[20];
    for (uint8_t i = 0; i < 100; i++)
    {
        uint8_t len = my_itoa(i, ptr, 7); 
        printf("value %d, text %s, len %d\n", i, ptr, len);
    }
    return 0;
}
