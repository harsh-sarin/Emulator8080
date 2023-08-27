#include <stdlib.h>
#include <stdint.h>
#include "domain.h"

void call(State* state, uint8_t lower_address, uint8_t higher_address);

void cnc(State* state, uint8_t lower_address, uint8_t higher_address);

void cz(State* state, uint8_t lower_address, uint8_t higher_address);

void cnz(State* state, uint8_t lower_address, uint8_t higher_address);

void cm(State* state, uint8_t lower_address, uint8_t higher_address);

void cpe(State* state, uint8_t lower_address, uint8_t higher_address);