#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "domain.h"

bool is_even_parity(uint8_t some_data);

bool is_even_parity_16bit(uint16_t some_data);

void single_dcr_register(State *some_state, uint8_t *some_register);

void single_inr_register(State *some_state, uint8_t *some_register);

void single_inr_memory(State *some_state);

void single_dcr_memory(State *some_state);

void update_condition_codes_due_to_add(State *some_state, uint16_t sum, uint8_t addend);

void add_value_to_accumulator(State *some_state, uint8_t addend);

void add_value_and_carry_to_accumulator(State *some_state, uint8_t addend);

void update_condition_codes_due_to_subtract(State *some_state, uint16_t difference, uint8_t subtrahend);

void subtract_value_from_accumulator(State *some_state, uint8_t subtrahend);

void subtract_value_and_borrow_from_accumulator(State *some_state, uint8_t subtrahend);

void ana(State* some_state, uint8_t value_to_and);

void xra(State* some_state, uint8_t value_to_xor);

void ora(State* some_state, uint8_t value_to_and);

void cmp(State* some_state, uint8_t value_to_compare);