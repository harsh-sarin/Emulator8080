#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "domain.h"
#include "helper.h"

bool is_even_parity_16bit(uint16_t some_data)
{
    uint8_t count = 0;
    uint16_t mask = 1;
    for (int i = 0; i < sizeof(uint16_t) * 8; i++)
    {
        if (some_data & (mask << i))
        {
            count++;
        }
    }
    return (count % 2 == 0);
}

void single_dcr_register(State *some_state, uint8_t *some_register)
{
    uint8_t unmodified_val = *some_register;
    (*some_register)--;
    uint8_t modified_val = *some_register;
    some_state->cc.z = (modified_val == 0x00);
    some_state->cc.s = ((modified_val & 0x80) == 0x80);
    some_state->cc.p = is_even_parity(modified_val);
    some_state->cc.ac = 0; // Assumption: no aux carry in decrement operation
}

void single_inr_register(State *some_state, uint8_t *some_register)
{
    uint8_t unmodified_val = *some_register;
    (*some_register)++;
    uint8_t modified_val = *some_register;
    some_state->cc.z = (modified_val == 0x00);
    some_state->cc.s = ((modified_val & 0x80) == 0x80);
    some_state->cc.p = is_even_parity(modified_val);
    // if bit at pos 3 before modification is 1 and after modificaiton is 0,
    // it implies a carry came out of pos 3 after the increment.
    if ((unmodified_val & 0x08) == 0x08 && (modified_val & 0x08) == 0x00)
    {
        some_state->cc.ac = 1;
    }
}

void single_inr_memory(State *some_state)
{
    uint16_t offset = (some_state->h << 8) | (some_state->l);
    uint16_t unmodified_val = some_state->memory[offset];
    some_state->memory[offset]++;
    uint16_t modified_val = some_state->memory[offset];
    some_state->cc.z = (modified_val == 0x0000);
    some_state->cc.s = ((modified_val & 0x8000) == 0x8000);
    some_state->cc.p = is_even_parity_16bit(modified_val);
    // if bit at pos 3 before modification is 1 and after modificaiton is 0,
    // it implies a carry came out of pos 3 after the increment.
    if ((unmodified_val & 0x0008) == 0x0008 && (modified_val & 0x0008) == 0x0000)
    {
        some_state->cc.ac = 1;
    }
}

void single_dcr_memory(State *some_state)
{
    uint16_t offset = (some_state->h << 8) | (some_state->l);
    uint16_t unmodified_val = some_state->memory[offset];
    some_state->memory[offset]--;
    uint16_t modified_val = some_state->memory[offset];
    some_state->cc.z = (modified_val == 0x0000);
    some_state->cc.s = ((modified_val & 0x8000) == 0x8000);
    some_state->cc.p = is_even_parity_16bit(modified_val);
    some_state->cc.ac = 0; // Assumption: no aux carry in decrement operation
}