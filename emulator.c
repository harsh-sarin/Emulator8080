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


void UnimplementedInstruction(State *state)
{
    printf("Error: Unimplemented instruction: %x", state->memory[state->pc]);
    exit(1);
}

int Emulate8080(State *state)
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
    case 0x02:
        // uint16_t offset = (state->b << 8) | (state->c);
        // state->memory[offset] = state->a;
        // state->pc += 1;
        // break;
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
    case 0x0a:
        UnimplementedInstruction(state);
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
    case 0x12:
        UnimplementedInstruction(state);
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
    case 0x1a:
        UnimplementedInstruction(state);
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
    case 0x37:
        UnimplementedInstruction(state);
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
    case 0x3f:
        UnimplementedInstruction(state);
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
    case 0x68:
        UnimplementedInstruction(state);
        break;
    case 0x69:
        UnimplementedInstruction(state);
        break;
    case 0x6a:
        UnimplementedInstruction(state);
        break;
    case 0x6b:
        UnimplementedInstruction(state);
        break;
    case 0x6c:
        UnimplementedInstruction(state);
        break;
    case 0x6d:
        UnimplementedInstruction(state);
        break;
    case 0x6e:
        UnimplementedInstruction(state);
        break;
    case 0x6f:
        UnimplementedInstruction(state);
        break;
    case 0x70:
        UnimplementedInstruction(state);
        break;
    case 0x71:
        UnimplementedInstruction(state);
        break;
    case 0x72:
        UnimplementedInstruction(state);
        break;
    case 0x73:
        UnimplementedInstruction(state);
        break;
    case 0x74:
        UnimplementedInstruction(state);
        break;
    case 0x75:
        UnimplementedInstruction(state);
        break;
    case 0x76:
        UnimplementedInstruction(state);
        break;
    case 0x77:
        UnimplementedInstruction(state);
        break;
    case 0x78:
        UnimplementedInstruction(state);
        break;
    case 0x79:
        UnimplementedInstruction(state);
        break;
    case 0x7a:
        UnimplementedInstruction(state);
        break;
    case 0x7b:
        UnimplementedInstruction(state);
        break;
    case 0x7c:
        UnimplementedInstruction(state);
        break;
    case 0x7d:
        UnimplementedInstruction(state);
        break;
    case 0x7e:
        UnimplementedInstruction(state);
        break;
    case 0x7f:
        UnimplementedInstruction(state);
        break;
    case 0x80:
        UnimplementedInstruction(state);
        break;
    case 0x81:
        UnimplementedInstruction(state);
        break;
    case 0x82:
        UnimplementedInstruction(state);
        break;
    case 0x83:
        UnimplementedInstruction(state);
        break;
    case 0x84:
        UnimplementedInstruction(state);
        break;
    case 0x85:
        UnimplementedInstruction(state);
        break;
    case 0x86:
        UnimplementedInstruction(state);
        break;
    case 0x87:
        UnimplementedInstruction(state);
        break;
    case 0x88:
        UnimplementedInstruction(state);
        break;
    case 0x89:
        UnimplementedInstruction(state);
        break;
    case 0x8a:
        UnimplementedInstruction(state);
        break;
    case 0x8b:
        UnimplementedInstruction(state);
        break;
    case 0x8c:
        UnimplementedInstruction(state);
        break;
    case 0x8d:
        UnimplementedInstruction(state);
        break;
    case 0x8e:
        UnimplementedInstruction(state);
        break;
    case 0x8f:
        UnimplementedInstruction(state);
        break;
    case 0x90:
        UnimplementedInstruction(state);
        break;
    case 0x91:
        UnimplementedInstruction(state);
        break;
    case 0x92:
        UnimplementedInstruction(state);
        break;
    case 0x93:
        UnimplementedInstruction(state);
        break;
    case 0x94:
        UnimplementedInstruction(state);
        break;
    case 0x95:
        UnimplementedInstruction(state);
        break;
    case 0x96:
        UnimplementedInstruction(state);
        break;
    case 0x97:
        UnimplementedInstruction(state);
        break;
    case 0x98:
        UnimplementedInstruction(state);
        break;
    case 0x99:
        UnimplementedInstruction(state);
        break;
    case 0x9a:
        UnimplementedInstruction(state);
        break;
    case 0x9b:
        UnimplementedInstruction(state);
        break;
    case 0x9c:
        UnimplementedInstruction(state);
        break;
    case 0x9d:
        UnimplementedInstruction(state);
        break;
    case 0x9e:
        UnimplementedInstruction(state);
        break;
    case 0x9f:
        UnimplementedInstruction(state);
        break;
    case 0xa0:
        UnimplementedInstruction(state);
        break;
    case 0xa1:
        UnimplementedInstruction(state);
        break;
    case 0xa2:
        UnimplementedInstruction(state);
        break;
    case 0xa3:
        UnimplementedInstruction(state);
        break;
    case 0xa4:
        UnimplementedInstruction(state);
        break;
    case 0xa5:
        UnimplementedInstruction(state);
        break;
    case 0xa6:
        UnimplementedInstruction(state);
        break;
    case 0xa7:
        UnimplementedInstruction(state);
        break;
    case 0xa8:
        UnimplementedInstruction(state);
        break;
    case 0xa9:
        UnimplementedInstruction(state);
        break;
    case 0xaa:
        UnimplementedInstruction(state);
        break;
    case 0xab:
        UnimplementedInstruction(state);
        break;
    case 0xac:
        UnimplementedInstruction(state);
        break;
    case 0xad:
        UnimplementedInstruction(state);
        break;
    case 0xae:
        UnimplementedInstruction(state);
        break;
    case 0xaf:
        UnimplementedInstruction(state);
        break;
    case 0xb0:
        UnimplementedInstruction(state);
        break;
    case 0xb1:
        UnimplementedInstruction(state);
        break;
    case 0xb2:
        UnimplementedInstruction(state);
        break;
    case 0xb3:
        UnimplementedInstruction(state);
        break;
    case 0xb4:
        UnimplementedInstruction(state);
        break;
    case 0xb5:
        UnimplementedInstruction(state);
        break;
    case 0xb6:
        UnimplementedInstruction(state);
        break;
    case 0xb7:
        UnimplementedInstruction(state);
        break;
    case 0xb8:
        UnimplementedInstruction(state);
        break;
    case 0xb9:
        UnimplementedInstruction(state);
        break;
    case 0xba:
        UnimplementedInstruction(state);
        break;
    case 0xbb:
        UnimplementedInstruction(state);
        break;
    case 0xbc:
        UnimplementedInstruction(state);
        break;
    case 0xbd:
        UnimplementedInstruction(state);
        break;
    case 0xbe:
        UnimplementedInstruction(state);
        break;
    case 0xbf:
        UnimplementedInstruction(state);
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