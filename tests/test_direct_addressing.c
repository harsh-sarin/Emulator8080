#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "emulator.h"
#include "test_commons.h"


bool test_sta() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0x32; // STA, 0x0009
    test_state.memory[1] = 0x09;
    test_state.memory[2] = 0x00;
    test_state.a = 0x14;

    Emulate8080(&test_state);
    
    int result = 1;
    result = result & assert_equals(test_state.memory[9], 0x14, "Contents of accumulator were not copied to desired location");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    cleanup_test_state(test_state);
    return result;
}

bool test_lda() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0x3a; // LDA, 0x0009
    test_state.memory[1] = 0x09;
    test_state.memory[2] = 0x00;
    test_state.memory[9] = 0xF4;
    test_state.a = 0xFF;

    Emulate8080(&test_state);
    
    int result = 1;
    result = result & assert_equals(test_state.a, 0xF4, "Contents from desired mem location was not copied to accumulator");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    cleanup_test_state(test_state);
    return result;
}

bool test_shld() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0x22; // SHLD, 0x0008
    test_state.memory[1] = 0x08;
    test_state.memory[2] = 0x00;
    test_state.memory[8] = 0xFF;
    test_state.memory[9] = 0xFF;
    test_state.h = 0x12;
    test_state.l = 0x4C;

    Emulate8080(&test_state);
    
    int result = 1;
    result = result & assert_equals(test_state.memory[8], 0x4C, "Contents of register L were not copied to desired mem location");
    result = result & assert_equals(test_state.memory[9], 0x12, "Contents of register H were not copied to desired mem location");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    cleanup_test_state(test_state);
    return result;
}

int main(int argc, char** argv){
    run_test_func(test_sta, "Test STA");
    run_test_func(test_lda, "Test LDA");
    run_test_func(test_shld, "Test SHLD");
    return 0;
}