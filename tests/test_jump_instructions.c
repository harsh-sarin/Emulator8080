#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "emulator.h"
#include "test_commons.h"


bool test_pchl() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xe9; // PCHL
    test_state.memory[9] = 0xdd; // some random instruction
    test_state.h = 0x00;
    test_state.l = 0x09;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x09, "Program counter was not jumped to address from H and L registers");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");

    cleanup_test_state(test_state);
    return result;
}

bool test_jmp() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xc3; //JMP 
    test_state.memory[1] = 0x08;
    test_state.memory[2] = 0x00;
    test_state.memory[8] = 0xf1; //some random instruction

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x08, "Program counter was not jumped to address specified in JMP instruction");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");    

    cleanup_test_state(test_state);
    return result;
}

bool test_jc_carry_set() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xda; //JC 
    test_state.memory[1] = 0x09;
    test_state.memory[2] = 0x00;
    test_state.memory[9] = 0xf1; //some random instruction

    test_state.cc.cy = 1;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x09, "Program counter was not jumped to address specified in JC instruction");
    result = result & assert_equals(test_state.cc.cy, 1, "Carry bit was reset");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");    

    cleanup_test_state(test_state);
    return result;
}

bool test_jc_carry_reset() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xda; //JC 
    test_state.memory[1] = 0x08;
    test_state.memory[2] = 0x00;
    test_state.memory[8] = 0xf1; //some random instruction

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x03, "Program counter was jumped to address specified in JC instruction");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");    

    cleanup_test_state(test_state);
    return result;
}

bool test_jnc_carry_reset() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xd2; //JNC 
    test_state.memory[1] = 0x08;
    test_state.memory[2] = 0x00;
    test_state.memory[8] = 0xf1; //some random instruction

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x08, "Program counter was not jumped to address specified in JNC instruction");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");    

    cleanup_test_state(test_state);
    return result;
    
}

bool test_jnc_carry_set() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xd2; //JNC 
    test_state.memory[1] = 0x08;
    test_state.memory[2] = 0x00;
    test_state.memory[8] = 0xf1; //some random instruction
    test_state.cc.cy = 1;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x03, "Program counter was jumped to address specified in JNC instruction");
    result = result & assert_equals(test_state.cc.cy, 1, "Carry bit was reset");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");    

    cleanup_test_state(test_state);
    return result;
}

bool test_jz_zero_set() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xca; //JZ
    test_state.memory[1] = 0x07;
    test_state.memory[2] = 0x00;
    test_state.memory[7] = 0xf1; //some random instruction
    test_state.cc.z = 1;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x07, "Program counter was not jumped to address specified in JZ instruction");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 1, "Zero bit was reset");    

    cleanup_test_state(test_state);
    return result;
    
}

bool test_jz_zero_reset() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xca; //JZ 
    test_state.memory[1] = 0x07;
    test_state.memory[2] = 0x00;
    test_state.memory[7] = 0xf1; //some random instruction

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x03, "Program counter was jumped to address specified in JZ instruction");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");    

    cleanup_test_state(test_state);
    return result;
}

bool test_jnz_zero_reset() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xc2; //JNZ
    test_state.memory[1] = 0x09;
    test_state.memory[2] = 0x00;
    test_state.memory[9] = 0xf1; //some random instruction

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x09, "Program counter was not jumped to address specified in JNZ instruction");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");    

    cleanup_test_state(test_state);
    return result;
    
}

bool test_jnz_zero_set() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xc2; //JNZ 
    test_state.memory[1] = 0x07;
    test_state.memory[2] = 0x00;
    test_state.memory[7] = 0xf1; //some random instruction
    test_state.cc.z = 1;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x03, "Program counter was jumped to address specified in JNZ instruction");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 1, "Zero bit was reset");    

    cleanup_test_state(test_state);
    return result;
}

bool test_jm_sign_set() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xfa; //JM
    test_state.memory[1] = 0x07;
    test_state.memory[2] = 0x00;
    test_state.memory[7] = 0xf1; //some random instruction
    test_state.cc.s = 1;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x07, "Program counter was not jumped to address specified in JM instruction");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 1, "Sign bit was reset");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");    

    cleanup_test_state(test_state);
    return result;
    
}

bool test_jm_sign_reset() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xfa; //JZ 
    test_state.memory[1] = 0x07;
    test_state.memory[2] = 0x00;
    test_state.memory[7] = 0xf1; //some random instruction

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x03, "Program counter was jumped to address specified in JM instruction");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");    

    cleanup_test_state(test_state);
    return result;
}

bool test_jp_sign_reset() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xf2; //JP
    test_state.memory[1] = 0x09;
    test_state.memory[2] = 0x00;
    test_state.memory[9] = 0xf1; //some random instruction

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x09, "Program counter was not jumped to address specified in JP instruction");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");    

    cleanup_test_state(test_state);
    return result; 
}

bool test_jp_sign_set() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xf2; //JP
    test_state.memory[1] = 0x07;
    test_state.memory[2] = 0x00;
    test_state.memory[7] = 0xf1; //some random instruction
    test_state.cc.s = 1;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x03, "Program counter was jumped to address specified in JP instruction");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 1, "Sign bit was reset");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");    

    cleanup_test_state(test_state);
    return result;
}

bool test_jpo_parity_reset() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xe2; //JPO
    test_state.memory[1] = 0x09;
    test_state.memory[2] = 0x00;
    test_state.memory[9] = 0xf1; //some random instruction

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x09, "Program counter was not jumped to address specified in JPO instruction");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 0, "Parity bit was set");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");    

    cleanup_test_state(test_state);
    return result; 
}

bool test_jpo_parity_set() {
    State test_state = create_test_state(sizeof(uint8_t) * 10);
    test_state.memory[0] = 0xe2; //JPO
    test_state.memory[1] = 0x07;
    test_state.memory[2] = 0x00;
    test_state.memory[7] = 0xf1; //some random instruction
    test_state.cc.p = 1;

    Emulate8080(&test_state);

    int result = 1;
    result = result & assert_equals(test_state.pc, 0x03, "Program counter was jumped to address specified in JPO instruction");
    result = result & assert_equals(test_state.cc.cy, 0, "Carry bit was set");
    result = result & assert_equals(test_state.cc.p, 1, "Parity bit was reset");
    result = result & assert_equals(test_state.cc.ac, 0, "Aux carry bit was set");
    result = result & assert_equals(test_state.cc.s, 0, "Sign bit was set");
    result = result & assert_equals(test_state.cc.z, 0, "Zero bit was set");    

    cleanup_test_state(test_state);
    return result;
}

int main(int argc, char** argv) {
    run_test_func(test_pchl, "Test PCHL");
    run_test_func(test_jmp, "Test JMP");
    run_test_func(test_jc_carry_set, "Test JC carry set");
    run_test_func(test_jc_carry_reset, "Test JC carry reset");
    run_test_func(test_jnc_carry_reset, "Test JNC carry reset");
    run_test_func(test_jnc_carry_set, "Test JNC carry set");
    run_test_func(test_jz_zero_set, "Test JZ zero set");
    run_test_func(test_jz_zero_reset, "Test JZ zero reset");
    run_test_func(test_jnz_zero_reset, "Test JNZ zero reset");
    run_test_func(test_jnz_zero_set, "Test JNZ zero set");
    run_test_func(test_jm_sign_set, "Test JM sign set");
    run_test_func(test_jm_sign_reset, "Test JM sign reset");
    run_test_func(test_jp_sign_reset, "Test JP sign reset");
    run_test_func(test_jp_sign_set, "Test JP sign set");
    run_test_func(test_jpo_parity_reset, "Test JPO parity reset");
    run_test_func(test_jpo_parity_set, "Test JPO parity set");
}