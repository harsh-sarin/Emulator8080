#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "domain.h"

void sta(State* state, uint8_t lower_order, uint8_t higher_order) {
    uint16_t mem_location = (((uint16_t)higher_order) << 8) | lower_order;
    state->memory[mem_location] = state->a;
    state->pc += 3;
}