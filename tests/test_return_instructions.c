#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "emulator.h"
#include "test_commons.h"


bool test_ret() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xc9; //RET
    test_state.memory[5] = 0x07;
    // stack
    test_state.memory[10] = 0x05;
    test_state.memory[11] = 0x00;
    test_state.memory[12] = 0x04;
    
    // SP pointing at lower order byte of the address
    test_state.sp = 0x0A;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x05, "Program counter was not returned to the address in SP");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");    
    result = result & assert_equals(test_state.sp, 0x0C, "Stack pointer was not updated");

    cleanup_test_state(test_state);
    return result;
}

bool test_rc_carry_set() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xd8; //RC
    test_state.memory[5] = 0x07;
    // stack
    test_state.memory[10] = 0x05;
    test_state.memory[11] = 0x00;
    test_state.memory[12] = 0x04;
    
    // SP pointing at lower order byte of the address
    test_state.sp = 0x0A;

    test_state.cc.cy = 1;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x05, "Program counter was not returned to the address in SP");
    result = result & assert_equals(test_state.cc.cy, 1, "Carry bit was reset");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");    
    result = result & assert_equals(test_state.sp, 0x0C, "Stack pointer was not updated");

    cleanup_test_state(test_state);
    return result;
}

bool test_rc_carry_reset() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xd8; //RC
    test_state.memory[1] = 0x03;
    test_state.memory[5] = 0x07;
    // stack
    test_state.memory[10] = 0x05;
    test_state.memory[11] = 0x00;
    test_state.memory[12] = 0x04;
    
    // SP pointing at lower order byte of the address
    test_state.sp = 0x0A;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x01, "Program counter was not incremented to the next instruction");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");    
    result = result & assert_equals(test_state.sp, 0x0A, "Stack pointer was updated");

    cleanup_test_state(test_state);
    return result;
}

bool test_rnc_carry_reset() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xd0; //RNC
    test_state.memory[5] = 0x07;
    // stack
    test_state.memory[10] = 0x05;
    test_state.memory[11] = 0x00;
    test_state.memory[12] = 0x04;
    
    // SP pointing at lower order byte of the address
    test_state.sp = 0x0A;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x05, "Program counter was not returned to the address in SP");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");    
    result = result & assert_equals(test_state.sp, 0x0C, "Stack pointer was not updated");

    cleanup_test_state(test_state);
    return result;
}

bool test_rnc_carry_set() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xd0; //RNC
    test_state.memory[1] = 0x03;
    test_state.memory[5] = 0x07;
    // stack
    test_state.memory[10] = 0x05;
    test_state.memory[11] = 0x00;
    test_state.memory[12] = 0x04;
    
    // SP pointing at lower order byte of the address
    test_state.sp = 0x0A;

    test_state.cc.cy = 1;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x01, "Program counter was not incremented to the next instruction");
    result = result & assert_equals(test_state.cc.cy, 1, "Carry bit was reset");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");    
    result = result & assert_equals(test_state.sp, 0x0A, "Stack pointer was updated");

    cleanup_test_state(test_state);
    return result;
}

bool test_rz_zero_set() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xc8; //RZ
    test_state.memory[5] = 0x07;
    // stack
    test_state.memory[10] = 0x05;
    test_state.memory[11] = 0x00;
    test_state.memory[12] = 0x04;
    
    // SP pointing at lower order byte of the address
    test_state.sp = 0x0A;

    test_state.cc.z = 1;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x05, "Program counter was not returned to the address in SP");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 1, "Zero bit was reset");    
    result = result & assert_equals(test_state.sp, 0x0C, "Stack pointer was not updated");

    cleanup_test_state(test_state);
    return result;
}

bool test_rz_zero_reset() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xc8; //RZ
    test_state.memory[1] = 0x03;
    test_state.memory[5] = 0x07;
    // stack
    test_state.memory[10] = 0x05;
    test_state.memory[11] = 0x00;
    test_state.memory[12] = 0x04;
    
    // SP pointing at lower order byte of the address
    test_state.sp = 0x0A;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x01, "Program counter was not incremented to the next instruction");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");    
    result = result & assert_equals(test_state.sp, 0x0A, "Stack pointer was updated");

    cleanup_test_state(test_state);
    return result;
}

bool test_rnz_zero_reset() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xc0; //RNZ
    test_state.memory[5] = 0x07;
    // stack
    test_state.memory[10] = 0x05;
    test_state.memory[11] = 0x00;
    test_state.memory[12] = 0x04;
    
    // SP pointing at lower order byte of the address
    test_state.sp = 0x0A;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x05, "Program counter was not returned to the address in SP");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");    
    result = result & assert_equals(test_state.sp, 0x0C, "Stack pointer was not updated");

    cleanup_test_state(test_state);
    return result;
}

bool test_rnz_zero_set() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xc0; //RNZ
    test_state.memory[1] = 0x03;
    test_state.memory[5] = 0x07;
    // stack
    test_state.memory[10] = 0x05;
    test_state.memory[11] = 0x00;
    test_state.memory[12] = 0x04;
    
    // SP pointing at lower order byte of the address
    test_state.sp = 0x0A;

    test_state.cc.z = 1;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x01, "Program counter was not incremented to the next instruction");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 1, "Zero bit was reset");    
    result = result & assert_equals(test_state.sp, 0x0A, "Stack pointer was updated");

    cleanup_test_state(test_state);
    return result;
}

bool test_rm_sign_set() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xf8; //RM
    test_state.memory[5] = 0x07;
    // stack
    test_state.memory[10] = 0x05;
    test_state.memory[11] = 0x00;
    test_state.memory[12] = 0x04;
    
    // SP pointing at lower order byte of the address
    test_state.sp = 0x0A;

    test_state.cc.s = 1;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x05, "Program counter was not returned to the address in SP");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 1, "Sign bit was reset");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");    
    result = result & assert_equals(test_state.sp, 0x0C, "Stack pointer was not updated");

    cleanup_test_state(test_state);
    return result;
}

bool test_rm_sign_reset() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xf8; //RM
    test_state.memory[1] = 0x03;
    test_state.memory[5] = 0x07;
    // stack
    test_state.memory[10] = 0x05;
    test_state.memory[11] = 0x00;
    test_state.memory[12] = 0x04;
    
    // SP pointing at lower order byte of the address
    test_state.sp = 0x0A;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x01, "Program counter was not incremented to the next instruction");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");    
    result = result & assert_equals(test_state.sp, 0x0A, "Stack pointer was updated");

    cleanup_test_state(test_state);
    return result;
}

bool test_rp_sign_reset() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xf0; //RP
    test_state.memory[5] = 0x07;
    // stack
    test_state.memory[10] = 0x05;
    test_state.memory[11] = 0x00;
    test_state.memory[12] = 0x04;
    
    // SP pointing at lower order byte of the address
    test_state.sp = 0x0A;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x05, "Program counter was not returned to the address in SP");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");    
    result = result & assert_equals(test_state.sp, 0x0C, "Stack pointer was not updated");

    cleanup_test_state(test_state);
    return result;
}

bool test_rp_sign_set() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xf0; //RP
    test_state.memory[1] = 0x03;
    test_state.memory[5] = 0x07;
    // stack
    test_state.memory[10] = 0x05;
    test_state.memory[11] = 0x00;
    test_state.memory[12] = 0x04;
    
    // SP pointing at lower order byte of the address
    test_state.sp = 0x0A;

    test_state.cc.s = 1;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x01, "Program counter was not incremented to the next instruction");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 1, "Sign bit was reset");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");    
    result = result & assert_equals(test_state.sp, 0x0A, "Stack pointer was updated");

    cleanup_test_state(test_state);
    return result;
}

bool test_rpo_parity_reset() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xe0; //RPO
    test_state.memory[5] = 0x07;
    // stack
    test_state.memory[10] = 0x05;
    test_state.memory[11] = 0x00;
    test_state.memory[12] = 0x04;
    
    // SP pointing at lower order byte of the address
    test_state.sp = 0x0A;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x05, "Program counter was not returned to the address in SP");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");    
    result = result & assert_equals(test_state.sp, 0x0C, "Stack pointer was not updated");

    cleanup_test_state(test_state);
    return result;
}

bool test_rpo_parity_set() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xe0; //RPO
    test_state.memory[1] = 0x03;
    test_state.memory[5] = 0x07;
    // stack
    test_state.memory[10] = 0x05;
    test_state.memory[11] = 0x00;
    test_state.memory[12] = 0x04;
    
    // SP pointing at lower order byte of the address
    test_state.sp = 0x0A;

    test_state.cc.p = 1;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x01, "Program counter was not incremented to the next instruction");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 1, "Parity bit was reset");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");    
    result = result & assert_equals(test_state.sp, 0x0A, "Stack pointer was updated");

    cleanup_test_state(test_state);
    return result;
}

int main(int argc, char** argv) {
    run_test_func(test_ret, "Test RET");
    run_test_func(test_rc_carry_set, "Test RC carry set");
    run_test_func(test_rc_carry_reset, "Test RC carry reset");
    run_test_func(test_rnc_carry_reset, "Test RNC carry reset");
    run_test_func(test_rnc_carry_set, "Test RNC carry set");
    run_test_func(test_rz_zero_set, "Test RZ zero set");
    run_test_func(test_rz_zero_reset, "Test RZ zero reset");
    run_test_func(test_rnz_zero_reset, "Test RNZ zero reset");
    run_test_func(test_rnz_zero_set, "Test RNZ zero set");
    run_test_func(test_rm_sign_set, "Test RM sign set");
    run_test_func(test_rm_sign_reset, "Test RM sign reset");
    run_test_func(test_rp_sign_reset, "Test RP sign reset");
    run_test_func(test_rp_sign_set, "Test RP sign set");
    run_test_func(test_rpo_parity_reset, "Test RPO parity reset");
    run_test_func(test_rpo_parity_set, "Test RPO parity set");
}