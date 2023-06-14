#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "domain.h"

uint16_t get_memory_location(uint8_t lower, uint8_t higher) {
    return (((uint16_t) higher) << 8) | lower; 
}

void sta(State* state, uint8_t lower_order, uint8_t higher_order) {
    uint16_t mem_location = get_memory_location(lower_order, higher_order);
    state->memory[mem_location] = state->a;
    state->pc += 3;
}

void lda(State* state, uint8_t lower_order, uint8_t higher_order) {
    uint16_t mem_location = get_memory_location(lower_order, higher_order);
    state->a = state->memory[mem_location];
    state->pc += 3;
}

void shld(State* state, uint8_t lower_order, uint8_t higher_order) {
    uint16_t mem_location = get_memory_location(lower_order, higher_order);
    state->memory[mem_location] = state->l;
    state->memory[mem_location+1] = state->h;
    state->pc += 3;
}