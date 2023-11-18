#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "emulator.h"
#include "test_commons.h"


bool test_in() {
    State test_state = create_test_state(sizeof(uint8_t) * 5);
    test_state.memory[0] = 0xdb; // IN

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x02, "Program counter was not incremented");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");

    cleanup_test_state(test_state);
    return result;
}

bool test_out() {
    State test_state = create_test_state(sizeof(uint8_t) * 5);
    test_state.memory[0] = 0xd3; // OUT

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x02, "Program counter was not incremented");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");

    cleanup_test_state(test_state);
    return result;
}

int main(int argc, char** argv) {
    run_test_func(test_in, "Test IN");
    run_test_func(test_out, "Test OUT");
}