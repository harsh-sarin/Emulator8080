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
    result = result & assert_equals(test_state.memory[9], 0x14, "Contents of register A were not copied to desired location");
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
    return 0;
}