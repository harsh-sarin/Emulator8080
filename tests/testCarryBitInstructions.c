#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "emulator.h"

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

    return test_state;
}

void cleanup_test_state(State test_state) {
    free(test_state.memory);
    test_state.memory = NULL;
}

bool test_complement_carry() {
    State test_state = create_test_state(sizeof(uint8_t) * 3);
    test_state.memory[0] = 0x3F; // CMC
    test_state.memory[1] = 0x3F; // CMC
    test_state.cc.cy = 0;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.cc.cy, 1, "Carry bit was not complemented to 1");
    
    Emulate8080(&test_state);
    result = result & assert_equals(test_state.cc.cy, 0, "Carru bit was not complemented to 0");
    result = result & assert_equals(test_state.pc, 0x02, "PC was not incremented");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_set_carry() {
    State test_state = create_test_state(sizeof(uint8_t) * 2);
    test_state.memory[0] = 0x37; // STC
    test_state.cc.cy = 0;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.cc.cy, 1, "Carry bit was not set to 1");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}


int main(int argc, char** argv){
    run_test_func(test_complement_carry, "Test CMC");
    run_test_func(test_complement_carry, "Test CMC");
    return 0;
}