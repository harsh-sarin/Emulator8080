#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "../emulator.h"

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

bool test_ADD_B() {
    State test_state = create_test_state(sizeof(uint8_t) * 2);
    test_state.memory[0] = 0x80;
    test_state.a = 0x84;
    test_state.b = 0x90;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.a, 0x14, "Add operation did not yield expected results");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 1, "Carry bit was not set");
    result = result & assert_equals(test_state.cc.p, 1, "Parity bit was not set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_ADD_C() {
    State test_state = create_test_state(sizeof(uint8_t) * 2);
    test_state.memory[0] = 0x81;
    test_state.a = 0x6C;
    test_state.c = 0x2C;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.a, 0x98, "Add operation did not yield expected results");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 1, "Aux carry bit was not set");
    result = result & assert_equals(test_state.cc.s, 1, "Sign bit was not set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_ADD_M() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0x86;
    test_state.memory[9] = 0x01;
    test_state.a = 0xFF;
    test_state.h = 0x00;
    test_state.l = 0x09; 

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.a, 0x00, "Add operation did not yield expected results");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 1, "Carry bit was not set");
    result = result & assert_equals(test_state.cc.p, 1, "Parity bit was not set");
    result = result & assert_equals(test_state.cc.ac, 1, "Aux carry bit was not set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 1, "Zero bit was not set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_ADC_D() {
    State test_state = create_test_state(sizeof(uint8_t) * 2);
    test_state.memory[0] = 0x8a;
    test_state.a = 0x3D;
    test_state.d = 0x42;
    test_state.cc.cy = 0;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.a, 0x7f, "ADC operation did not yield expected results");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_ADC_E() {
    State test_state = create_test_state(sizeof(uint8_t) * 2);
    test_state.memory[0] = 0x8b;
    test_state.a = 0x3D;
    test_state.e = 0x42;
    test_state.cc.cy = 1;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.a, 0x80, "ADC operation did not yield expected results");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 1, "Aux carry bit was not set");
    result = result & assert_equals(test_state.cc.s, 1, "Sign bit was not set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_SUB_C() {
    State test_state = create_test_state(sizeof(uint8_t)*2);
    test_state.memory[0] = 0x91;
    test_state.a = 0xc5; //197
    test_state.c = 0x62; //98

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.a, 0x63, "SUB operation did not yield expected results");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 1, "Parity bit was not set");
    result = result & assert_equals(test_state.cc.ac, 1, "Aux carry bit was not set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_SUB_E() {
    State test_state = create_test_state(sizeof(uint8_t)*2);
    test_state.memory[0] = 0x93;
    test_state.a = 0x0C; //12
    test_state.e = 0x0F; //15

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.a, 0xFD, "SUB operation did not yield expected results");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 1, "Carry bit was not set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 1, "Sign bit was not set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_SUB_A() {
    State test_state = create_test_state(sizeof(uint8_t)*2);
    test_state.memory[0] = 0x97;
    test_state.a = 0x3E;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.a, 0x00, "SUB operation did not yield expected results");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 1, "Parity bit was not set");
    result = result & assert_equals(test_state.cc.ac, 1, "Aux carry bit was not set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 1, "Zero bit was not set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_SBB_H() {
    State test_state = create_test_state(sizeof(uint8_t)*2);
    test_state.memory[0] = 0x9C;
    test_state.a = 0x04;
    test_state.h = 0x02;
    test_state.cc.cy = 1;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.a, 0x01, "SBB operation did not yield expected results");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 1, "Aux carry bit was not set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_SBB_M() {
    State test_state = create_test_state(sizeof(uint8_t)*10);
    test_state.memory[0] = 0x9E;
    test_state.memory[9] = 0xFF;
    test_state.a = 0x34;
    test_state.h = 0x00;
    test_state.l = 0x09;
    test_state.cc.cy = 1;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.a, 0x34, "SBB operation did not yield expected results");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 1, "Carry bit was not set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_ANA_B() {
    State test_state = create_test_state(sizeof(uint8_t)*2);
    test_state.memory[0] = 0xa0;
    test_state.a = 0xfc;
    test_state.b = 0x0f;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.a, 0x0c, "ANA operation did not yield expected results");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 1, "Parity bit was not set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_ANA_L() {
    State test_state = create_test_state(sizeof(uint8_t)*2);
    test_state.memory[0] = 0xa5;
    test_state.a = 0xff;
    test_state.l = 0x00;
    test_state.cc.cy = 1;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.a, 0x00, "ANA operation did not yield expected results");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 1, "Parity bit was not set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 1, "Zero bit was not set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_XRA_with_self() {
    State test_state = create_test_state(sizeof(uint8_t)*2);
    test_state.memory[0] = 0xaf; // XRA A
    test_state.a = 0xc3;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.a, 0x00, "XRA operation did not yield expected results");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 1, "Parity bit was not set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 1, "Zero bit was not set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_XRA_one_complement() {
    State test_state = create_test_state(sizeof(uint8_t)*2);
    test_state.memory[0] = 0xaa; // XRA D
    test_state.a = 0x0F;
    test_state.d = 0xFF;
    test_state.cc.cy = 1;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.a, 0xF0, "XRA operation did not yield expected results");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 1, "Parity bit was not set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 1, "Sign bit was not set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");

    cleanup_test_state(test_state);
    return result;
}

bool test_XRA_condition_bit_test() {
    State test_state = create_test_state(sizeof(uint8_t)*10);
    test_state.memory[0] = 0xAE; // XRA M
    test_state.memory[8] = 0x78;
    test_state.a = 0x5C;
    test_state.h = 0x00;
    test_state.l = 0x08;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.a, 0x24, "XRA operation did not yield expected results");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 1, "Parity bit was not set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");

    cleanup_test_state(test_state);
    return result;
}

bool test_ORA_C() {
    State test_state = create_test_state(sizeof(uint8_t)*2);
    test_state.memory[0] = 0xB1;
    test_state.a = 0x33;
    test_state.c = 0x0F;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.a, 0x3F, "ORA operation did not yield expected results");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 1, "Parity bit was not set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");

    cleanup_test_state(test_state);
    return result;
}

bool test_CMP_less_than_accumulator() {
    State test_state = create_test_state(sizeof(uint8_t)*2);
    test_state.memory[0] = 0xBA; // CMP D
    test_state.a = 0x0A;
    test_state.d = 0x05;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.a, 0x0A, "CMP operation changed accumulator");
    result = result & assert_equals(test_state.d, 0x05, "CMP operation changed register");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.z, 0, "Register and accumulator values are marked equal");
    result = result & assert_equals(test_state.cc.cy, 0, "Register value is marked greater than accumulator value");
    result = result & assert_equals(test_state.cc.p, 1, "Parity bit was not set");
    result = result & assert_equals(test_state.cc.ac, 1, "Aux carry bit was not set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");

    cleanup_test_state(test_state);
    return result;
}

bool test_CMP_more_than_accumulator() {
    State test_state = create_test_state(sizeof(uint8_t)*2);
    test_state.memory[0] = 0xBC; // CMP H
    test_state.a = 0x02;
    test_state.h = 0x07;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.a, 0x02, "CMP operation changed accumulator");
    result = result & assert_equals(test_state.h, 0x07, "CMP operation changed register");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.z, 0, "Register and accumulator values are marked equal");
    result = result & assert_equals(test_state.cc.cy, 1, "Register value is marked less than accumulator value");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 1, "Sign bit was not set");

    cleanup_test_state(test_state);
    return result;
}

bool test_CMP_equal_to_accumulator() {
    State test_state = create_test_state(sizeof(uint8_t)*2);
    test_state.memory[0] = 0xBF; // CMP A
    test_state.a = 0x05;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.a, 0x05, "CMP operation changed accumulator");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.z, 1, "Accumulator compared with self is not marked equal");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 1, "Parity bit was not set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");

    cleanup_test_state(test_state);
    return result;
}

int main(int argc, char** argv){
    run_test_func(test_ADD_B, "Test ADD B");
    run_test_func(test_ADD_C, "Test ADD C");
    run_test_func(test_ADD_M, "Test ADD M");
    run_test_func(test_ADC_D, "Test ADC D");
    run_test_func(test_ADC_E, "Test ADC E");
    run_test_func(test_SUB_C, "Test SUB C");
    run_test_func(test_SUB_E, "Test SUB E");
    run_test_func(test_SUB_A, "Test SUB A");
    run_test_func(test_SBB_H, "Test SBB H");
    run_test_func(test_SBB_M, "Test SBB M");
    run_test_func(test_ANA_B, "Test ANA B");
    run_test_func(test_ANA_L, "Test ANA L");
    run_test_func(test_XRA_with_self, "Test XRA with self");
    run_test_func(test_XRA_one_complement, "Test XRA does one complement");
    run_test_func(test_XRA_condition_bit_test, "Test XRA used for testing condition bits");
    run_test_func(test_ORA_C, "Test ORA C");
    run_test_func(test_CMP_less_than_accumulator, "Test CMP register < acc");
    run_test_func(test_CMP_more_than_accumulator, "Test CMP register > acc");
    run_test_func(test_CMP_equal_to_accumulator, "Test CMP acc == acc");
    return 0;
}