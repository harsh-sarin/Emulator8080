vpath %.c src src/instructions
vpath %.h include
vpath %.o out

includes_path = $(CURDIR)/include 

test: test-single-register test-data-transfer test-carry-bit test-register-to-acc test-rotate-acc  test-direct-addressing test-immediate test-register-pair-instructions test-jump

test-single-register: emulator-combined.o
				gcc -I$(includes_path) ./tests/testSingleRegisterInstructions.c ./out/emulator-combined.o  -o ./out/testSingleRegisterInstructions
				./out/testSingleRegisterInstructions

test-data-transfer: emulator-combined.o
				gcc -I$(includes_path) ./tests/testDataTransferInstructions.c ./out/emulator-combined.o -o ./out/testDataTransferInstructions
				./out/testDataTransferInstructions

test-carry-bit: emulator-combined.o
				gcc -I$(includes_path) ./tests/testCarryBitInstructions.c ./out/emulator-combined.o  -o ./out/testCarryBitInstructions
				./out/testCarryBitInstructions

test-register-to-acc: emulator-combined.o
				gcc -I$(includes_path) ./tests/testRegisterToAccumulatorInstructions.c ./out/emulator-combined.o  -o ./out/testRegisterToAccumulatorInstructions
				./out/testRegisterToAccumulatorInstructions

test-rotate-acc: emulator-combined.o
				gcc -I$(includes_path) ./tests/testRotateAccumulatorInstructions.c ./out/emulator-combined.o -o ./out/testRotateAccumulatorInstructions
				./out/testRotateAccumulatorInstructions

test-register-pair-instructions: emulator-combined.o
				gcc -I$(includes_path) ./tests/test_register_pair_instructions.c ./out/emulator-combined.o -o ./out/test-register-pair-instructions
				./out/test-register-pair-instructions

test-immediate: emulator-combined.o
				gcc -I$(includes_path) ./tests/test_immediate_instructions.c ./out/emulator-combined.o -o ./out/test-immediate-instructions
				./out/test-immediate-instructions

test-direct-addressing: emulator-combined.o
				gcc -I$(includes_path) ./tests/test_direct_addressing.c ./out/emulator-combined.o -o ./out/test_direct_addressing
				./out/test_direct_addressing

test-jump: emulator-combined.o
				gcc -I$(includes_path) ./tests/test_jump_instructions.c ./out/emulator-combined.o -o ./out/test_jump_instructions
				./out/test_jump_instructions

emulator-combined.o: emulator.o helper.o arithmetic_and_logical.o single_register.o register_pair_instructions.o immediate_instructions.o direct_addressing.o jump_instructions.o
		gcc -r ./out/emulator.o ./out/helper.o ./out/arithmetic_and_logical.o ./out/single_register.o ./out/register_pair_instructions.o ./out/immediate_instructions.o ./out/direct_addressing.o ./out/jump_instructions.o -o ./out/emulator-combined.o

emulator.o: emulator.c
		gcc -I$(includes_path) -c ./src/emulator.c -o ./out/emulator.o

helper.o: helper.c
		gcc -I$(includes_path) -c ./src/helper.c -o ./out/helper.o

arithmetic_and_logical.o: arithmetic_and_logical.c
		gcc -I/$(includes_path) -c ./src/instructions/arithmetic_and_logical.c -o ./out/arithmetic_and_logical.o

single_register.o: single_register.c
		gcc -I/$(includes_path) -c ./src/instructions/single_register.c -o ./out/single_register.o

register_pair_instructions.o: register_pair_instructions.c
		gcc -I/$(includes_path) -c ./src/instructions/register_pair_instructions.c -o ./out/register_pair_instructions.o

immediate_instructions.o: immediate_instructions.c
		gcc -I/$(includes_path) -c ./src/instructions/immediate_instructions.c -o ./out/immediate_instructions.o

direct_addressing.o: direct_addressing.c
		gcc -I/$(includes_path) -c ./src/instructions/direct_addressing.c -o ./out/direct_addressing.o

jump_instructions.o: jump_instructions.c
		gcc -I/$(includes_path) -c ./src/instructions/jump_instructions.c -o ./out/jump_instructions.o

clean:
	rm ./out/*