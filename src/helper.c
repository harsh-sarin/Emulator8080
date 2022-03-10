#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "domain.h"

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

void update_condition_codes_due_to_add(State *some_state, uint16_t sum, uint8_t addend) {
    some_state->cc.cy = (sum > 0xFF);
    some_state->cc.z = ((sum & 0xFF) == 0);
    some_state->cc.s = ((sum & 0x80) == 0x80);
    some_state->cc.p = is_even_parity((sum & 0xFF));
    bool is_acc_bit_pos_3_set = ((some_state->a & 0x08) == 0x08);
    bool is_addend_bit_pos_3_set = ((addend & 0x08) == 0x08);
    // If both the operands have the 3rd bit set, there is bound to be a carry out of that position when
    // these bits are added.
    // If at least one operand has the 3rd bit set, and the result of adding these bits yields 0, it implies that 
    // a carry from adding lower order bits propagated to bit 3, resuling in carry out of bit 3 as well.
    // 
    if ((is_acc_bit_pos_3_set && is_addend_bit_pos_3_set) || 
        ((is_acc_bit_pos_3_set || is_addend_bit_pos_3_set) && (sum & 0x08) == 0)){
        some_state->cc.ac = 1;
    } else {
        some_state->cc.ac = 0;
    }
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