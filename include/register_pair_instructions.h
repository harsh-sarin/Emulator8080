#include <stdlib.h>
#include <stdint.h>
#include "domain.h"

void push_register_pair_to_stack(State* state, uint8_t register_higher, uint8_t register_lower);

void push_psw(State* state);

void pop_from_stack_to_register_pair(State* state, uint8_t* register_higher, uint8_t* register_lower);

void pop_psw(State* state);

void dad(State* state, uint8_t higher_order, uint8_t lower_order);

void inx(State* state, uint8_t* register_higher, uint8_t* register_lower);

void inx_sp(State* state);

void dcx(State* state, uint8_t* register_higher, uint8_t* register_lower);

void dcx_sp(State* state);
