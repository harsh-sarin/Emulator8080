#include <stdlib.h>
#include <stdint.h>
#include "domain.h"

void lxi(State* state, uint8_t* register_higher, uint8_t* register_lower, uint8_t data_higher, uint8_t data_lower);

void lxi_sp(State* state, uint8_t data_higher, uint8_t data_lower);

void mvi(State* state, uint8_t* target_register, uint8_t data);

void mvi_m(State* state, uint8_t data);

void xri(State* state, uint8_t data);

void ori(State* state, uint8_t data);

void cpi(State* state, uint8_t data);