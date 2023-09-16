#include <stdint.h>
#include <stdbool.h>
#include "domain.h"


void ret(State* state) {
    uint16_t sp_address = state->sp;
    state->pc = (((uint16_t) state->memory[sp_address+1]) << 8) | ((uint16_t) state->memory[sp_address]);
    state->memory[sp_address] = 0x00;
    state->memory[sp_address+1] = 0x00;
    state->sp += 2;
}


void return_if_true(State* state, bool condition) {
    condition ? ret(state) : state->pc++;
}

void rc(State* state) {
    return_if_true(state, state->cc.cy == 1);
}

void rnc(State* state) {
    return_if_true(state, state->cc.cy == 0);
}

void rz(State* state) {
    return_if_true(state, state->cc.z == 1);
}

void rnz(State* state) {
    return_if_true(state, state->cc.z == 0);
}

void rm(State* state) {
    return_if_true(state, state->cc.s == 1);
}

void rp(State* state) {
    return_if_true(state, state->cc.s == 0);
}

void rpo(State* state) {
    return_if_true(state, state->cc.p == 0);
}