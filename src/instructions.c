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

void add_value_to_accumulator(State *some_state, uint8_t addend) {
    uint16_t sum = (uint16_t) some_state->a + (uint16_t) addend;
    update_condition_codes_due_to_add(some_state, sum, addend);
    some_state->a = sum & 0xFF;
}

void add_value_and_carry_to_accumulator(State *some_state, uint8_t addend) {
    uint16_t sum = (uint16_t) some_state->a + (uint16_t) addend + (some_state->cc.cy == 1 ? 0x0001 : 0x0000);
    update_condition_codes_due_to_add(some_state, sum, addend);
    some_state->a = sum & 0xFF;
}

void update_condition_codes_due_to_subtract(State *some_state, uint16_t difference, uint8_t subtrahend) {
    some_state->cc.cy = !(difference > 0xFF);
    some_state->cc.z = ((difference & 0xFF) == 0);
    some_state->cc.s = ((difference & 0x80) == 0x80);
    some_state->cc.p = is_even_parity((difference & 0xFF));
    bool is_acc_bit_pos_3_set = ((some_state->a & 0x08) == 0x08);
    bool is_subtrahend_bit_pos_3_set = ((subtrahend & 0x08) == 0x08);
    if ((is_acc_bit_pos_3_set && is_subtrahend_bit_pos_3_set) || 
        ((is_acc_bit_pos_3_set || is_subtrahend_bit_pos_3_set) && (difference & 0x08) == 0)){
        some_state->cc.ac = 1;
    } else {
        some_state->cc.ac = 0;
    }
}

void subtract_value_from_accumulator(State *some_state, uint8_t subtrahend) {
    uint8_t subtrahend_two_complement = (~subtrahend) + 0x01;
    uint16_t difference = (uint16_t) some_state->a + (uint16_t) subtrahend_two_complement;
    update_condition_codes_due_to_subtract(some_state, difference, subtrahend_two_complement);
    some_state->a = difference & 0xFF;
}

void subtract_value_and_borrow_from_accumulator(State *some_state, uint8_t subtrahend) {
    uint8_t subtrahend_with_borrow_added = (subtrahend + (some_state->cc.cy == 1 ? 0x01 : 0x00)) & 0xFF;
    uint8_t subtrahend_two_complement = (~subtrahend_with_borrow_added) + 0x01;
    uint16_t difference = (uint16_t) some_state->a + (uint16_t) subtrahend_two_complement;
    update_condition_codes_due_to_subtract(some_state, difference, subtrahend_two_complement);
    some_state->a = difference & 0xFF;
}

void ana(State* some_state, uint8_t value_to_and) {
    some_state->a &= value_to_and;
    some_state->cc.cy = 0;
    some_state->cc.z = ((some_state->a & 0xFF) == 0);
    some_state->cc.s = ((some_state->a & 0x80) == 0x80);
    some_state->cc.p = is_even_parity(some_state->a);
}

void xra(State* some_state, uint8_t value_to_xor) {
    some_state->a ^= value_to_xor;
    some_state->cc.cy = 0;
    some_state->cc.z = ((some_state->a & 0xFF) == 0);
    some_state->cc.s = ((some_state->a & 0x80) == 0x80);
    some_state->cc.p = is_even_parity(some_state->a);
}

void ora(State* some_state, uint8_t value_to_and) {
    some_state->a |= value_to_and;
    some_state->cc.cy = 0;
    some_state->cc.z = ((some_state->a & 0xFF) == 0);
    some_state->cc.s = ((some_state->a & 0x80) == 0x80);
    some_state->cc.p = is_even_parity(some_state->a);
}

void cmp(State* some_state, uint8_t value_to_compare) {
    uint8_t subtrahend_two_complement = (~value_to_compare) + 0x01;
    uint16_t difference = (uint16_t) some_state->a + (uint16_t) subtrahend_two_complement;
    update_condition_codes_due_to_subtract(some_state, difference, value_to_compare);
}
