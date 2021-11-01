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

bool test_moves_to_register_pair() {
    State test_state = create_test_state(sizeof(uint8_t) * 2);
    test_state.memory[0] = 0x41; // MOV B,C
    test_state.b = 0x40;
    test_state.c = 0x5F;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.b, 0x5F, "Contents of register C was not copied to register B");
    result = result & assert_equals(test_state.c, 0x5F, "Contents of register C were modified");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    
    cleanup_test_state(test_state);
    return result;
}


bool test_moves_to_register_from_memory_location() {
    State test_state = create_test_state(sizeof(uint8_t) * 3);
    test_state.memory[0] = 0x46; // MOV B,M
    test_state.h = 0x00;
    test_state.l = 0x02;
    test_state.memory[0x02] = 0x2E;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.b, 0x2E, "Contents of memory at 0x02 was not copied to register B");
    result = result & assert_equals(test_state.memory[0x02], 0x2E, "Contents of memory at 0x02 were modified");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    
    cleanup_test_state(test_state);
    return result;
}


bool test_moves_to_memory_location_from_register() {
    State test_state = create_test_state(sizeof(uint8_t) * 3);
    test_state.memory[0] = 0x72; // MOV M,D
    test_state.h = 0x00;
    test_state.l = 0x02;
    test_state.d = 0xDF;
    test_state.memory[0x02] = 0x11; // initialize with some random data

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.memory[0x02], 0xDF, "Contents of register D was not copied to memory at 0x02");
    result = result & assert_equals(test_state.d, 0xDF, "Contents of register D were modified");
    result = result & assert_equals(test_state.pc, 0x01, "PC was not incremented");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    
    cleanup_test_state(test_state);
    return result;
}


bool test_STAX_B_AND_D() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.a = 0x4F;
    test_state.b = 0x00;
    test_state.c = 0x09;
    test_state.d = 0x00;
    test_state.e = 0x08;
    test_state.memory[0x0008] = 0x00;
    test_state.memory[0x0009] = 0x00;
    test_state.memory[0x0000] = 0x02; // STAX B
    test_state.memory[0x0001] = 0x12; // STAX D

    Emulate8080(&test_state);
    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.memory[0x0008], 0x4F, "Contents of accumulator were not copied to memory at 0x0008");
    result = result & assert_equals(test_state.memory[0x0009], 0x4F, "Contents of accumulator were not copied to memory at 0x0009");
    result = result & assert_equals(test_state.a, 0x4F, "Contents of accumulator were modified");
    result = result & assert_equals(test_state.pc, 0x02, "PC was not incremented");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");

    cleanup_test_state(test_state);
    return result;
}

bool test_LDAX_B_AND_D() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.a = 0x00;
    test_state.b = 0x00;
    test_state.c = 0x08;
    test_state.d = 0x00;
    test_state.e = 0x09;
    test_state.memory[0x0008] = 0xDE;
    test_state.memory[0x0009] = 0x13;
    test_state.memory[0x0000] = 0x0a; // LDAX B
    test_state.memory[0x0001] = 0x1a; // LDAX D

    Emulate8080(&test_state);
    int result = 1;
    result = result & assert_equals(test_state.a, 0xDE, "Content from location 0x0008 was not copied to accumulator");

    Emulate8080(&test_state);

    result = result & assert_equals(test_state.a, 0x13, "Contents from location 0x0009 was not copied to accumulator");
    result = result & assert_equals(test_state.memory[0x0008], 0xDE, "Contents of memory location 0x0008 were modified");
    result = result & assert_equals(test_state.memory[0x0009], 0x13, "Contents of memory location 0x0009 were modified");
    result = result & assert_equals(test_state.pc, 0x02, "PC was not incremented");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");

    cleanup_test_state(test_state);
    return result;
}


int main(int argc, char** argv){
    run_test_func(test_moves_to_register_pair, "Test data transfer between register pair");
    run_test_func(test_moves_to_register_from_memory_location, "Test data transfer from memory to a register");
    run_test_func(test_moves_to_memory_location_from_register, "Test data transfer from a register to memory");
    run_test_func(test_STAX_B_AND_D, "Test STAX B and STAX D");
    run_test_func(test_LDAX_B_AND_D, "Test LDAX B and STAX D");
    return 0;
}