#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "domain.h"

bool is_even_parity(uint8_t some_data);

void update_condition_codes_due_to_add(State *some_state, uint16_t sum, uint8_t addend);