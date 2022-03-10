#include <stdint.h>
#include <stdlib.h>
#include "domain.h"

void add_value_to_accumulator(State *some_state, uint8_t addend);

void add_value_and_carry_to_accumulator(State *some_state, uint8_t addend);

void subtract_value_from_accumulator(State *some_state, uint8_t subtrahend);

void subtract_value_and_borrow_from_accumulator(State *some_state, uint8_t subtrahend);

void ana(State* some_state, uint8_t value_to_and);

void xra(State* some_state, uint8_t value_to_xor);

void ora(State* some_state, uint8_t value_to_and);

void cmp(State* some_state, uint8_t value_to_compare);