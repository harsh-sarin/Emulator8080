#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "emulator.h"

bool is_even_parity(uint8_t some_data)
{
    uint8_t count = 0, mask = 1;
    for (int i = 0; i < sizeof(uint8_t) * 8; i++)
    {
        if (some_data & (mask << i))
        {
            count++;
        }
    }
    return (count % 2 == 0);
}

bool is_even_parity_16bit(uint16_t some_data)
{
    uint8_t count = 0;
    uint16_t mask = 1;
    for (int i = 0; i < sizeof(uint16_t) * 8; i++)
    {
        if (some_data & (mask << i))
        {
            count++;
        }
    }
    return (count % 2 == 0);
}

void single_dcr_register(State *some_state, uint8_t *some_register)
{
    uint8_t unmodified_val = *some_register;
    (*some_register)--;
    uint8_t modified_val = *some_register;
    some_state->cc.z = (modified_val == 0x00);
    some_state->cc.s = ((modified_val & 0x80) == 0x80);
    some_state->cc.p = is_even_parity(modified_val);
    some_state->cc.ac = 0; // Assumption: no aux carry in decrement operation
}

void single_inr_register(State *some_state, uint8_t *some_register)
{
    uint8_t unmodified_val = *some_register;
    (*some_register)++;
    uint8_t modified_val = *some_register;
    some_state->cc.z = (modified_val == 0x00);
    some_state->cc.s = ((modified_val & 0x80) == 0x80);
    some_state->cc.p = is_even_parity(modified_val);
    // if bit at pos 3 before modification is 1 and after modificaiton is 0,
    // it implies a carry came out of pos 3 after the increment.
    if ((unmodified_val & 0x08) == 0x08 && (modified_val & 0x08) == 0x00)
    {
        some_state->cc.ac = 1;
    }
}

void single_inr_memory(State *some_state)
{
    uint16_t offset = (some_state->h << 8) | (some_state->l);
    uint16_t unmodified_val = some_state->memory[offset];
    some_state->memory[offset]++;
    uint16_t modified_val = some_state->memory[offset];
    some_state->cc.z = (modified_val == 0x0000);
    some_state->cc.s = ((modified_val & 0x8000) == 0x8000);
    some_state->cc.p = is_even_parity_16bit(modified_val);
    // if bit at pos 3 before modification is 1 and after modificaiton is 0,
    // it implies a carry came out of pos 3 after the increment.
    if ((unmodified_val & 0x0008) == 0x0008 && (modified_val & 0x0008) == 0x0000)
    {
        some_state->cc.ac = 1;
    }
}

void single_dcr_memory(State *some_state)
{
    uint16_t offset = (some_state->h << 8) | (some_state->l);
    uint16_t unmodified_val = some_state->memory[offset];
    some_state->memory[offset]--;
    uint16_t modified_val = some_state->memory[offset];
    some_state->cc.z = (modified_val == 0x0000);
    some_state->cc.s = ((modified_val & 0x8000) == 0x8000);
    some_state->cc.p = is_even_parity_16bit(modified_val);
    some_state->cc.ac = 0; // Assumption: no aux carry in decrement operation
}

void update_condition_codes_due_to_add(State *some_state, uint16_t sum, uint8_t addend) {
    some_state->cc.cy = (sum > 0xFF);
    some_state->cc.z = ((sum & 0xFF) == 0);
    some_state->cc.s = ((sum & 0x80) == 0x80);
    some_state->cc.p = is_even_parity((sum & 0xFF));
    bool is_acc_bit_pos_3_set = ((some_state->a & 0x08) == 0x08);
    bool is_addend_bit_pos_3_set = ((addend & 0x08) == 0x08);
    // If both the operands have the 3rd bit set, there is bound to be a carry out of that position when
    // these bits are added.
    // If at least one operand has the 3rd bit set, and the result of adding these bits yields 0, it implies that 
    // a carry from adding lower order bits propagated to bit 3, resuling in carry out of bit 3 as well.
    // 
    if ((is_acc_bit_pos_3_set && is_addend_bit_pos_3_set) || 
        ((is_acc_bit_pos_3_set || is_addend_bit_pos_3_set) && (sum & 0x08) == 0)){
        some_state->cc.ac = 1;
    } else {
        some_state->cc.ac = 0;
    }
}

void add_value_to_accumulator(State *some_state, uint8_t addend) {
    uint16_t sum = (uint16_t) some_state->a + (uint16_t) addend;
    update_condition_codes_due_to_add(some_state, sum, addend);
    some_state->a = sum & 0xFF;
}

void add_value_and_carry_to_accumulator(State *some_state, uint8_t addend) {
    uint16_t sum = (uint16_t) some_state->a + (uint16_t) addend + (some_state->cc.cy == 1 ? 0x0001 : 0x0000);
    update_condition_codes_due_to_add(some_state, sum, addend);
    some_state->a = sum & 0xFF;
}

void update_condition_codes_due_to_subtract(State *some_state, uint16_t difference, uint8_t subtrahend) {
    some_state->cc.cy = !(difference > 0xFF);
    some_state->cc.z = ((difference & 0xFF) == 0);
    some_state->cc.s = ((difference & 0x80) == 0x80);
    some_state->cc.p = is_even_parity((difference & 0xFF));
    bool is_acc_bit_pos_3_set = ((some_state->a & 0x08) == 0x08);
    bool is_subtrahend_bit_pos_3_set = ((subtrahend & 0x08) == 0x08);
    if ((is_acc_bit_pos_3_set && is_subtrahend_bit_pos_3_set) || 
        ((is_acc_bit_pos_3_set || is_subtrahend_bit_pos_3_set) && (difference & 0x08) == 0)){
        some_state->cc.ac = 1;
    } else {
        some_state->cc.ac = 0;
    }
}

void subtract_value_from_accumulator(State *some_state, uint8_t subtrahend) {
    uint8_t subtrahend_two_complement = (~subtrahend) + 0x01;
    uint16_t difference = (uint16_t) some_state->a + (uint16_t) subtrahend_two_complement;
    update_condition_codes_due_to_subtract(some_state, difference, subtrahend_two_complement);
    some_state->a = difference & 0xFF;
}

void subtract_value_and_borrow_from_accumulator(State *some_state, uint8_t subtrahend) {
    uint8_t subtrahend_with_borrow_added = (subtrahend + (some_state->cc.cy == 1 ? 0x01 : 0x00)) & 0xFF;
    uint8_t subtrahend_two_complement = (~subtrahend_with_borrow_added) + 0x01;
    uint16_t difference = (uint16_t) some_state->a + (uint16_t) subtrahend_two_complement;
    update_condition_codes_due_to_subtract(some_state, difference, subtrahend_two_complement);
    some_state->a = difference & 0xFF;
}

void ana(State* some_state, uint8_t value_to_and) {
    some_state->a &= value_to_and;
    some_state->cc.cy = 0;
    some_state->cc.z = ((some_state->a & 0xFF) == 0);
    some_state->cc.s = ((some_state->a & 0x80) == 0x80);
    some_state->cc.p = is_even_parity(some_state->a);
}

void xra(State* some_state, uint8_t value_to_xor) {
    some_state->a ^= value_to_xor;
    some_state->cc.cy = 0;
    some_state->cc.z = ((some_state->a & 0xFF) == 0);
    some_state->cc.s = ((some_state->a & 0x80) == 0x80);
    some_state->cc.p = is_even_parity(some_state->a);
}

void ora(State* some_state, uint8_t value_to_and) {
    some_state->a |= value_to_and;
    some_state->cc.cy = 0;
    some_state->cc.z = ((some_state->a & 0xFF) == 0);
    some_state->cc.s = ((some_state->a & 0x80) == 0x80);
    some_state->cc.p = is_even_parity(some_state->a);
}

void cmp(State* some_state, uint8_t value_to_compare) {
    uint8_t subtrahend_two_complement = (~value_to_compare) + 0x01;
    uint16_t difference = (uint16_t) some_state->a + (uint16_t) subtrahend_two_complement;
    update_condition_codes_due_to_subtract(some_state, difference, value_to_compare);
}


void UnimplementedInstruction(State *state)
{
    printf("Error: Unimplemented instruction: %x", state->memory[state->pc]);
    exit(1);
}

void Emulate8080(State *state)
{
    unsigned char *opcode = &state->memory[state->pc];
    switch (*opcode)
    {
    case 0x00:
        break;
    case 0x01: // LXI B,immediate data
        state->c = opcode[1];
        state->b = opcode[2];
        state->pc += 3;
        break;
    case 0x02: // STAX B
        {
            uint16_t offset = (state->b << 8) | (state->c);
            state->memory[offset] = state->a;
            state->pc += 1;
        }
        break;
    case 0x03:
        UnimplementedInstruction(state);
        break;
    case 0x04: // INR B
        single_inr_register(state, &state->b);
        state->pc += 1;
        break;
    case 0x05: // DCR B
        single_dcr_register(state, &state->b);
        state->pc += 1;
        break;
    case 0x06: // MVI B,immediate data
        state->b = opcode[1];
        state->pc += 2;
        break;
    case 0x07: // RLC
    {
        state->cc.cy = ((state->a & 0x80) == 0x80);
        state->a = (state->a << 1) | (state->a >> (8 - 1));
        state->pc += 1;
    }
    break;
    case 0x08:
        UnimplementedInstruction(state);
        break;
    case 0x09:
        UnimplementedInstruction(state);
        break;
    case 0x0a: // LDAX B
        {
            uint16_t offset = (state->b << 8) | (state->c);
            state->a = state->memory[offset];
            state->pc += 1;
        }
        break;
    case 0x0b:
        UnimplementedInstruction(state);
        break;
    case 0x0c:
        single_inr_register(state, &state->c);
        state->pc += 1;
        break;
    case 0x0d: // DCR C
        single_dcr_register(state, &state->c);
        state->pc += 1;
        break;
    case 0x0e: // MVI C, immediate data
        state->c = opcode[1];
        state->pc += 2;
        break;
    case 0x0f:
        UnimplementedInstruction(state);
        break;
    case 0x10:
        UnimplementedInstruction(state);
        break;
    case 0x11:
        UnimplementedInstruction(state);
        break;
    case 0x12: // STAX D
        {
            uint16_t offset = (state->d << 8) | (state->e);
            state->memory[offset] = state->a;
            state->pc += 1;
        }
        break;
    case 0x13:
        UnimplementedInstruction(state);
        break;
    case 0x14:
        single_inr_register(state, &state->d);
        state->pc += 1;
        break;
    case 0x15:
        single_dcr_register(state, &state->d);
        state->pc += 1;
        break;
    case 0x16:
        UnimplementedInstruction(state);
        break;
    case 0x17:
        UnimplementedInstruction(state);
        break;
    case 0x18:
        UnimplementedInstruction(state);
        break;
    case 0x19:
        UnimplementedInstruction(state);
        break;
    case 0x1a:// LDAX D
        {
            uint16_t offset = (state->d << 8) | (state->e);
            state->a = state->memory[offset];
            state->pc += 1;
        }
        break;
    case 0x1b:
        UnimplementedInstruction(state);
        break;
    case 0x1c:
        single_inr_register(state, &state->e);
        state->pc += 1;
        break;
    case 0x1d:
        single_dcr_register(state, &state->e);
        state->pc += 1;
        break;
    case 0x1e:
        UnimplementedInstruction(state);
        break;
    case 0x1f:
        UnimplementedInstruction(state);
        break;
    case 0x20:
        UnimplementedInstruction(state);
        break;
    case 0x21:
        UnimplementedInstruction(state);
        break;
    case 0x22:
        UnimplementedInstruction(state);
        break;
    case 0x23:
        UnimplementedInstruction(state);
        break;
    case 0x24: // INR H
        single_inr_register(state, &state->h);
        state->pc += 1;
        break;
    case 0x25: // DCR H
        single_dcr_register(state, &state->h);
        state->pc += 1;
        break;
    case 0x26:
        UnimplementedInstruction(state);
        break;
    case 0x27: // DAA
    {
        uint8_t lower_order = state->a & 0x0F;
        if (lower_order > 0x09 || state->cc.ac == 1)
        {
            uint8_t unmodified_val = state->a;
            state->a += 0x06;
            if ((unmodified_val & 0x08) == 0x08 && (state->a & 0x08) == 0x00)
            {
                state->cc.ac = 1;
            }
            else
            {
                state->cc.ac = 0;
            }
        }
        uint8_t higher_order = state->a & 0xF0;
        if (higher_order > 0x09 || state->cc.cy == 1)
        {
            uint8_t unmodified_val = state->a;
            state->a += 0x60;
            if ((unmodified_val & 0x80) == 0x80 && (state->a & 0x80) == 0x00)
            {
                state->cc.cy = 1;
            }
            else
            {
                state->cc.cy = 0;
            }
        }
        state->cc.z = (state->a == 0x00);
        state->cc.s = ((state->a & 0x80) == 0x80);
        state->cc.p = is_even_parity(state->a);
        state->pc += 1;
    }
    break;
    case 0x28:
        UnimplementedInstruction(state);
        break;
    case 0x29:
        UnimplementedInstruction(state);
        break;
    case 0x2a:
        UnimplementedInstruction(state);
        break;
    case 0x2b:
        UnimplementedInstruction(state);
        break;
    case 0x2c: // INR L
        single_inr_register(state, &state->l);
        state->pc += 1;
        break;
    case 0x2d: // DCR L
        single_dcr_register(state, &state->l);
        state->pc += 1;
        break;
    case 0x2e:
        UnimplementedInstruction(state);
        break;
    case 0x2f: // CMA
        state->a = ~state->a;
        state->pc += 1;
        break;
    case 0x30:
        UnimplementedInstruction(state);
        break;
    case 0x31:
        UnimplementedInstruction(state);
        break;
    case 0x32:
        UnimplementedInstruction(state);
        break;
    case 0x33:
        UnimplementedInstruction(state);
        break;
    case 0x34: // INR M
        single_inr_memory(state);
        state->pc += 1;
        break;
    case 0x35: // DCR M
        single_dcr_memory(state);
        state->pc += 1;
        break;
    case 0x36:
        UnimplementedInstruction(state);
        break;
    case 0x37: // STC
        state->cc.cy = 1;
        state->pc += 1;
        break;
    case 0x38:
        UnimplementedInstruction(state);
        break;
    case 0x39:
        UnimplementedInstruction(state);
        break;
    case 0x3a:
        UnimplementedInstruction(state);
        break;
    case 0x3b:
        UnimplementedInstruction(state);
        break;
    case 0x3c: // INR A
        single_inr_register(state, &state->a);
        state->pc += 1;
        break;
    case 0x3d: // DCR A
        single_dcr_register(state, &state->a);
        state->pc += 1;
        break;
    case 0x3e:
        UnimplementedInstruction(state);
        break;
    case 0x3f: // CMC
        state->cc.cy = ~state->cc.cy;
        state->pc += 1;
        break;
    case 0x40: // MOV B,B
        state->pc += 1;
        break;
    case 0x41: // MOV B,C
        state->b = state->c;
        state->pc += 1;
        break;
    case 0x42: // MOV B,D
        state->b = state->d;
        state->pc += 1;
        break;
    case 0x43: // MOV B,E
        state->b = state->e;
        state->pc += 1;
        break;
    case 0x44: // MOV B,H
        state->b = state->h;
        state->pc += 1;
        break;
    case 0x45: // MOV B,L
        state->b = state->l;
        state->pc += 1;
        break;
    case 0x46: // MOV B,M
        {
            uint16_t memory_location = (state->h << 8) | (state->l);
            state->b = state->memory[memory_location];
            state->pc += 1;
        }
        break;
    case 0x47: // MOV B,A
        state->b = state->a;
        state->pc += 1;
        break;
    case 0x48: // MOV C,B
        state->c = state->b;
        state->pc += 1;
        break;
    case 0x49: // MOV C,C
        state->pc += 1;
        break;
    case 0x4a: // MOV C,D
        state->c = state->d;
        state->pc += 1;
        break;
    case 0x4b: // MOV C,E
        state->c = state->e;
        state->pc += 1;
        break;
    case 0x4c: // MOV C,H
        state->c = state->h;
        state->pc += 1;
        break;
    case 0x4d: // MOV C,L
        state->c = state->l;
        state->pc += 1;
        break;
    case 0x4e: // MOV C,M
        {
            uint16_t memory_location = (state->h << 8) | (state->l);
            state->c = state->memory[memory_location];
            state->pc += 1;
        }
        break;
    case 0x4f: // MOV C,A
        state->c = state->a;
        state->pc += 1;
        break;
    case 0x50: // MOV D,B
        state->d = state->b;
        state->pc += 1;
        break;
    case 0x51: // MOV D,C
        state->d = state->c;
        state->pc += 1;
        break;
    case 0x52: // MOV D,D
        state->pc += 1;
        break;
    case 0x53: // MOV D,E
        state->d = state->e;
        state->pc += 1;
        break;
    case 0x54: // MOV D,H
        state->d = state->h;
        state->pc += 1;
        break;
    case 0x55: // MOV D,L
        state->d = state->l;
        state->pc += 1;
        break;
    case 0x56: // MOV D,M
        {
            uint16_t memory_location = (state->h << 8) | (state->l);
            state->d = state->memory[memory_location];
            state->pc += 1;
        }
        break;
    case 0x57: // MOV D,A
        state->d = state->a;
        state->pc += 1;
        break;
    case 0x58: // MOV E,B
        state->e = state->b;
        state->pc += 1;
        break;
    case 0x59: // MOV E,C
        state->e = state->c;
        state->pc += 1;
        break;
    case 0x5a: // MOV E,D
        state->e = state->d;
        state->pc += 1;
        break;
    case 0x5b: // MOV E,E
        state->pc += 1;
        break;
    case 0x5c: // MOV E,H
        state->e = state->h;
        state->pc += 1;
        break;
    case 0x5d: // MOV E,L
        state->e = state->l;
        state->pc += 1;
        break;
    case 0x5e: // MOV E,M
        {
            uint16_t memory_location = (state->h << 8) | (state->l);
            state->e = state->memory[memory_location];
            state->pc += 1;
        }
        break;
    case 0x5f: // MOV E,A
        state->e = state->a;
        state->pc += 1;
        break;
    case 0x60: // MOV H,B
        state->h = state->b;
        state->pc += 1;
        break;
    case 0x61: // MOV H,C
        state->h = state->c;
        state->pc += 1;
        break;
    case 0x62: // MOV H,D
        state->h = state->d;
        state->pc += 1;
        break;
    case 0x63: // MOV H,E
        state->h = state->e;
        state->pc += 1;
        break;
    case 0x64: // MOV H,H
        state->pc += 1;
        break;
    case 0x65: // MOV H,L
        state->h = state->l;
        state->pc += 1;
        break;
    case 0x66: // MOV H,M
        {
            uint16_t memory_location = (state->h << 8) | (state->l);
            state->h = state->memory[memory_location];
            state->pc += 1;
        }
        break;
    case 0x67: // MOV H,A
        state->h = state->a;
        state->pc += 1;
        break;
    case 0x68: // MOV L,B
        state->l = state->b;
        state->pc += 1;
        break;
    case 0x69: // MOV L,C
        state->l = state->c;
        state->pc += 1;
        break;
    case 0x6a: // MOV L,D
        state->l = state->d;
        state->pc += 1;
        break;
    case 0x6b: // MOV L,E
        state->l = state->e;
        state->pc += 1;
        break;
    case 0x6c: // MOV L,H
        state->l = state->h;
        state->pc += 1;
        break;
    case 0x6d: // MOV L,L
        state->pc += 1;
        break;
    case 0x6e: // MOV L,M
        {
            uint16_t memory_location = (state->h << 8) | (state->l);
            state->l = state->memory[memory_location];
            state->pc += 1;
        }
        break;
    case 0x6f: // MOV L,A
        state->l = state->a;
        state->pc += 1;
        break;
    case 0x70: // MOV M,B
        {
            uint16_t memory_location = (state->h << 8) | (state->l);
            state->memory[memory_location] = state->b;
            state->pc += 1;
        }
        break;
    case 0x71: // MOV M,C
        {
            uint16_t memory_location = (state->h << 8) | (state->l);
            state->memory[memory_location] = state->c;
            state->pc += 1;
        }
        break;
    case 0x72: // MOV M,D
        {
            uint16_t memory_location = (state->h << 8) | (state->l);
            state->memory[memory_location] = state->d;
            state->pc += 1;
        }
        break;
    case 0x73: // MOV M,E
        {
            uint16_t memory_location = (state->h << 8) | (state->l);
            state->memory[memory_location] = state->e;
            state->pc += 1;
        }
        break;
    case 0x74: // MOV M,H
        {
            uint16_t memory_location = (state->h << 8) | (state->l);
            state->memory[memory_location] = state->h;
            state->pc += 1;
        }
        break;
    case 0x75: // MOV M,L
        {
            uint16_t memory_location = (state->h << 8) | (state->l);
            state->memory[memory_location] = state->l;
            state->pc += 1;
        }
        break;
    case 0x76:
        UnimplementedInstruction(state);
        break;
    case 0x77: // MOV M,A
        {
            uint16_t memory_location = (state->h << 8) | (state->l);
            state->memory[memory_location] = state->a;
            state->pc += 1;
        }
        break;
    case 0x78: // MOV A,B
        state->a = state->b;
        state->pc += 1;
        break;
    case 0x79: // MOV A,C
        state->a = state->c;
        state->pc += 1;
        break;
    case 0x7a: // MOV A,D
        state->a = state->d;
        state->pc += 1;
        break;
    case 0x7b: // MOV A,E
        state->a = state->e;
        state->pc += 1;
        break;
    case 0x7c: // MOV A,H
        state->a = state->h;
        state->pc += 1;
        break;
    case 0x7d: // MOV A,L
        state->a = state->l;
        state->pc += 1;
        break;
    case 0x7e: // MOV A,M
        {
            uint16_t memory_location = (state->h << 8) | (state->l);
            state->a = state->memory[memory_location];
            state->pc += 1;
        }
        break;
    case 0x7f: // MOV A,A
        state->pc += 1;
        break;
    case 0x80: // ADD B
        add_value_to_accumulator(state, state->b);
        state->pc +=1;
        break;
    case 0x81: // ADD C
        add_value_to_accumulator(state, state->c);
        state->pc +=1;
        break;
    case 0x82: // ADD D
        add_value_to_accumulator(state, state->d);
        state->pc +=1;
        break;
    case 0x83: // ADD E
        add_value_to_accumulator(state, state->d);
        state->pc +=1;
        break;
    case 0x84: // ADD H
        add_value_to_accumulator(state, state->d);
        state->pc +=1;
        break;
    case 0x85: // ADD L
        add_value_to_accumulator(state, state->d);
        state->pc +=1;
        break;
    case 0x86: // ADD M
        {
            uint16_t memory_location = (state->h << 8) | (state->l);
            add_value_to_accumulator(state, state->memory[memory_location]);
            state->pc +=1;
        }
        break;
    case 0x87: // ADD A
        add_value_to_accumulator(state, state->a);
        state->pc +=1;
        break;
    case 0x88: // ADC B
        add_value_and_carry_to_accumulator(state, state->b);
        state->pc +=1;
        break;
    case 0x89: // ADC C
        add_value_and_carry_to_accumulator(state, state->c);
        state->pc +=1;
        break;
    case 0x8a: // ADC D
        add_value_and_carry_to_accumulator(state, state->d);
        state->pc +=1;
        break;
    case 0x8b: // ADC E
        add_value_and_carry_to_accumulator(state, state->e);
        state->pc +=1;
        break;
    case 0x8c: // ADC H
        add_value_and_carry_to_accumulator(state, state->h);
        state->pc +=1;
        break;
    case 0x8d: // ADC L
        add_value_and_carry_to_accumulator(state, state->l);
        state->pc +=1;
        break;
    case 0x8e: // ADC M
        {
            uint16_t memory_location = (state->h << 8) | (state->l);
            add_value_and_carry_to_accumulator(state, state->memory[memory_location]);
            state->pc +=1;
        }
        break;
    case 0x8f: // ADC A
        add_value_and_carry_to_accumulator(state, state->a);
        state->pc +=1;
        break;
    case 0x90: // SUB B
        subtract_value_from_accumulator(state, state->b);
        state->pc +=1;
        break;
    case 0x91: // SUB C
        subtract_value_from_accumulator(state, state->c);
        state->pc +=1;
        break;
    case 0x92: // SUB D
        subtract_value_from_accumulator(state, state->d);
        state->pc +=1;
        break;
    case 0x93: // SUB E
        subtract_value_from_accumulator(state, state->e);
        state->pc +=1;
        break;
    case 0x94: // SUB H
        subtract_value_from_accumulator(state, state->h);
        state->pc +=1;
        break;
    case 0x95: // SUB L
        subtract_value_from_accumulator(state, state->l);
        state->pc +=1;
        break;
    case 0x96: // SUB M
        {
            uint16_t memory_location = (state->h << 8) | (state->l);
            subtract_value_from_accumulator(state, state->memory[memory_location]);
            state->pc +=1;
        }
        break;
    case 0x97: // SUB A
        subtract_value_from_accumulator(state, state->a);
        state->pc +=1;
        break;
    case 0x98: // SBB B
        subtract_value_and_borrow_from_accumulator(state, state->b);
        state->pc += 1;
        break;
    case 0x99: // SBB C
        subtract_value_and_borrow_from_accumulator(state, state->c);
        state->pc += 1;
        break;
    case 0x9a: // SBB D
        subtract_value_and_borrow_from_accumulator(state, state->d);
        state->pc += 1;
        break;
    case 0x9b: // SBB E
        subtract_value_and_borrow_from_accumulator(state, state->e);
        state->pc += 1;
        break;
    case 0x9c: // SBB H
        subtract_value_and_borrow_from_accumulator(state, state->h);
        state->pc += 1;
        break;
    case 0x9d: // SBB L
        subtract_value_and_borrow_from_accumulator(state, state->l);
        state->pc += 1;
        break;
    case 0x9e: // SBB M
        {
            uint16_t memory_location = (state->h << 8) | (state->l);
            subtract_value_and_borrow_from_accumulator(state, state->memory[memory_location]);
            state->pc +=1;
        }
        break;
    case 0x9f: // SBB A
        subtract_value_and_borrow_from_accumulator(state, state->a);
        state->pc += 1;
        break;
    case 0xa0: // ANA B
        ana(state, state->b);
        state->pc += 1;
        break;
    case 0xa1: // ANA C
        ana(state, state->c);
        state->pc += 1;
        break;
    case 0xa2: // ANA D
        ana(state, state->d);
        state->pc += 1;
        break;
    case 0xa3: // ANA E
        ana(state, state->e);
        state->pc += 1;
        break;
    case 0xa4: // ANA H
        ana(state, state->h);
        state->pc += 1;
        break;
    case 0xa5: // ANA L
        ana(state, state->b);
        state->pc += 1;
        break;
    case 0xa6:
        {
            uint16_t memory_location = (state->h << 8) | (state->l);
            ana(state, state->memory[memory_location]);
            state->pc +=1;
        }
        break;
    case 0xa7: // ANA A
        ana(state, state->a);
        state->pc += 1;
        break;
    case 0xa8: // XRA B
        xra(state, state->b);
        state->pc += 1;
        break;
    case 0xa9: // XRA C
        xra(state, state->c);
        state->pc += 1;
        break;
    case 0xaa: // XRA D
        xra(state, state->d);
        state->pc += 1;
        break;
    case 0xab: // XRA E
        xra(state, state->e);
        state->pc += 1;
        break;
    case 0xac: // XRA H
        xra(state, state->h);
        state->pc += 1;
        break;
    case 0xad: // XRA L
        xra(state, state->l);
        state->pc += 1;
        break;
    case 0xae: // XRA M
        {
            uint16_t memory_location = (state->h << 8) | (state->l);
            xra(state, state->memory[memory_location]);
            state->pc +=1;
        }
        break;
    case 0xaf: // XRA A
        xra(state, state->a);
        state->pc += 1;
        break;
    case 0xb0: // ORA B
        ora(state, state->b);
        state->pc +=1;
        break;
    case 0xb1: // ORA C
        ora(state, state->c);
        state->pc +=1;
        break;
    case 0xb2: // ORA D
        ora(state, state->d);
        state->pc +=1;
        break;
    case 0xb3: // ORA E
        ora(state, state->e);
        state->pc +=1;
        break;
    case 0xb4: // ORA H
        ora(state, state->h);
        state->pc +=1;
        break;
    case 0xb5: // ORA L
        ora(state, state->l);
        state->pc +=1;
        break;
    case 0xb6: // ORA M
        {
            uint16_t memory_location = (state->h << 8) | (state->l);
            ora(state, state->memory[memory_location]);
            state->pc +=1;
        }
        break;
    case 0xb7: // ORA A
        ora(state, state->a);
        state->pc +=1;
        break;
    case 0xb8: // CMP B
        cmp(state, state->b);
        state->pc += 1;
        break;
    case 0xb9: // CMP C
        cmp(state, state->c);
        state->pc += 1;
        break;
    case 0xba: // CMP D
        cmp(state, state->d);
        state->pc += 1;
        break;
    case 0xbb: // CMP E
        cmp(state, state->e);
        state->pc += 1;
        break;
    case 0xbc: // CMP H
        cmp(state, state->h);
        state->pc += 1;
        break;
    case 0xbd: // CMP L
        cmp(state, state->l);
        state->pc += 1;
        break;
    case 0xbe: // CMP M
        {
            uint16_t memory_location = (state->h << 8) | (state->l);
            cmp(state, state->memory[memory_location]);
            state->pc +=1;
        }
        break;
    case 0xbf: // CMP A
        cmp(state, state->a);
        state->pc += 1;
        break;
    case 0xc0:
        UnimplementedInstruction(state);
        break;
    case 0xc1:
        UnimplementedInstruction(state);
        break;
    case 0xc2:
        UnimplementedInstruction(state);
        break;
    case 0xc3:
        UnimplementedInstruction(state);
        break;
    case 0xc4:
        UnimplementedInstruction(state);
        break;
    case 0xc5:
        UnimplementedInstruction(state);
        break;
    case 0xc6:
        UnimplementedInstruction(state);
        break;
    case 0xc7:
        UnimplementedInstruction(state);
        break;
    case 0xc8:
        UnimplementedInstruction(state);
        break;
    case 0xc9:
        UnimplementedInstruction(state);
        break;
    case 0xca:
        UnimplementedInstruction(state);
        break;
    case 0xcb:
        UnimplementedInstruction(state);
        break;
    case 0xcc:
        UnimplementedInstruction(state);
        break;
    case 0xcd:
        UnimplementedInstruction(state);
        break;
    case 0xce:
        UnimplementedInstruction(state);
        break;
    case 0xcf:
        UnimplementedInstruction(state);
        break;
    case 0xd0:
        UnimplementedInstruction(state);
        break;
    case 0xd1:
        UnimplementedInstruction(state);
        break;
    case 0xd2:
        UnimplementedInstruction(state);
        break;
    case 0xd3:
        UnimplementedInstruction(state);
        break;
    case 0xd4:
        UnimplementedInstruction(state);
        break;
    case 0xd5:
        UnimplementedInstruction(state);
        break;
    case 0xd6:
        UnimplementedInstruction(state);
        break;
    case 0xd7:
        UnimplementedInstruction(state);
        break;
    case 0xd8:
        UnimplementedInstruction(state);
        break;
    case 0xd9:
        UnimplementedInstruction(state);
        break;
    case 0xda:
        UnimplementedInstruction(state);
        break;
    case 0xdb:
        UnimplementedInstruction(state);
        break;
    case 0xdc:
        UnimplementedInstruction(state);
        break;
    case 0xdd:
        UnimplementedInstruction(state);
        break;
    case 0xde:
        UnimplementedInstruction(state);
        break;
    case 0xdf:
        UnimplementedInstruction(state);
        break;
    case 0xe0:
        UnimplementedInstruction(state);
        break;
    case 0xe1:
        UnimplementedInstruction(state);
        break;
    case 0xe2:
        UnimplementedInstruction(state);
        break;
    case 0xe3:
        UnimplementedInstruction(state);
        break;
    case 0xe4:
        UnimplementedInstruction(state);
        break;
    case 0xe5:
        UnimplementedInstruction(state);
        break;
    case 0xe6:
        UnimplementedInstruction(state);
        break;
    case 0xe7:
        UnimplementedInstruction(state);
        break;
    case 0xe8:
        UnimplementedInstruction(state);
        break;
    case 0xe9:
        UnimplementedInstruction(state);
        break;
    case 0xea:
        UnimplementedInstruction(state);
        break;
    case 0xeb:
        UnimplementedInstruction(state);
        break;
    case 0xec:
        UnimplementedInstruction(state);
        break;
    case 0xed:
        UnimplementedInstruction(state);
        break;
    case 0xee:
        UnimplementedInstruction(state);
        break;
    case 0xef:
        UnimplementedInstruction(state);
        break;
    case 0xf0:
        UnimplementedInstruction(state);
        break;
    case 0xf1:
        UnimplementedInstruction(state);
        break;
    case 0xf2:
        UnimplementedInstruction(state);
        break;
    case 0xf3:
        UnimplementedInstruction(state);
        break;
    case 0xf4:
        UnimplementedInstruction(state);
        break;
    case 0xf5:
        UnimplementedInstruction(state);
        break;
    case 0xf6:
        UnimplementedInstruction(state);
        break;
    case 0xf7:
        UnimplementedInstruction(state);
        break;
    case 0xf8:
        UnimplementedInstruction(state);
        break;
    case 0xf9:
        UnimplementedInstruction(state);
        break;
    case 0xfa:
        UnimplementedInstruction(state);
        break;
    case 0xfb:
        UnimplementedInstruction(state);
        break;
    case 0xfc:
        UnimplementedInstruction(state);
        break;
    case 0xfd:
        UnimplementedInstruction(state);
        break;
    case 0xfe:
        UnimplementedInstruction(state);
        break;
    case 0xff:
        UnimplementedInstruction(state);
        break;
    }
}