#include "data.h"

uint8_t my_itoa(int32_t data, uint8_t * ptr, uint32_t base)
{
    if (data == 0){
        *ptr = '0';
        *(ptr+1) = '\0';
        return 2;
    }
    uint8_t index = 0;
    while(data > 0){
        uint8_t digit = data % base;
        uint8_t tmp = index;
        while (tmp > 0)
        {
            *(ptr+tmp) = *(ptr+tmp-1);
            tmp--;
        }
        *(ptr) = digit > 9 ? 'A' + digit - 10 : '0' + digit;
        index++;
        data = data / base;
    }
    *(ptr+index) = '\0';
    return index+1;
}

int32_t my_atoi(uint8_t * ptr, uint8_t digits, uint32_t base)
{
    return 0;
}
