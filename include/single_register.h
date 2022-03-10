#include <stdint.h>
#include <stdlib.h>
#include "domain.h"

void single_dcr_register(State *some_state, uint8_t *some_register);

void single_inr_register(State *some_state, uint8_t *some_register);

void single_inr_memory(State *some_state);

void single_dcr_memory(State *some_state);