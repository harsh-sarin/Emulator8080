#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "emulator.h"
#include "test_commons.h"


bool test_lxi_h() {
    State test_state = create_test_state(sizeof(uint8_t) * 4);
    test_state.memory[0] = 0x21;
    test_state.memory[1] = 0x6A;
    test_state.memory[2] = 0x39;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.h, 0x39, "Register H was not loaded with higher order byte");
    result = result & assert_equals(test_state.l, 0x6A, "Register L was not loaded with lower order byte");
    result = result & assert_equals(test_state.pc, 0x03, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_lxi_sp() {
    State test_state = create_test_state(sizeof(uint8_t) * 4);
    test_state.memory[0] = 0x31;
    test_state.memory[1] = 0x11;
    test_state.memory[2] = 0x23;

    Emulate8080(&test_state);

    int result = 1;
    if (test_state.sp != 0x2311) {
        printf("SP was not loaded with immediate 16 bit data\n");
        return false;
    }
    result = result & assert_equals(test_state.pc, 0x03, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_mvi_c() {
    State test_state = create_test_state(sizeof(uint8_t) * 3);
    test_state.memory[0] = 0x0E;
    test_state.memory[1] = 0x3C;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.c, 0x3C, "Register C was not loaded with immediate 8 bit data");
    result = result & assert_equals(test_state.pc, 0x02, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_mvi_m() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0x26; // MVI H, 00
    test_state.memory[1] = 0x00;
    test_state.memory[2] = 0x2E; // MVI L, 09
    test_state.memory[3] = 0x09;
    test_state.memory[9] = 0x00;
    test_state.memory[4] = 0x36; // MVI M, 9F
    test_state.memory[5] = 0x9F;

    for (int i=0; i < 3; i++) {
        Emulate8080(&test_state);
    }

    int result = 1;
    result = result & assert_equals(test_state.memory[9], 0x9F, "Memory location was not loaded with immediate 8 bit data");
    result = result & assert_equals(test_state.pc, 0x06, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_adi() {
    State test_state = create_test_state(sizeof(uint8_t) * 7);
    test_state.memory[0] = 0x3E; // MVI A, 0x14
    test_state.memory[1] = 0x14;
    test_state.memory[2] = 0xC6; // ADI, 0x42
    test_state.memory[3] = 0x42;
    test_state.memory[4] = 0xC6; // ADI, 0xBE
    test_state.memory[5] = 0xBE;

    for (int i=0; i < 2; i++) {
        Emulate8080(&test_state);
    }

    int result = 1;
    result = result & assert_equals(test_state.a, 0x56, "Register A does not contain the expected result of addition: 0x56");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set after first add");
    result = result & assert_equals(test_state.cc.p, 1, "Parity bit was reset after first add");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set after first add");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set after first add");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set after first add");
    
    Emulate8080(&test_state); // Run the second addition

    result = result & assert_equals(test_state.a, 0x14, "Register A does not contain the expected result of addition: 0x14");
    result = result & assert_equals(test_state.cc.cy, 1, "Carry bit was reset after second add");
    result = result & assert_equals(test_state.cc.p, 1, "Parity bit was reset after second add");
    result = result & assert_equals(test_state.cc.ac, 1, "Aux carry bit was reset after second add");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set after second add");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set after second add");
    result = result & assert_equals(test_state.pc, 0x06, "PC was not incremented");

    cleanup_test_state(test_state);
    return result;
}

bool test_aci() {
    State test_state = create_test_state(sizeof(uint8_t) * 7);
    test_state.memory[0] = 0x3E; // MVI A, 0x56
    test_state.memory[1] = 0x56;
    test_state.memory[2] = 0xCE; // ACI, 0xBE
    test_state.memory[3] = 0xBE;
    test_state.memory[4] = 0xCE; // ACI, 0x42
    test_state.memory[5] = 0x42;

    for (int i=0; i < 2; i++) {
        Emulate8080(&test_state);
    }

    int result = 1;
    result = result & assert_equals(test_state.a, 0x14, "Register A does not contain the expected result of addition: 0x14");
    result = result & assert_equals(test_state.cc.cy, 1, "Carry bit was reset after first add with carry");
    result = result & assert_equals(test_state.cc.p, 1, "Parity bit was reset after first add with carry");
    result = result & assert_equals(test_state.cc.ac, 1, "Aux carry bit was reset after first add with carry");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set after first add with carry");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set after first add with carry");
    
    Emulate8080(&test_state); // Run the second addition

    result = result & assert_equals(test_state.a, 0x57, "Register A does not contain the expected result of addition with carry: 0x57");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set after second add with carry");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set after second add with carry");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set after second add with carry");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set after second add with carry");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set after second add with carry");
    result = result & assert_equals(test_state.pc, 0x06, "PC was not incremented");

    cleanup_test_state(test_state);
    return result;
}

bool test_sui() {
    State test_state = create_test_state(sizeof(uint8_t) * 5);
    test_state.memory[0] = 0x3E; // MVI A, 0x20
    test_state.memory[1] = 0x20;
    test_state.memory[2] = 0xD6; // SUI, 0x36
    test_state.memory[3] = 0x36;

    for (int i=0; i < 2; i++) {
        Emulate8080(&test_state);
    }

    int result = 1;
    result = result & assert_equals(test_state.a, 0xEA, "Register A does not contain the expected result of subtraction: 0xEA");
    result = result & assert_equals(test_state.cc.cy, 1, "Carry bit shows borrow did not occur");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set after subtraction");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set after subtraction");
    result = result & assert_equals(test_state.cc.s, 1, "Sign bit was not set after subtraction");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set after subtraction");
    cleanup_test_state(test_state);
    return result;
}

bool test_sbi() {
    State test_state = create_test_state(sizeof(uint8_t) * 5);
    test_state.memory[0] = 0x3E; // MVI A, 0x36
    test_state.memory[1] = 0x36;
    test_state.memory[2] = 0xDE; // SBI, 0x36
    test_state.memory[3] = 0x36;
    test_state.cc.cy = 1;

    for (int i=0; i < 2; i++) {
        Emulate8080(&test_state);
    }

    int result = 1;
    result = result & assert_equals(test_state.a, 0xFF, "Register A does not contain the expected result of subtraction: 0xFF");
    result = result & assert_equals(test_state.cc.cy, 1, "Carry bit shows borrow did not occur");
    result = result & assert_equals(test_state.cc.p, 1, "Parity bit was reset after subtraction");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set after subtraction");
    result = result & assert_equals(test_state.cc.s, 1, "Sign bit was not set after subtraction");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set after subtraction");
    cleanup_test_state(test_state);
    return result;
}

bool test_ani() {
    State test_state = create_test_state(sizeof(uint8_t) * 5);
    test_state.memory[0] = 0x3E; // MVI A, 0x4A
    test_state.memory[1] = 0x4A;
    test_state.memory[2] = 0xE6; // ANI, 0x0F
    test_state.memory[3] = 0x0F;
    test_state.cc.cy = 1;

    for (int i=0; i < 2; i++) {
        Emulate8080(&test_state);
    }

    int result = 1;
    result = result & assert_equals(test_state.a, 0x0A, "Register A does not contain the expected result of logical and: 0x0A");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 1, "Parity bit was reset");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    cleanup_test_state(test_state);
    return result;
}

int main(int argc, char** argv){
    run_test_func(test_lxi_h, "Test LXI H");
    run_test_func(test_lxi_sp, "Test LXI SP");
    run_test_func(test_mvi_c, "Test MVI C");
    run_test_func(test_mvi_m, "Test MVI M");
    run_test_func(test_adi, "Test ADI");
    run_test_func(test_aci, "Test ACI");
    run_test_func(test_sui, "Test SUI");
    run_test_func(test_sbi, "Test SBI");
    run_test_func(test_ani, "Test ANI");
    return 0;
}