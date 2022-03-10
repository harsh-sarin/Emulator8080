#include <stdint.h>
#include <stdlib.h>
#include "domain.h"
#include "helper.h"


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
