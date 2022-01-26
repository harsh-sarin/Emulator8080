vpath %.c src src/instructions
vpath %.h include
vpath %.o out

includes_path = $(CURDIR)/include 

test: test-register-to-acc test-rotate-acc test-single-register test-data-transfer test-carry-bit

test-single-register: testSingleRegisterInstructions
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

test-register-pair-instructions: emulator.o instructions.o register_pair_instructions.o
				gcc -I$(includes_path) ./tests/test_register_pair_instructions.c ./out/emulator.o ./out/instructions.o ./out/register_pair_instructions.o -o ./out/test-register-pair-instructions

emulator.o: emulator.c
		gcc -I$(includes_path) -c ./src/emulator.c -o ./out/emulator.o

instructions.o: instructions.c
		gcc -I/$(includes_path) -c ./src/instructions.c -o ./out/instructions.o

register_pair_instructions.o: register_pair_instructions.c
		gcc -I/$(includes_path) -c ./src/instructions/register_pair_instructions.c -o ./out/register_pair_instructions.o

clean:
	rm ./out/*