#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "jump_instructions.h"
#include "domain.h"


void update_inte(State* state, bool flag) {
    state->inte = flag;
    state->pc += 1;
}

void rst(State* state, uint8_t content) {
    if (state->inte) {
        state->memory[state->sp-1] = ((state->pc & 0xff00) >> 8);
        state->memory[state->sp-2] = state->pc & 0xff;
        state->sp -= 2;

        uint8_t routine_address = content & 0x38;
        jmp(state, routine_address, 0x00);
    } else {
        state->exception = true;
        strcpy(state->exception_message, "rst inte disabled");
    }
}