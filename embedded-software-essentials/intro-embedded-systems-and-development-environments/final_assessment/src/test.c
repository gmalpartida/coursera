#include "data.h"
#include "memory.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    uint8_t * ptr = malloc(sizeof(uint8_t) * 20);

    int16_t value = 79;
    uint8_t base = 7;
    uint8_t len = my_itoa(value, ptr, base); 
    value = my_atoi(ptr, len, base);
    printf ("base %d, value %d, text %s, len %d\n", base, value, ptr, len);

    base = 3;
    len = my_itoa(value, ptr, base); 
    value = my_atoi(ptr, len, base);
    printf ("base %d, value %d, text %s, len %d\n", base, value, ptr, len);

    base = 9;
    len = my_itoa(value, ptr, base);
    value = my_atoi(ptr, len, base);
    printf ("base %d, value %d, text %s, len %d\n", base, value, ptr, len);

    base = 16;
    len = my_itoa(value, ptr, base);
    value = my_atoi(ptr, len, base);
    printf ("base %d, value %d, text %s, len %d\n", base, value, ptr, len);

    base = 16;
    value = -52;
    len = my_itoa(value, ptr, base);
    value = my_atoi(ptr, len, base);
    printf ("base %d, value %d, text %s, len %d\n", base, value, ptr, len);
    
    value = -2352;
    base = 10;
    len = my_itoa(value, ptr, base);
    value = my_atoi(ptr, len, base);
    printf ("base %d, value %d, text %s, len %d\n", base, value, ptr, len);

    free(ptr);    

    uint8_t size = 10;
    uint8_t * ptra = (uint8_t*)malloc(sizeof(uint8_t) * size);

    uint8_t * ptrb = (uint8_t*)malloc(sizeof(uint8_t) * size * 2);

    for (uint8_t i = 0; i < size; i++)
    {
        ptra[i] = i + 1;
    }

    my_memcopy(ptra, ptrb, size);
    
    for (uint8_t i = 0; i < size; i++)
        printf("%d\n", ptrb[i]);

    return 0;
}
