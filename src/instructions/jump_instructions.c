#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "domain.h"

void pchl(State* state) {
    state->pc = (((uint16_t) state->h) << 8) | ((uint16_t) state->l);
}

void jmp(State* state, uint8_t lower_address, uint8_t higher_address) {
    state->pc = (((uint16_t) higher_address) << 8) | ((uint16_t) lower_address);
}

void jc(State* state, uint8_t lower_address, uint8_t higher_address) {
    if (state->cc.cy) {
        jmp(state, lower_address, higher_address);
    } else {
        state->pc += 3;
    }
}