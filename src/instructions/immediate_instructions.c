#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "domain.h"
#include "helper.h"
#include "arithmetic_and_logical.h"

void lxi(State* state, uint8_t* register_higher, uint8_t* register_lower, uint8_t data_higher, uint8_t data_lower) {
    *register_higher = data_higher;
    *register_lower = data_lower;
    state->pc += 3;
}

void lxi_sp(State* state, uint8_t data_higher, uint8_t data_lower) {
    state->sp = (((uint16_t) data_higher) << 8) | ((uint16_t) data_lower);
    state->pc += 3;
}

void mvi(State* state, uint8_t* target_register, uint8_t data) {
    *target_register = data;
    state->pc += 2;
}

void mvi_m(State* state, uint8_t data) {
    uint16_t location = (((uint16_t) state->h) << 8) | ((uint16_t) state->l);
    state->memory[location] = data;
    state->pc += 2;
}

void xri(State* state, uint8_t data) {
    xra(state, data);
    state->pc += 2;
}

void ori(State* state, uint8_t data) {
    ora(state, data);
    state->pc += 2;
}

void cpi(State* state, uint8_t data) {
    cmp(state, data);
    state->pc += 2;
}