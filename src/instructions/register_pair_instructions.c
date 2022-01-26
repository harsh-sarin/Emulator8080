#include <stdlib.h>
#include <stdint.h>
#include "domain.h"

void push_register_pair_to_stack(State* state, uint8_t register_higher, uint8_t register_lower) {
    uint8_t sp = state->sp;
    state->memory[sp-1] = register_higher;
    state->memory[sp-2] = register_lower;
    state->sp -= 2;
    state->pc += 1;
}

void push_psw(State* state) {
    uint8_t sp = state->sp;
    state->memory[sp-1] = state->a;
    uint8_t condition_bits = 0x02; //xx0x0x1x
    condition_bits |= (state->cc.s == 1 ? 0x80 : 0x00); //Sx0x0x1x
    condition_bits |= (state->cc.z == 1 ? 0x40 : 0x00); //SZ0x0x1x
    condition_bits |= (state->cc.ac == 1 ? 0x10 : 0x00); //SZ0Ac0x1x
    condition_bits |= (state->cc.p == 1 ? 0x04 : 0x00); //SZ0Ac0P1x
    condition_bits |= (state->cc.cy == 1 ? 0x01 : 0x00); //SZ0Ac0P1C
    state->memory[sp-2] = condition_bits;
    state->sp -= 2;
    state->pc += 1;
}