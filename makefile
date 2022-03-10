vpath %.c src src/instructions
vpath %.h include
vpath %.o out

includes_path = $(CURDIR)/include 

test: test-register-to-acc test-rotate-acc test-single-register test-data-transfer test-carry-bit

test-single-register: testSinleRegisterInstructions
				./out/testSingleRegisterInstructions

testSingleRegisterInstructions: emulator.o
				gcc -I$(includes_path) ./tests/testSingleRegisterInstructions.c ./out/emulator.o ./out/instructions.o  -o ./out/testSingleRegisterInstructions

test-data-transfer: testDataTransferInstructions
				./out/testDataTransferInstructions

testDataTransferInstructions: emulator.o
				gcc -I$(includes_path) ./tests/testDataTransferInstructions.c ./out/emulator.o ./out/instructions.o  -o ./out/testDataTransferInstructions

test-carry-bit: testCarryBitInstructions
				./out/testCarryBitInstructions

testCarryBitInstructions: emulator.o
				gcc -I$(includes_path) ./tests/testCarryBitInstructions.c ./out/emulator.o ./out/instructions.o  -o ./out/testCarryBitInstructions

test-register-to-acc:  testRegisterToAccumulatorInstructions
				./out/testRegisterToAccumulatorInstructions

testRegisterToAccumulatorInstructions: emulator.o
				gcc -I$(includes_path) ./tests/testRegisterToAccumulatorInstructions.c ./out/emulator.o ./out/instructions.o  -o ./out/testRegisterToAccumulatorInstructions
				
test-rotate-acc: testRotateAccumulatorInstructions
				./out/testRotateAccumulatorInstructions

testRotateAccumulatorInstructions: emulator.o instructions.o
				gcc -I$(includes_path) ./tests/testRotateAccumulatorInstructions.c ./out/emulator.o ./out/instructions.o -o ./out/testRotateAccumulatorInstructions

test-register-pair: test-register-pair-instructions
				./out/test-register-pair-instructions

test-register-pair-instructions: emulator.o helper.o instructions.o register_pair_instructions.o
				gcc -I$(includes_path) ./tests/test_register_pair_instructions.c ./out/emulator.o ./out/instructions.o ./out/register_pair_instructions.o -o ./out/test-register-pair-instructions

test-immediate: test-immediate-instructions
				./out/test-immediate-instructions

test-immediate-instructions: emulator-combined.o
				gcc -I$(includes_path) ./tests/test_immediate_instructions.c ./out/emulator-combined.o -o ./out/test-immediate-instructions


emulator-combined.o: emulator.o helper.o arithmetic_and_logical.o single_register.o register_pair_instructions.o immediate_instructions.o
		gcc -r ./out/emulator.o ./out/helper.o ./out/arithmetic_and_logical.o ./out/single_register.o ./out/register_pair_instructions.o ./out/immediate_instructions.o -o ./out/emulator-combined.o

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

clean:
	rm ./out/*