#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "domain.h"
#include "jump_instructions.h"

void call(State* state, uint8_t lower_address, uint8_t higher_address) {
    uint16_t next_instruction_address = state->pc + 3;
    state->memory[state->sp-1] = ((next_instruction_address & 0xff00) >> 8);
    state->memory[state->sp-2] = next_instruction_address & 0xff;
    state->sp -= 2;
    
    jmp(state, lower_address, higher_address);
}

void call_if_true(State* state, uint8_t lower_address, uint8_t higher_address, bool condition) {
    if (condition) {
        call(state, lower_address, higher_address);
    } else {
        state->pc += 3;
    }
}

void cnc(State* state, uint8_t lower_address, uint8_t higher_address) {
    call_if_true(state, lower_address, higher_address, state->cc.cy == 0);
}

void cz(State* state, uint8_t lower_address, uint8_t higher_address) {
    call_if_true(state, lower_address, higher_address, state->cc.z == 0);
}

void cnz(State* state, uint8_t lower_address, uint8_t higher_address) {
    call_if_true(state, lower_address, higher_address, state->cc.z == 1);
}

void cm(State* state, uint8_t lower_address, uint8_t higher_address) {
    call_if_true(state, lower_address, higher_address, state->cc.s == 1);
}

void cpe(State* state, uint8_t lower_address, uint8_t higher_address) {
    call_if_true(state, lower_address, higher_address, state->cc.p == 1);
}