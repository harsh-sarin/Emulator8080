#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "emulator.h"
#include "test_commons.h"


bool test_ei() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xfb; //EI
    test_state.inte = false;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x01, "Program counter was not incremented");
    result = result & test_state.inte;
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");    

    cleanup_test_state(test_state);
    return result;
}

bool test_di() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xf3; //DI
    test_state.inte = true;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x01, "Program counter was not incremented");
    result = result & !test_state.inte;
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");    

    cleanup_test_state(test_state);
    return result;
}

bool test_rst() {
    State test_state = create_test_state(sizeof(uint8_t) * 75);
    test_state.memory[56] = 0x01; // start of service routine at RST 7
    test_state.memory[68] = 0xff; //RST 7
    test_state.pc = 0x44; //current program execution
    test_state.inte = true; //enable interrupts
    test_state.sp = 75;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x38, "Program counter was not updated to start address of service routine");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    result = result & assert_equals(test_state.sp, 73, "Stack pointer was not updated");
    result = result & assert_equals(test_state.memory[73], 68, "Stack does not contain the lower half of the return address");
    result = result & assert_equals(test_state.memory[72], 0x00, "Stack does not contain the lower half of the return address");

    cleanup_test_state(test_state);
    return result;
}

bool test_rst_inte_disabled() {
    State test_state = create_test_state(sizeof(uint8_t) * 75);
    test_state.memory[8] = 0x01; // start of service routine at RST 1
    test_state.memory[68] = 0xcf; //RST 1
    test_state.pc = 0x44; //current program execution
    test_state.inte = false; //disable interrupts
    test_state.sp = 75;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x44, "Program counter was updated when an exception was expected");
    result = result & test_state.exception;
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    result = result & assert_equals(test_state.sp, 75, "Stack pointer was updated when an exception was expected");

    cleanup_test_state(test_state);
    return result;
}

int main(int argc, char** argv) {
    run_test_func(test_ei, "Test EI");
    run_test_func(test_di, "Test DI");
    run_test_func(test_rst, "Test RST");
    run_test_func(test_rst_inte_disabled, "Test RST INTE disabled");
}