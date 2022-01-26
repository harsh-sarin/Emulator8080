#include <stdlib.h>
#include <stdint.h>
#include "domain.h"

void push_register_pair_to_stack(State* state, uint8_t register_higher, uint8_t register_lower);

void push_psw(State* state);