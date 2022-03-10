#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

bool is_even_parity(uint8_t some_data)
{
    uint8_t count = 0, mask = 1;
    for (int i = 0; i < sizeof(uint8_t) * 8; i++)
    {
        if (some_data & (mask << i))
        {
            count++;
        }
    }
    return (count % 2 == 0);
}