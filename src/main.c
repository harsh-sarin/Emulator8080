#include "domain.h"
#include "disassembler.h"
#include "emulator.h"


int main(int argc, char** argv) {
	FILE *f = fopen(argv[1], "rb");
	if (f == NULL) {
		printf("Error: Failed to open %s\n", argv[1]);
		exit(1);
	}

	fseek(f, 0L, SEEK_END);
	int fsize = ftell(f);
	fseek(f, 0L, SEEK_SET);	

    printf("Initializing state...\n");
    State state;
    state.memory = (uint8_t *) malloc(fsize);
    printf("Loading memory...\n");
    fread(state.memory, fsize, 1, f);
	fclose(f);
    printf("Memory loaded...\n");
    state.pc = 0x00;

    ConditionCodes condition_codes;
    state.cc = condition_codes;
    state.cc.p = 0;
    state.cc.z = 0;
    state.cc.cy = 0;
    state.cc.ac = 0;
    state.cc.s = 0;
    printf("State ready...\n");

	while (state.pc < fsize) {
        Disassemble8080p(state.memory, state.pc);
		Emulate8080(&state);
	}
	return 0;
}