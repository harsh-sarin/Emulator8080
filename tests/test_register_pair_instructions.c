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
    result = result & assert_equals(test_state.sp, 0x07, "SP was not decremented appropriately after push");
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
    result = result & assert_equals(test_state.sp, 0x07, "SP was not decremented appropriately after push");
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


bool test_pop_h() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xE1;
    test_state.sp = 0x07;
    test_state.memory[0x07] = 0x3D;
    test_state.memory[0x08] = 0x93;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.sp, 0x09, "SP was not incremented appropriately after pop");
    result = result & assert_equals(test_state.h, 0x93, "Higher order register was not populated from stack as per expectations");
    result = result & assert_equals(test_state.l, 0x3D, "Lower order register was not populated from stack as per expectations");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_pop_psw() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xF1;
    test_state.sp = 0x07;
    test_state.memory[0x07] = 0xC3;
    test_state.memory[0x08] = 0xFF;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.sp, 0x09, "SP was not incremented appropriately after push");
    result = result & assert_equals(test_state.a, 0xFF, "Stack content was not popped to Accumulator");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 1, "Carry bit was reset");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 1, "Sign bit was reset");
    result = result & assert_equals(test_state.cc.z, 1, "Zero bit was reset");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_dad_b() {
    State test_state = create_test_state(sizeof(uint8_t) * 2);
    test_state.memory[0] = 0x09;
    test_state.h = 0xA1;
    test_state.l = 0x7B;
    test_state.b = 0x33;
    test_state.c = 0x9F;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.h, 0xD5, "Register H was not updated after the DAD");
    result = result & assert_equals(test_state.l, 0x1A, "Register L was not updated after the DAD");
    result = result & assert_equals(test_state.b, 0x33, "Register B was updated after the DAD");
    result = result & assert_equals(test_state.c, 0x9F, "Register C was updated after the DAD");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_dad_sp() {
    State test_state = create_test_state(sizeof(uint8_t) * 2);
    test_state.memory[0] = 0x39;
    test_state.h = 0xA1;
    test_state.l = 0xFB;
    test_state.sp = 0xD97B;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.h, 0x7B, "Register H was not updated after the DAD");
    result = result & assert_equals(test_state.l, 0x76, "Register L was not updated after the DAD");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 1, "Carry bit was reset");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_inx_d() {
    State test_state = create_test_state(sizeof(uint8_t) * 2);
    test_state.memory[0] = 0x13;
    test_state.d = 0x38;
    test_state.e = 0xFF;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.d, 0x39, "Register D was not incremented after the INX");
    result = result & assert_equals(test_state.e, 0x00, "Register E was not incremented after the INX");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_inx_sp() {
    State test_state = create_test_state(sizeof(uint8_t) * 2);
    test_state.memory[0] = 0x33;
    test_state.sp = 0xFFFF;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.sp, 0x0000, "Stack Pointer was not incremented after the INX");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_dcx_h() {
    State test_state = create_test_state(sizeof(uint8_t) * 2);
    test_state.memory[0] = 0x2B;
    test_state.h = 0x98;
    test_state.l = 0x00;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.h, 0x97, "Register H was not decremented after the DCX");
    result = result & assert_equals(test_state.l, 0xFF, "Register L was not decremented after the DCX");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_dcx_sp() {
    State test_state = create_test_state(sizeof(uint8_t) * 2);
    test_state.memory[0] = 0x3B;
    test_state.sp = 0x0000;

    Emulate8080(&test_state);

    int result = 1;
    if (test_state.sp != 0xFFFF) {
        printf("Stack Pointer was not decremented after the INX\n");
        return false;
    }
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
    run_test_func(test_push_b, "Test PUSH B");
    run_test_func(test_push_psw, "Test PUSH PSW");
    run_test_func(test_pop_h, "Test POP H");
    run_test_func(test_pop_psw, "Test POP PSW");
    run_test_func(test_dad_b, "Test DAD B");
    run_test_func(test_dad_sp, "Test DAD SP");
    run_test_func(test_inx_d, "Test INX D");
    run_test_func(test_inx_sp, "Test INX SP");
    run_test_func(test_dcx_h, "Test DCX H");
    run_test_func(test_dcx_sp, "Test DCX SP");
    return 0;
}