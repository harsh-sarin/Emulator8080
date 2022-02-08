#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "domain.h"

void red() {
    printf("\033[1;31m");
}

void reset() {
    printf("\033[0m");
}

bool assert_equals(uint8_t e1, uint8_t e2, char error_message[]) {
    if (e1 != e2) {
        printf("%s\n", error_message);
        return false;
    }
    return true;
}

void run_test_func(bool (*test_func)(), char test_name[]) {
    printf("Testing %s\n", test_name);
    if (test_func()) {
        printf("Test passed\n");
    } else {
        red();
        printf("Test failed\n");
        reset();
    }
    printf("------\n");
}

State create_test_state(uint8_t mem_size) {
    State test_state;

    ConditionCodes condition_codes;
    test_state.cc = condition_codes;

    uint8_t* mem;
    mem = (uint8_t *) malloc(mem_size);
    test_state.memory = mem;
    test_state.pc = 0x00;

    test_state.cc.p = 0;
    test_state.cc.z = 0;
    test_state.cc.cy = 0;
    test_state.cc.ac = 0;
    test_state.cc.s = 0;

    return test_state;
}

void cleanup_test_state(State test_state) {
    free(test_state.memory);
    test_state.memory = NULL;
}