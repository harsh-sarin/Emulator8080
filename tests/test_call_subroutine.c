#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "emulator.h"
#include "test_commons.h"


bool test_call() {
    State test_state = create_test_state(sizeof(uint8_t) * 20);
    test_state.memory[0] = 0xcd; // CALL
    test_state.memory[1] = 0x06;
    test_state.memory[2] = 0x00;
    test_state.memory[6] = 0xdd; // some random instruction
    
    test_state.sp = 0x0F; //SP at address 15

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x06, "Program counter was not jumped to the subroutine");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    result = result & assert_equals(test_state.sp, 0x0D, "Stack pointer was not updated");
    result = result & assert_equals(test_state.memory[0x0D], 0x03, "Stack pointer does not contain the return address");

    cleanup_test_state(test_state);
    return result;
}

bool test_cnc_carry_reset() {
    State test_state = create_test_state(sizeof(uint8_t) * 20);
    test_state.memory[0] = 0xd4; // CNC
    test_state.memory[1] = 0x06;
    test_state.memory[2] = 0x00;
    test_state.memory[6] = 0xdd; // some random instruction
    
    test_state.sp = 0x0F; //SP at address 15

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x06, "Program counter was not jumped to the subroutine");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    result = result & assert_equals(test_state.sp, 0x0D, "Stack pointer was not updated");
    result = result & assert_equals(test_state.memory[0x0D], 0x03, "Stack pointer does not contain the return address");

    cleanup_test_state(test_state);
    return result;
}

bool test_cnc_carry_set() {
    State test_state = create_test_state(sizeof(uint8_t) * 20);
    test_state.memory[0] = 0xd4; // CNC
    test_state.memory[1] = 0x06;
    test_state.memory[2] = 0x00;
    test_state.memory[3] = 0x41;
    test_state.memory[6] = 0xdd; // some random instruction

    test_state.cc.cy = 1;
    
    test_state.sp = 0x0F; //SP at address 15

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x03, "Program counter was jumped to the subroutine");
    result = result & assert_equals(test_state.cc.cy, 1, "Carry bit was reset");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    result = result & assert_equals(test_state.sp, 0x0F, "Stack pointer was updated");

    cleanup_test_state(test_state);
    return result;
}

bool test_cz_zero_reset() {
    State test_state = create_test_state(sizeof(uint8_t) * 20);
    test_state.memory[0] = 0xcc; // CZ
    test_state.memory[1] = 0x06;
    test_state.memory[2] = 0x00;
    test_state.memory[6] = 0xdd; // some random instruction
    
    test_state.sp = 0x0F; //SP at address 15

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x06, "Program counter was not jumped to the subroutine");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    result = result & assert_equals(test_state.sp, 0x0D, "Stack pointer was not updated");
    result = result & assert_equals(test_state.memory[0x0D], 0x03, "Stack pointer does not contain the return address");

    cleanup_test_state(test_state);
    return result;
}

bool test_cz_zero_set() {
    State test_state = create_test_state(sizeof(uint8_t) * 20);
    test_state.memory[0] = 0xcc; // CZ
    test_state.memory[1] = 0x06;
    test_state.memory[2] = 0x00;
    test_state.memory[3] = 0x41;
    test_state.memory[6] = 0xdd; // some random instruction

    test_state.cc.z = 1;
    
    test_state.sp = 0x0F; //SP at address 15

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x03, "Program counter was jumped to the subroutine");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 1, "Zero bit was reset");
    result = result & assert_equals(test_state.sp, 0x0F, "Stack pointer was updated");

    cleanup_test_state(test_state);
    return result;
}

bool test_cnz_zero_set() {
    State test_state = create_test_state(sizeof(uint8_t) * 20);
    test_state.memory[0] = 0xc4; // CNZ
    test_state.memory[1] = 0x06;
    test_state.memory[2] = 0x00;
    test_state.memory[6] = 0xdd; // some random instruction

    test_state.cc.z = 1;
    
    test_state.sp = 0x0F; //SP at address 15

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x06, "Program counter was not jumped to the subroutine");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 1, "Zero bit was reset");
    result = result & assert_equals(test_state.sp, 0x0D, "Stack pointer was not updated");
    result = result & assert_equals(test_state.memory[0x0D], 0x03, "Stack pointer does not contain the return address");

    cleanup_test_state(test_state);
    return result;
}

bool test_cnz_zero_reset() {
    State test_state = create_test_state(sizeof(uint8_t) * 20);
    test_state.memory[0] = 0xc4; // CNZ
    test_state.memory[1] = 0x06;
    test_state.memory[2] = 0x00;
    test_state.memory[3] = 0x41;
    test_state.memory[6] = 0xdd; // some random instruction
    
    test_state.sp = 0x0F; //SP at address 15

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x03, "Program counter was jumped to the subroutine");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    result = result & assert_equals(test_state.sp, 0x0F, "Stack pointer was updated");

    cleanup_test_state(test_state);
    return result;
}

bool test_cm_sign_set() {
    State test_state = create_test_state(sizeof(uint8_t) * 20);
    test_state.memory[0] = 0xfc; // CM
    test_state.memory[1] = 0x06;
    test_state.memory[2] = 0x00;
    test_state.memory[6] = 0xdd; // some random instruction

    test_state.cc.s = 1;
    
    test_state.sp = 0x0F; //SP at address 15

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x06, "Program counter was not jumped to the subroutine");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 1, "Sign bit was reset");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    result = result & assert_equals(test_state.sp, 0x0D, "Stack pointer was not updated");
    result = result & assert_equals(test_state.memory[0x0D], 0x03, "Stack pointer does not contain the return address");

    cleanup_test_state(test_state);
    return result;
}

bool test_cm_sign_reset() {
    State test_state = create_test_state(sizeof(uint8_t) * 20);
    test_state.memory[0] = 0xfc; // CM
    test_state.memory[1] = 0x06;
    test_state.memory[2] = 0x00;
    test_state.memory[3] = 0x41;
    test_state.memory[6] = 0xdd; // some random instruction
    
    test_state.sp = 0x0F; //SP at address 15

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x03, "Program counter was jumped to the subroutine");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    result = result & assert_equals(test_state.sp, 0x0F, "Stack pointer was updated");

    cleanup_test_state(test_state);
    return result;
}

bool test_cpe_parity_set() {
    State test_state = create_test_state(sizeof(uint8_t) * 20);
    test_state.memory[0] = 0xec; // CPE
    test_state.memory[1] = 0x06;
    test_state.memory[2] = 0x00;
    test_state.memory[6] = 0xdd; // some random instruction

    test_state.cc.p = 1;
    
    test_state.sp = 0x0F; //SP at address 15

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x06, "Program counter was not jumped to the subroutine");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 1, "Parity bit was reset");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    result = result & assert_equals(test_state.sp, 0x0D, "Stack pointer was not updated");
    result = result & assert_equals(test_state.memory[0x0D], 0x03, "Stack pointer does not contain the return address");

    cleanup_test_state(test_state);
    return result;
}

bool test_cpe_parity_reset() {
    State test_state = create_test_state(sizeof(uint8_t) * 20);
    test_state.memory[0] = 0xec; // CPE
    test_state.memory[1] = 0x06;
    test_state.memory[2] = 0x00;
    test_state.memory[3] = 0x41;
    test_state.memory[6] = 0xdd; // some random instruction
    
    test_state.sp = 0x0F; //SP at address 15

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x03, "Program counter was jumped to the subroutine");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    result = result & assert_equals(test_state.sp, 0x0F, "Stack pointer was updated");

    cleanup_test_state(test_state);
    return result;
}

int main(int argc, char** argv) {
    run_test_func(test_call, "Test CALL");
    run_test_func(test_cnc_carry_reset, "Test CNC carry reset");
    run_test_func(test_cnc_carry_set, "Test CNC carry set");
    run_test_func(test_cz_zero_reset, "Test CZ zero reset");
    run_test_func(test_cz_zero_set, "Test CZ zero set");
    run_test_func(test_cnz_zero_set, "Test CNZ zero set");
    run_test_func(test_cnz_zero_reset, "Test CNZ zero reset");
    run_test_func(test_cm_sign_set, "Test CM sign set");
    run_test_func(test_cm_sign_set, "Test CM sign reset");
    run_test_func(test_cpe_parity_set, "Test CPE parity set");
    run_test_func(test_cpe_parity_reset, "Test CPE parity reset");
}