#include <stdlib.h>
#include <stdint.h>
#include "domain.h"

void pchl(State* state);

void jmp(State* state, uint8_t lower_address, uint8_t higher_address);

void jc(State* state, uint8_t lower_address, uint8_t higher_address);

void jnc(State* state, uint8_t lower_address, uint8_t higher_address);

void jz(State* state, uint8_t lower_address, uint8_t higher_address);

void jnz(State* state, uint8_t lower_address, uint8_t higher_address);

void jm(State* state, uint8_t lower_address, uint8_t higher_address);

void jp(State* state, uint8_t lower_address, uint8_t higher_address);

void jpo(State* state, uint8_t lower_address, uint8_t higher_address);