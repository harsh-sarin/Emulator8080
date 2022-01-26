#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "emulator.h"
#include "test_commons.h"

bool test_push_b() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xC5;
    test_state.b = 0x8F;
    test_state.c = 0x9D;
    test_state.sp = 0x09;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.sp, 0x07, "SP was not updated appropriately after push");
    result = result & assert_equals(test_state.memory[test_state.sp+1], 0x8F, "Higher order register was not pushed to stack");
    result = result & assert_equals(test_state.memory[test_state.sp], 0x9D, "Lower order register was not pushed to stack");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_push_psw() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xF5;
    test_state.a = 0x1F;
    test_state.cc.cy = 1;
    test_state.cc.z = 1;
    test_state.cc.p = 1;
    test_state.sp = 0x09;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.sp, 0x07, "SP was not updated appropriately after push");
    result = result & assert_equals(test_state.memory[test_state.sp+1], 0x1F, "Accumulator was not pushed to stack");
    result = result & assert_equals(test_state.memory[test_state.sp], 0x47, "Condition bit was not pushed to stack");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 1, "Carry bit was reset");
    result = result & assert_equals(test_state.cc.p, 1, "Parity bit was reset");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 1, "Zero bit was reset");
    
    cleanup_test_state(test_state);
    return result;
}


int main(int argc, char** argv){
    run_test_func(test_push_b, "Test PUSH B");
    run_test_func(test_push_psw, "Test PUSH PSW");
    return 0;
}