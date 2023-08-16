#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "domain.h"

void pchl(State* state) {
    state->pc = (((uint16_t) state->h) << 8) | ((uint16_t) state->l);
}

void jmp(State* state, uint8_t lower_address, uint8_t higher_address) {
    state->pc = (((uint16_t) higher_address) << 8) | ((uint16_t) lower_address);
}

void jump_if_true(State* state, uint8_t lower_address, uint8_t higher_address, bool condition) {
    if (condition) {
        jmp(state, lower_address, higher_address);
    } else {
        state->pc += 3;
    }
}

void jc(State* state, uint8_t lower_address, uint8_t higher_address) {
    jump_if_true(state, lower_address, higher_address, state->cc.cy == 1);
}

void jnc(State* state, uint8_t lower_address, uint8_t higher_address) {
    jump_if_true(state, lower_address, higher_address, state->cc.cy == 0);
}

void jz(State* state, uint8_t lower_address, uint8_t higher_address) {
    jump_if_true(state, lower_address, higher_address, state->cc.z == 1);
}

void jnz(State* state, uint8_t lower_address, uint8_t higher_address) {
    jump_if_true(state, lower_address, higher_address, state->cc.z == 0);
}

void jm(State* state, uint8_t lower_address, uint8_t higher_address) {
    jump_if_true(state, lower_address, higher_address, state->cc.s == 1);   
}

void jp(State* state, uint8_t lower_address, uint8_t higher_address) {
    jump_if_true(state, lower_address, higher_address, state->cc.s == 0);
}

void jpo(State* state, uint8_t lower_address, uint8_t higher_address) {
    jump_if_true(state, lower_address, higher_address, state->cc.p == 0);
}