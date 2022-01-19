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

bool test_rlc() {
    State test_state = create_test_state(sizeof(uint8_t)*2);
    test_state.memory[0] = 0x07;
    test_state.a = 0xF3;
    test_state.cc.ac = 0;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.a, 0xE7, "RLC operation did not work as expected");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 1, "Carry bit was not set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");

    cleanup_test_state(test_state);
    return result;
}

bool test_rrc() {
    State test_state = create_test_state(sizeof(uint8_t)*2);
    test_state.memory[0] = 0x0F;
    test_state.a = 0xF2;
    test_state.cc.ac = 0;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.a, 0x79, "RRC operation did not work as expected");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");

    cleanup_test_state(test_state);
    return result;
}

bool test_ral() {
    State test_state = create_test_state(sizeof(uint8_t)*2);
    test_state.memory[0] = 0x17;
    test_state.a = 0xB5;
    test_state.cc.cy = 1;
    test_state.cc.ac = 0;
    test_state.cc.z = 0;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.a, 0x6B, "RAL operation did not work as expected");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 1, "Carry bit was not set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");

    cleanup_test_state(test_state);
    return result;
}

bool test_rar() {
    State test_state = create_test_state(sizeof(uint8_t)*2);
    test_state.memory[0] = 0x1F;
    test_state.a = 0x6A;
    test_state.cc.cy = 1;
    test_state.cc.ac = 0;
    test_state.cc.z = 0;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.a, 0xB5, "RAR operation did not work as expected");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");

    cleanup_test_state(test_state);
    return result;
}

int main(int argc, char** argv){
    run_test_func(test_rlc, "Test RLC");
    run_test_func(test_rrc, "Test RRC");
    run_test_func(test_ral, "Test RAL");
    run_test_func(test_rar, "Test RAR");
    return 0;
}