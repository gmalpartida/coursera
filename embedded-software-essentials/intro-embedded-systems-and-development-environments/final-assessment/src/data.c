#include "data.h"
#include "memory.h"
#include "platform.h"

uint8_t my_itoa(int32_t data, uint8_t * ptr, uint32_t base)
{
    if (data == 0){
        *ptr = '0';
        *(ptr+1) = '\0';
        return 2;
    }
    uint8_t neg_flag = 0;
    if (0 > data)
    {
        data = data * -1;
        neg_flag = 1;
    }
    uint8_t index = 0;
    while(data > 0){
        uint8_t digit = data % base;
        *(ptr+index) = digit > 9 ? 'A' + digit - 10 : '0' + digit;
        index++;
        data = data / base;
    }
    if (neg_flag){
        *(ptr+index) = '-';
        index++;
    }
    *(ptr+index) = '\0';
    my_reverse(ptr, index);    
    return index+1;
}

int32_t my_atoi(uint8_t * ptr, uint8_t digits, uint32_t base)
{
    int32_t total_value = 0;
    uint8_t multiplier = 1;
    uint8_t neg_flag = 0;
    uint8_t c = 0;
    while ((c=*ptr++))
    {
        if (c == ' ')
        {
            continue;
        }
        else if (c == '-')
        {
            neg_flag = 1;
            continue;
        }
        else
        {
            int32_t value = 0;
            if (c >= 'A' && c <= 'F')
                value += (c - 'A' + 10);
            else
                value += (c - '0');
            total_value *= base;
            total_value += value;
            multiplier *= base;
        }
    }
    return total_value * (neg_flag ? -1 : 1);

}


