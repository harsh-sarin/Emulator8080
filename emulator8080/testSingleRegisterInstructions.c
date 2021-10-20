#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "emulator.h"


void red() {
    printf("\033[1;31m");
}

void reset() {
    printf("\033[0m");
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

bool test_INR_B() {
    State test_state = create_test_state(sizeof(uint8_t) * 2);
    test_state.memory[0] = 0x04; //INR B

    test_state.b = 0x3B;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.b, 0x3C, "Register B was not incremented");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.p, 1, "Parity bit was not set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_INR_C() {
    State test_state = create_test_state(sizeof(uint8_t) * 2);
    test_state.memory[0] = 0x0c; //INR C

    test_state.c = 0x14;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.c, 0x15, "Register C was not incremented");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_INR_D() {
    State test_state = create_test_state(sizeof(uint8_t) * 2);
    test_state.memory[0] = 0x14; //INR D

    test_state.d = 0x5F;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.d, 0x60, "Register D was not incremented");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.p, 1, "Parity bit was not set");
    result = result & assert_equals(test_state.cc.ac, 1, "Aux carry bit was not set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_INR_E() {
    State test_state = create_test_state(sizeof(uint8_t) * 2);
    test_state.memory[0] = 0x1c; //INR E

    test_state.e = 0xa1;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.e, 0xa2, "Register E was not incremented");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 1, "Sign bit was not set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_INR_H() {
    State test_state = create_test_state(sizeof(uint8_t) * 2);
    test_state.memory[0] = 0x24; //INR H

    test_state.h = 0xFF;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.h, 0x00, "Register H was not incremented");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.p, 1, "Parity bit was not set");
    result = result & assert_equals(test_state.cc.ac, 1, "Aux carry bit was not set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 1, "Zero bit was not set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_INR_L() {
    State test_state = create_test_state(sizeof(uint8_t) * 2);
    test_state.memory[0] = 0x2c; //INR L

    test_state.l = 0xFF;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.l, 0x00, "Register L was not incremented");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.p, 1, "Parity bit was not set");
    result = result & assert_equals(test_state.cc.ac, 1, "Aux carry bit was not set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 1, "Zero bit was not set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_INR_memref() {
    State test_state = create_test_state(sizeof(uint8_t) * 2);
    test_state.memory[0] = 0x34; //INR mem referenced by H + L
    test_state.memory[2] = 0x11;
    
    test_state.h = 0x00;
    test_state.l = 0x02;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.memory[2], 0x12, "Data at memory reference was not incremented");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.p, 1, "Parity bit was not set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_INR_A() {
    State test_state = create_test_state(sizeof(uint8_t) * 2);
    test_state.memory[0] = 0x3c; //INR A

    test_state.a = 0x03;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.a, 0x04, "Register A was not incremented");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

void test_INR() {
    run_test_func(test_INR_B, "INR B");
    run_test_func(test_INR_C, "INR C");
    run_test_func(test_INR_D, "INR D");
    run_test_func(test_INR_E, "INR E");
    run_test_func(test_INR_H, "INR H");
    run_test_func(test_INR_L, "INR L");
    run_test_func(test_INR_memref, "INR mem ref");
    run_test_func(test_INR_A, "INR A");
 }

 bool test_DCR_B() {
    State test_state = create_test_state(sizeof(uint8_t) * 2);
    test_state.memory[0] = 0x05; //DCR B

    test_state.b = 0x30;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.b, 0x2F, "Register B was not decremented");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_DCR_C() {
    State test_state = create_test_state(sizeof(uint8_t) * 2);
    test_state.memory[0] = 0x0d; //DCR C

    test_state.c = 0x01;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.c, 0x00, "Register C was not decremented");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.p, 1, "Parity bit was not set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 1, "Zero bit was not set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_DCR_D() {
    State test_state = create_test_state(sizeof(uint8_t) * 2);
    test_state.memory[0] = 0x15; //DCR D

    test_state.d = 0x12;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.d, 0x11, "Register D was not decremented");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.p, 1, "Parity bit was not set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_DCR_E() {
    State test_state = create_test_state(sizeof(uint8_t) * 2);
    test_state.memory[0] = 0x1d; //DCR E

    test_state.e = 0x00;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.e, 0xFF, "Register E was not decremented");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.p, 1, "Parity bit was not set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 1, "Sign bit was not set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_DCR_H() {
    State test_state = create_test_state(sizeof(uint8_t) * 2);
    test_state.memory[0] = 0x25; //DCR H

    test_state.h = 0xFF;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.h, 0xFE, "Register H was not decremented");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 1, "Sign bit was not set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_DCR_L() {
    State test_state = create_test_state(sizeof(uint8_t) * 2);
    test_state.memory[0] = 0x2d; //DCR L

    test_state.l = 0x43;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.l, 0x42, "Register L was not decremented");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.p, 1, "Parity bit was not set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_DCR_memref() {
    State test_state = create_test_state(sizeof(uint8_t) * 2);
    test_state.memory[0] = 0x35; //DCR mem referenced by H + L
    test_state.memory[2] = 0x11;
    
    test_state.h = 0x00;
    test_state.l = 0x02;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.memory[2], 0x10, "Data at memory reference was not decremented");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

bool test_DCR_A() {
    State test_state = create_test_state(sizeof(uint8_t) * 2);
    test_state.memory[0] = 0x3d; //DCR A

    test_state.a = 0x04;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.a, 0x03, "Register A was not decremented");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.p, 1, "Parity bit was not set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    
    cleanup_test_state(test_state);
    return result;
}

void test_DCR() {
    run_test_func(test_DCR_B, "DCR B");
    run_test_func(test_DCR_C, "DCR C");
    run_test_func(test_DCR_D, "DCR D");
    run_test_func(test_DCR_E, "DCR E");
    run_test_func(test_DCR_H, "DCR H");
    run_test_func(test_DCR_L, "DCR L");
    run_test_func(test_DCR_memref, "DCR mem ref");
    run_test_func(test_DCR_A, "DCR A");
 }

/*
1. Handle overflows during increment. Will the byte = 0x00?
2. When 0x00 is decremented should it become 0xFF and sign bit be set to 1?
*/


int main(int argc, char** argv){
    test_INR();
    test_DCR();
    return 0;
}