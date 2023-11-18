#include <stdint.h>
#include "domain.h"

void in(State* state, uint8_t device_number) {
    //TODO: connect to input device (emulate)
    state->pc += 2;
}

void out(State* state, uint8_t device_number) {
    //TODO: connect to output device (emulate)
    state->pc += 2;
}