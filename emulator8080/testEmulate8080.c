#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "emulator.h"


bool test_LXI() {
    State test_state;

    ConditionCodes condition_codes;
    test_state.cc = condition_codes;

    uint8_t* mem;
    mem = (uint8_t *) malloc(sizeof(uint8_t) * 4);
    test_state.memory = mem;
    test_state.memory[0] = 0x01;
    test_state.memory[1] = 0x3A;
    test_state.memory[2] = 0x02;
    test_state.pc = 0x00;

    int ret_val = Emulate8080(&test_state);

    return test_state.b == 0x02 
        && test_state.c == 0x3A
        && test_state.pc == 0x003;
}

bool test_INR() {
    State test_state;

    ConditionCodes condition_codes;
    test_state.cc = condition_codes;

    uint8_t* mem;
    mem = (uint8_t *) malloc(sizeof(uint8_t) * 2);
    test_state.memory = mem;
    test_state.memory[0] = 0x04;
    test_state.pc = 0x00;

    test_state.b = 0x3A;

    int ret_val = Emulate8080(&test_state);

    return test_state.b == 0x3B && test_state.pc == 0x01;
    
}

bool test_RLC() {
    State test_state;

    ConditionCodes condition_codes;
    test_state.cc = condition_codes;

    uint8_t* mem;
    mem = (uint8_t *) malloc(sizeof(uint8_t) * 2);
    test_state.memory = mem;
    test_state.memory[0] = 0x07;
    test_state.pc = 0x00;

    test_state.a = 0xF2;

    int ret_val = Emulate8080(&test_state);

    return test_state.a == 0xE5 && test_state.cc.cy == 1;
    
}


int main(int argc, char** argv){
    if (test_LXI() && test_INR() && test_RLC()) {
        printf("Test success\n");
    } else {
        printf("Test failure\n");
    }
    return 0;
}