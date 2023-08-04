#include <stdlib.h>
#include <stdint.h>
#include "domain.h"

void pchl(State* state);

void jmp(State* state, uint8_t lower_address, uint8_t higher_address);

void jc(State* state, uint8_t lower_address, uint8_t higher_address);