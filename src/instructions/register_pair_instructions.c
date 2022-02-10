#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
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

void pop_from_stack_to_register_pair(State *state, uint8_t *register_higher, uint8_t *register_lower) {
    *register_lower = state->memory[state->sp];
    *register_higher = state->memory[state->sp+1];
    state->sp += 2;
    state->pc += 1;
}

void pop_psw(State *state) {
    state->a = state->memory[state->sp+1];
    uint8_t condition_bits = state->memory[state->sp];
    state->cc.s = (condition_bits & 0x80) == 0x80;
    state->cc.z = (condition_bits & 0x40) == 0x40;
    state->cc.ac = (condition_bits & 0x10) == 0x10;
    state->cc.p = (condition_bits & 0x04) == 0x04;
    state->cc.cy = (condition_bits & 0x01) == 0x01;
    state->sp += 2;
    state->pc += 1;
}

void dad(State* state, uint8_t higher_order, uint8_t lower_order) {
    uint16_t addend1 = (((uint16_t) state->h) << 8) + ((uint16_t) state->l);
    uint16_t addend2 = (((uint16_t) higher_order) << 8) + ((uint16_t) lower_order);
    uint32_t sum = addend1 + addend2;
    state->cc.cy = (sum > 0xFFFF);
    state->h = ((sum & 0xFF00) >> 8);
    state->l = sum & 0xFF;
    state->pc += 1;
}

void inx(State* state, uint8_t* register_higher, uint8_t* register_lower) {
    (*register_lower)++;
    if (*register_lower == 0x00) {
        (*register_higher)++;
    }
    state->pc += 1;
}

void inx_sp(State* state) {
    state->sp++;
    state->pc += 1;
}

void dcx(State* state, uint8_t* register_higher, uint8_t* register_lower) {
    (*register_lower)--;
    if (*register_lower == 0xFF) {
        (*register_higher)--;
    }
    state->pc += 1;
}

void dcx_sp(State* state) {
    state->sp--;
    state->pc += 1;
}

void xchg(State* state) {
    uint8_t temp_d = state->d;
    uint8_t temp_e = state->e;
    state->d = state->h;
    state->e = state->l;
    state->h = temp_d;
    state->l = temp_e;
    state->pc += 1;
}

void xthl(State* state) {
    uint8_t temp_h = state->h;
    uint8_t temp_l = state->l;
    state->h = state->memory[state->sp+1];
    state->l = state->memory[state->sp];
    state->memory[state->sp+1] = temp_h;
    state->memory[state->sp] = temp_l;
    state->pc += 1;
}

void sphl(State* state) {
    state->sp = (((uint16_t) state->h) << 8) | ((uint16_t) state->l);
    state->pc += 1;
}